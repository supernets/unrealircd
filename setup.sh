#!/bin/bash
# SuperNETS UnrealIRCd - Developed by acidvegas (https://github.com/supernets/unrealircd)
# unrealircd/setup.sh

set -xev

# Load environment variables
if [ -f .env ]; then
    source .env
else
    echo "Error: .env file not found" && exit 1
fi

# Copy database files from container to assets directory if they exist
if docker exec ircd sh -c "ls /opt/ircd/data/*.db 2>/dev/null"; then
    docker cp ircd:/opt/ircd/data/*.db assets/
fi    

# Check if all of the assets exist
[ ! -d assets          ] && echo "error: assets directory not found" && exit 1
[ ! -f assets/tls.crt  ] && echo "error: tls.crt file not found"     && exit 1
[ ! -f assets/tls.key  ] && echo "error: tls.key file not found"     && exit 1
[ -z $(ls assets/*.db) ] && echo "warning: no database files found"

# Remove existing container if it exists
docker rm -f ircd 2>/dev/null || true

# Cleanup docker volumes
docker system prune -af --volumes

# Build the Docker image
docker build -t ircd .

# Run the Docker container with proper settings and environment variables
docker run -d \
    --name ircd \
    --restart always \
    --hostname $HOSTNAME \
    -p 6660-6669:6660-6669 \ 
    -p 6697:6697 \
    -p 7000:7000 \
    -p 9000:9000 \
    -p ${LEAF_PORT}:${LEAF_PORT} \
    ircd