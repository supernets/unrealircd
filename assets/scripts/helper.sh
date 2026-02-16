#!/bin/bash
# SuperNETs UnrealIRCd source update script - Developed by acidvegas (https://github.com/supernets/unrealircd)
# unrealircd/assets/helper.sh

get_latest_release() {
	curl -s https://www.unrealircd.org/downloads/list.json | jq -r 'to_entries[].value.Stable.version'
}


update_source() {
	# Download and extract the source code
	wget -O $SOURCE.tar.gz https://www.unrealircd.org/downloads/unrealircd-latest.tar.gz
	tar -xvf $SOURCE.tar.gz --one-top-level --strip-components=1 && rm $SOURCE.tar.gz

	# Tweak the source code
	sed -i 's/NICKNAMEHISTORYLENGTH="2000"/NICKNAMEHISTORYLENGTH="100"/g'            $SOURCE/Config
	sed -i 's/REMOTEINC=""/REMOTEINC="1"/g'                                          $SOURCE/Config
	sed -i 's/if \[ "\$QUICK" != "0" \] ; then/if [ "$QUICK" != "fuckoff" ] ; then/' $SOURCE/Config
	sed -i 's|BASEPATH="$HOME/unrealircd"|BASEPATH="/opt/unrealircd"|'               $SOURCE/Config
	sed -i 's/*.default.conf/*.conf/g'                                               $SOURCE/Makefile.in
	sed -i 's/*.optional.conf/*.motd/g'                                              $SOURCE/Makefile.in
	sed -i '/modules.sources.list/,/doc\/conf\/example/d'                            $SOURCE/Makefile.in
	sed -i 's/sendnotice(target, "\*\*\* You were forced to join %s", jbuf);//g'     $SOURCE/src/modules/sajoin.c
	sed -i 's;//#undef FAKELAG_CONFIGURABLE;#define FAKELAG_CONFIGURABLE;g'          $SOURCE/include/config.h

	# Cleanup unnecessary files & directories
	for item in ".github" ".gitignore" "BSD" "CONTRIBUTING.md" "LICENSE" "Makefile.windows" "README.md" "SECURITY.md" "doc/conf/*.*" "doc/conf/aliases" "doc/conf/examples" "doc/conf/help"; do
		rm -rf $SOURCE/$item
	done

	# Download the SuperNETsconfiguration files
	for item in "badwords.conf" "except.conf" "ircd.motd" "ircd.rules" "modules.conf" "opers.conf" "remote.motd" "snomasks.conf" "spamfilter.conf" "unrealircd.hub.conf" "unrealircd.link.conf" "unrealircd.remote.conf"; do
		wget -O $SOURCE/doc/conf/$item https://raw.githubusercontent.com/supernets/unrealircd/master/doc/conf/$item
	done
}


incus2docker() {
	incus file pull unrealircd-container/opt/ircd/conf/tls/irc.key      conf/tls/
	incus file pull unrealircd-container/opt/ircd/conf/tls/irc.crt      conf/tls/
	incus file pull unrealircd-container/opt/ircd/conf/tls/server.*.pem conf/tls/
	incus file pull unrealircd-container/opt/ircd/conf/unrealircd.conf  conf/unrealircd.conf
	incus file pull unrealircd-container/opt/ircd/data/*.db             data/
	incus file pull unrealircd-container/opt/ircd/logs/*.log            logs/
	incus stop unrealircd-container
	../setup.sh
}


deploy() {
	if [ -z "$1" ]; then
		echo "Usage: deploy [all|hostname]"
		exit 1
	elif [ "$1" == "all" ]; then
		ansible-playbook -i inventory.ini deploy.yml
	else
		ansible-playbook -i inventory.ini deploy.yml --limit $1
	fi
}