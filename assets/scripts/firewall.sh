#!/bin/bash
# SuperNETS UnrealIRCd - Developed by acidvegas (https://git.supernets.org/supernets/unrealircd)
# unrealircd/assets/firewall.sh

# Set xtrace, exit on error, & verbose mode
set -xev

# Get the SSH port
SSH_PORT=$(grep -oP '^Port \K\d+' /etc/ssh/sshd_config)

# Flush existing rules
iptables  -F && iptables  -X
ip6tables -F && ip6tables -X

# Default policies
iptables  -P INPUT DROP
iptables  -P FORWARD DROP
iptables  -P OUTPUT ACCEPT
ip6tables -P INPUT DROP
ip6tables -P FORWARD DROP
ip6tables -P OUTPUT ACCEPT

# Loopback
iptables  -A INPUT -i lo -j ACCEPT
ip6tables -A INPUT -i lo -j ACCEPT

# Established connections
iptables  -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
ip6tables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT

# SSH
iptables  -A INPUT -p tcp --dport $SSH_PORT -j ACCEPT
ip6tables -A INPUT -p tcp --dport $SSH_PORT -j ACCEPT

# IRC
for port in 6667 6697 7000; do
	iptables  -A INPUT -p tcp --dport $port -j ACCEPT
	ip6tables -A INPUT -p tcp --dport $port -j ACCEPT
done

# Save rules
if command -v netfilter-persistent >/dev/null 2>&1; then
	netfilter-persistent save
elif command -v iptables-save >/dev/null 2>&1; then
	iptables-save  > /etc/iptables/rules.v4
	ip6tables-save > /etc/iptables/rules.v6
fi