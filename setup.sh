#!/bin/bash
# SuperNETs UnrealIRCd source update script - Developed by acidvegas (https://github.com/supernets/unrealircd)
# unrealircd/update-source.sh

set -xev

# Load environment variables
source .env

# Commonly used UnrealIRCd paths
UNREAL=/opt/ircd
SOURCE=/home/agent/unrealircd.source
BACKUP=/home/agent/unrealircd.backup


container_create() {
    # Create the container
    incus storage create unrealircd-pool dir
    incus launch images:debian/12 unrealircd-container -s unrealircd-pool
    incus config set unrealircd-container boot.autostart true

    sleep 10 # Sleep for 10 seconds to ensure DHCP has assigned an IP address

    # Basic provisioning
    incus exec unrealircd-container -- sh -c 'printf "[Journal]\nStorage=volatile\nSplitMode=none\nRuntimeMaxUse=500K\n" > /etc/systemd/journald.conf'
    incus exec unrealircd-container -- systemctl restart systemd-journald
    incus exec unrealircd-container -- bash -c "apt update  -y && apt upgrade -y && apt install -y curl cron git nano unattended-upgrades wget"
    incus exec unrealircd-container -- apt install -y build-essential pkg-config gdb libssl-dev libpcre2-dev libargon2-dev libsodium-dev libc-ares-dev libcurl4-openssl-dev
    incus exec unrealircd-container -- useradd -m -s /bin/bash agent

    # Download & compile the source code
    incus exec unrealircd-container -- chown agent:agent /opt
    incus exec unrealircd-container -- sudo -u agent git clone --depth 1 https://github.com/supernets/unrealircd.git $SOURCE
    incus exec unrealircd-container -- sudo -u agent bash -c "cd $SOURCE/unrealircd && echo -e "\n" | ./Config -nointro && make && make install && rm -rf $SOURCE"

    # Add cronjobs
    incus exec unrealircd-container -- sudo -u agent bash -c 'echo "*/5 * * * * $UNREAL/unrealircd croncheck" | crontab -'
    incus exec unrealircd-container -- sudo -u agent bash -c 'echo "@reboot     $UNREAL/unrealircd croncheck" | crontab -'

    # IPv4 Port Forwarding
    CONTAINER_IPV4=$(incus list | grep unrealircd-container | awk '{print $6}')
    PUBLIC_IPV4=$(curl 4.icanhazip.com)
    #PRIVATE_IPV4=$(ip route get 8.8.8.8 | awk '{print $7}' | head -1) # Replace PUBLIC_IPV4 below with this if running on AWS
    incus config device override unrealircd-container eth0
    incus config device set unrealircd-container eth0 ipv4.address=${CONTAINER_IPV4}
    incus config device add unrealircd-container unrealircd-plaintext-ipv4-port proxy listen=tcp:${PUBLIC_IPV4}:6660-6669,7000 connect=tcp:${CONTAINER_IPV4}:6667        nat=true
    incus config device add unrealircd-container unrealircd-tls-ipv4-port       proxy listen=tcp:${PUBLIC_IPV4}:6697,9000      connect=tcp:${CONTAINER_IPV4}:6697        nat=true
    incus config device add unrealircd-container unrealircd-servers-ipv4-port   proxy listen=tcp:${PUBLIC_IPV4}:${S2S_PORT}    connect=tcp:${CONTAINER_IPV4}:${S2S_PORT} nat=true

    # IPv6 Port Forwarding
    CONTAINER_IPV6=$(incus list | grep unrealircd-container | awk '{print $9}')
    PUBLIC_IPV6=$(curl 6.icanhazip.com)
    incus config device set unrealircd-container eth0 ipv6.address=${CONTAINER_IPV6}
    incus network set incusbr0 ipv6.dhcp.stateful=true
    incus config device add unrealircd-container unrealircd-plaintext-ipv6-port proxy listen=tcp:[${PUBLIC_IPV6}]:6660-6669,7000 connect=tcp:[${CONTAINER_IPV6}]:6667 nat=true
    incus config device add unrealircd-container unrealircd-tls-ipv6-port       proxy listen=tcp:[${PUBLIC_IPV6}]:6697,9000      connect=tcp:[${CONTAINER_IPV6}]:6697 nat=true
}


container_update() {
    # Backup the existing configuration before nuking the old instance
    incus exec unrealircd-container -- sudo -u agent mkdir -p $BACKUP
    incus exec unrealircd-container -- sudo -u agent cp $UNREAL/conf/*.conf    $BACKUP
    incus exec unrealircd-container -- sudo -u agent cp $UNREAL/conf/tls/*.pem $BACKUP
    incus exec unrealircd-container -- sudo -u agent cp $UNREAL/data/*.db      $BACKUP
    incus exec unrealircd-container -- sudo -u agent bash -c "$UNREAL/unrealircd stop && rm -rf $UNREAL"

    # Download & compile the new source code
    incus exec unrealircd-container -- sudo -u agent git clone --depth 1 https://github.com/supernets/unrealircd.git $SOURCE
    incus exec unrealircd-container -- sudo -u agent bash -c "cd $SOURCE && echo -e "\n" | ./Config -nointro && make && make install"

    # Restore the configuration & start the new instance
    incus exec unrealircd-container -- sudo -u agent rm -rf $UNREAL/conf/*.conf
    incus exec unrealircd-container -- sudo -u agent mv $BACKUP/*.conf $UNREAL/conf
    incus exec unrealircd-container -- sudo -u agent mv $BACKUP/*.pem  $UNREAL/conf/tls
    incus exec unrealircd-container -- sudo -u agent mv $BACKUP/*.db   $UNREAL/data
    incus exec unrealircd-container -- sudo -u agent rm -rf $BACKUP
    incus exec unrealircd-container -- sudo -u agent $UNREAL/unrealircd start
}


deploy_leaf() {
    read -p "Link Name: " NAME
    SID=$(cat /dev/urandom | tr -dc '0-9' | fold -w 256 | head -n 1 | head --bytes 1)$(cat /dev/urandom | tr -dc 'A-Z0-9' | fold -w 2 | head -n 1)
    read -p "Remote Include: " REMOTE
    for item in badwords except ircd modules opers snomasks spamfilter; do echo "include \"$REMOTE/$item.conf\";" >> $UNREAL/conf/unrealircd.conf; done
    echo "me { name \"$NAME.supernets.org\"; info \"SuperNETs IRC Network\"; sid $SID; }" >> $UNREAL/conf/unrealircd.conf
    $UNREAL/unrealircd start &

    # Get the SPKIFP & IP addresses for adding to the hub links.conf
    SPKIFP=$($UNREAL/unrealircd spkifp | tail -n2 | head -1)
    IP4=$(curl -4 icanhazip.com)
    echo "SPKIFP: $SPKIFP" && echo "IPv4: $IP4"
}


get_latest_release() {
    curl -s https://www.unrealircd.org/downloads/list.json | jq .[].[].version
}


provision() {
    # Define a random SSH port
    SSH_PORT=$((RANDOM % 16501 + 49000))

    # Set password
    passwd

    # Set SSH config
    echo -e "AuthenticationMethods publickey\nBanner /etc/issue\nClientAliveInterval 0\nDisableForwarding yes\nPermitRootLogin no\nPort $SSH_PORT\nPrintLastLog no" > /etc/ssh/sshd_config && systemctl restart ssh

    # Limit journal size
    echo -e "[Journal]\nStorage=volatile\nSplitMode=none\nRuntimeMaxUse=500K" > /etc/systemd/journald.conf && systemctl restart systemd-journald

    # Set hostname
    echo "$SERVER_NAME" > /etc/hostname

    # Create a 1GB swap file
    fallocate -l 1G /swapfile && chmod 600 /swapfile && mkswap /swapfile && swapon /swapfile && echo "/swapfile none swap sw 0 0" >> /etc/fstab

    # Update & install packages
    apt update -y && apt upgrade -y
    apt install -y curl git htop incus incus-tools net-tools unattended-upgrades wget

    # Create user
    useradd -m -s /bin/bash supernets && passwd supernets && gpasswd -a supernets incus && gpasswd -a supernets incus-admin

    # Set SSH keys
    mkdir -p /root/.ssh && echo "$SSH_KEY" > /root/.ssh/authorized_keys && chmod 700 /root/.ssh && chown -R root /root/.ssh && chmod 400 /root/.ssh/authorized_keys

    # Set bash configs
    echo "export PS1=\"\e[38;5;237m\T\e[0m \e[38;5;196m\u@\h\e[0m \e[38;5;226m\w\e[0m : \"" > /root/.bashrc
    echo "export PS1=\"\e[38;5;237m\T\e[0m \e[38;5;51m\u@\h\e[0m \e[38;5;129m\w\e[0m : \""  > /home/supernets/.bashrc

    # Wipe issue
    >/etc/issue

    # Prevent logging
    [ -f /root/.bash_history ]           && rm /root/.bash_history
    [ -f /home/supernets/.bash_history ] && rm /home/supernets/.bash_history
    ln -s /dev/null /root/.bash_history  && ln -s /dev/null /home/supernets/.bash_history
    >/var/lastlog && chattr +i /var/lastlog

    # Initialize incus
    incus admin init

}

setup_tor() {
    apt install -y tor
    {
        echo "HiddenServiceDir /var/lib/tor/ircd"
        echo "HiddenServicePort 6667 unix:/etc/tor/unrealircd/tor_ircd.socket"
        echo "HiddenServicePort 6697 unix:/etc/tor/unrealircd/tor_tls_ircd.socket"
    } > /etc/tor/torrc

    mkdir /etc/tor/unrealircd
    chown unrealircd:debian-tor /etc/tor/unrealircd
    chmod 750 /etc/tor/unrealircd

    systemctl restart tor.service && systemctl enable tor.service
    ONION_HOST=$(cat /var/lib/tor/ircd/hostname)
    echo "MapAddress irc.supernets.org $ONION_HOST" >> /etc/tor/torrc
}


setup_firewall() {
    # Install iptables-persistent & netfilter-persistent
    apt install -y iptables-persistent netfilter-persistent wireguard wireguard-tools

    # Kernel hardening settings
    mkdir -p /etc/sysctl.d
    {
        echo "net.ipv4.conf.all.accept_source_route = 0"
        echo "net.ipv6.conf.all.accept_source_route = 0"
        echo "net.ipv4.conf.all.rp_filter = 1"
        echo "net.ipv4.conf.default.rp_filter = 1"
        echo "net.ipv4.conf.all.accept_redirects = 0"
        echo "net.ipv6.conf.all.accept_redirects = 0"
        echo "net.ipv4.conf.default.accept_redirects = 0"
        echo "net.ipv6.conf.default.accept_redirects = 0"
        echo "net.ipv4.conf.all.log_martians = 1"
        echo "kernel.randomize_va_space = 2"
        echo "fs.suid_dumpable = 0"
    } > /etc/sysctl.d/99-custom-hardening.conf

    # Apply hardening settings
    sysctl -p /etc/sysctl.d/99-custom-hardening.conf

    # Default chain policies
    iptables -P INPUT   DROP
    iptables -P FORWARD DROP
    iptables -P OUTPUT  ACCEPT

    # Common Firewall rules
    iptables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
    iptables -A INPUT -i lo -j ACCEPT

    # Allow container NAT
    iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
    iptables -A FORWARD -i incusbr0 -o eth0 -j ACCEPT
    iptables -A FORWARD -i eth0 -o incusbr0 -m state --state RELATED,ESTABLISHED -j ACCEPT

    # Allow container DHCP
    iptables -I INPUT   -i incusbr0 -p udp --dport 67:68 -j ACCEPT
    iptables -I FORWARD -i incusbr0 -p udp --dport 67:68 -j ACCEPT

    # Allow container DNS
    iptables -A INPUT   -i incusbr0         -p udp --dport 53 -j ACCEPT
    iptables -A INPUT   -i incusbr0         -p tcp --dport 53 -j ACCEPT
    iptables -A FORWARD -i incusbr0 -o eth0 -p udp --dport 53 -j ACCEPT
    iptables -A FORWARD -i incusbr0 -o eth0 -p tcp --dport 53 -j ACCEPT

    # Allow SSH access
    iptables -A INPUT -p tcp --dport $SSH_PORT -j ACCEPT

    # Allow plaintext IRC access
    iptables -A INPUT -p tcp --dport 6660:6669 -j ACCEPT
    iptables -A INPUT -p tcp --dport 7000      -j ACCEPT

    # Allow TLS IRC access
    iptables -A INPUT -p tcp --dport 6697,9000 -j ACCEPT

    # Allow IRC S2S access
    iptables -A INPUT -p tcp --dport $S2S_PORT -j ACCEPT

    # IPv6 Default chain policies
    ip6tables -P INPUT   DROP
    ip6tables -P FORWARD DROP
    ip6tables -P OUTPUT  ACCEPT
    
    # IPv6 Common Firewall rules
    ip6tables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
    ip6tables -A INPUT -i lo -j ACCEPT
    
    # IPv6 Allow container forwarding (no NAT needed for IPv6)
    ip6tables -A FORWARD -i incusbr0 -o eth0 -j ACCEPT
    ip6tables -A FORWARD -i eth0     -o incusbr0 -m state --state RELATED,ESTABLISHED -j ACCEPT
    
    # IPv6 Allow container DHCPv6
    ip6tables -I INPUT   -i incusbr0 -p udp --dport 546:547 -j ACCEPT
    ip6tables -I FORWARD -i incusbr0 -p udp --dport 546:547 -j ACCEPT
    
    # IPv6 Allow container DNS
    ip6tables -A INPUT   -i incusbr0         -p udp --dport 53 -j ACCEPT
    ip6tables -A INPUT   -i incusbr0         -p tcp --dport 53 -j ACCEPT
    ip6tables -A FORWARD -i incusbr0 -o eth0 -p udp --dport 53 -j ACCEPT
    ip6tables -A FORWARD -i incusbr0 -o eth0 -p tcp --dport 53 -j ACCEPT
        
    # IPv6 Allow plaintext IRC access
    ip6tables -A INPUT -p tcp --dport 6660:6669 -j ACCEPT
    ip6tables -A INPUT -p tcp --dport 7000      -j ACCEPT
    
    # IPv6 Allow TLS IRC access
    ip6tables -A INPUT -p tcp --dport 6697,9000 -j ACCEPT
    
    # Save rules
    netfilter-persistent save
}


update_source() {
    # Download and extract the source code
    wget -O $SOURCE.tar.gz https://www.unrealircd.org/downloads/unrealircd-latest.tar.gz
    tar -xvf $SOURCE.tar.gz --one-top-level --strip-components=1 && rm $SOURCE.tar.gz

    # Tweak the source code
    sed -i 's/NICKNAMEHISTORYLENGTH="2000"/NICKNAMEHISTORYLENGTH="100"/g' $SOURCE/Config
    sed -i 's/REMOTEINC=""/REMOTEINC="1"/g' $SOURCE/Config
    sed -i 's/if \[ "\$QUICK" != "0" \] ; then/if [ "$QUICK" != "fuckoff" ] ; then/' $SOURCE/Config
    sed -i 's|BASEPATH="$HOME/unrealircd"|BASEPATH="/opt/ircd"|' $SOURCE/Config
    sed -i 's/*.default.conf/*.conf/g'  $SOURCE/Makefile.in
    sed -i 's/*.optional.conf/*.motd/g' $SOURCE/Makefile.in
    sed -i '/modules.sources.list/,/doc\/conf\/example/d' $SOURCE/Makefile.in
    sed -i 's/sendnotice(target, "\*\*\* You were forced to join %s", jbuf);//g' $SOURCE/src/modules/sajoin.c
    sed -i 's/0.organizationName_default      = IRC geeks/0.organizationName_default      = SuperNETs/g'         $SOURCE/extras/tls.cnf
    sed -i 's/1.commonName_value              = localhost/1.commonName_value              = irc.supernets.org/g' $SOURCE/extras/tls.cnf
    sed -i 's;//#undef FAKELAG_CONFIGURABLE;#define FAKELAG_CONFIGURABLE;g' $SOURCE/include/config.h

    # Cleanup unnecessary files & directories
    for item in ".github" ".gitignore" "BSD" "CONTRIBUTING.md" "LICENSE" "Makefile.windows" "README.md" "SECURITY.md" "doc/conf/*.*" "doc/conf/aliases" "doc/conf/examples" "doc/conf/help"; do
        rm -rf $SOURCE/$item
    done

    # Download the SuperNETsconfiguration files
    for item in "badwords.conf" "except.conf" "ircd.motd" "ircd.rules" "modules.conf" "opers.conf" "remote.motd" "snomasks.conf" "spamfilter.conf" "unrealircd.hub.conf" "unrealircd.link.conf" "unrealircd.remote.conf"; do
        wget -O $SOURCE/doc/conf/$item https://raw.githubusercontent.com/supernets/unrealircd/master/doc/conf/$item
    done
}


