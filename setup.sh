#!/bin/bash
# SuperNETs UnrealIRCd Docker Setup - Developed by acidvegas (https://github.com/supernets/unrealircd)

# Load environment variables
[ -f assets/.env ] && source assets/.env || { echo "Error: assets/.env file not found"; exit 1; }

# Set xtrace, exit on error, & verbose mode (after loading environment variables)
set -xev

# Remove existing docker container if it exists
docker rm -f unrealircd 2>/dev/null || true

# Build the Docker image
docker build -t unrealircd .

# Setup persistent directories
mkdir -p $(pwd)/assets/conf/tls $(pwd)/assets/data $(pwd)/assets/logs

# Generate self-signed TLS certs if they don't exist
if [ ! -f $(pwd)/assets/conf/tls/server.cert.pem ]; then
	openssl ecparam -out $(pwd)/assets/conf/tls/server.key.pem -name secp384r1 -genkey
	openssl req -new -x509 -days 3650 -sha256 -nodes \
		-key $(pwd)/assets/conf/tls/server.key.pem \
		-out $(pwd)/assets/conf/tls/server.cert.pem \
		-subj "/C=US/ST=New York/O=SuperNETs/CN=irc.supernets.org"
	chmod 600 $(pwd)/assets/conf/tls/server.key.pem $(pwd)/assets/conf/tls/server.cert.pem
fi

# Copy the curl-ca-bundle.crt to the assets/conf/tls directory
cp $(pwd)/src/doc/conf/tls/curl-ca-bundle.crt $(pwd)/assets/conf/tls/

# Generate unrealircd.conf for leaf
if [ ! -f $(pwd)/assets/conf/unrealircd.conf ]; then
	for item in badwords except ircd modules opers snomasks spamfilter; do echo "include \"$REMOTE_INCLUDE/$item.conf\";" >> $(pwd)/assets/conf/unrealircd.conf; done
	echo "me { name \"$SERVER_NAME.supernets.org\"; info \"SuperNETs IRC Network\"; sid $SID; }" >> $(pwd)/assets/conf/unrealircd.conf
fi

# Run the Docker container
docker run -d --name unrealircd --restart unless-stopped --network host \
	-p 6667:6667 \
	-p 6697:6697 \
	-p 7000:7000 \
	-v $(pwd)/assets/conf:/opt/unrealircd/conf \
	-v $(pwd)/assets/data:/opt/unrealircd/data \
	-v $(pwd)/assets/logs:/opt/unrealircd/logs \
	unrealircd

# Output SPKIFP & IP
SPKIFP=$(docker exec -w /opt/unrealircd unrealircd /opt/unrealircd/unrealircd spkifp | tail -n2 | head -1)
IP4=$(curl -4 -s https://maxmind.supernets.org | jq -rc .ip)
printf "\nSPKIFP: $SPKIFP\nIPv4:   $IP4\n"

echo "link $SERVER_NAME.supernets.org {
	incoming { mask $IP4; }
	outgoing {
		bind-ip *;
		hostname $IP4;
		port 9000;
		options { tls; }
	}
	$SPKIFP
	class servers;
}"