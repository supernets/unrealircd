# SuperNETS UnrealIRCd - Developed by acidvegas (https://github.com/supernets/unrealircd)
# unrealircd/Dockerfile

# Define the source and build directories
ENV SOURCE_DIR="/tmp/ircd"
ENV BUILD_DIR="/opt/ircd"

# Use debian-based image for consistency
FROM debian:bullseye-slim

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    pkg-config \
    gdb \
    libssl-dev \
    libpcre2-dev \
    libargon2-0-dev \
    libsodium-dev \
    libc-ares-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy the .env file
COPY .env /tmp/.env

# Parse .env file and set environment variables
ENV LEAF_NAME=$(grep LEAF_NAME /tmp/.env | cut -d'"' -f2)
ENV LEAF_PORT=$(grep LEAF_PORT /tmp/.env | cut -d'"' -f2)
ENV REMOTE_INCLUDE_URL=$(grep REMOTE_INCLUDE_URL /tmp/.env | cut -d'"' -f2)

# Remove the temporary .env file after parsing
RUN rm /tmp/.env

# Copy source files
COPY ircd ${SOURCE_DIR}

# Enter the source directory
WORKDIR ${SOURCE_DIR}

# Build UnrealIRCd
RUN echo -e "\n" | ./Config -nointro && make && make install

# Switch to the build directory
WORKDIR ${BUILD_DIR}

# Nuke the source directory
RUN rm -rf ${SOURCE_DIR}

# Nuke the default configuration files
RUN find ${BUILD_DIR}/doc/conf/ -maxdepth 1 -type f ! -name 'unrealircd.link.conf' ! -name 'remote.motd' -exec rm -f {} +
RUN mv ${BUILD_DIR}/doc/conf/unrealircd.link.conf ${BUILD_DIR}/doc/conf/unrealircd.conf

# Replace the remote include URL placeholder
RUN sed -i "s|https://USERNAME:PASSWORD@hub.supernets.org:PORT|${REMOTE_INCLUDE_URL}|g" ${BUILD_DIR}/doc/conf/unrealircd.conf

# Replace the server name placeholder
RUN sed -i "s|example.supernets.org|${LEAF_NAME}.supernets.org|g" ${BUILD_DIR}/doc/conf/unrealircd.conf

# Replace the SID placeholder
# Generate a random SID
ENV SID=$(cat /dev/urandom | tr -dc '0-9' | fold -w 256 | head -n 1 | head --bytes 1)$(cat /dev/urandom | tr -dc 'A-Z0-9' | fold -w 2 | head -n 1)
RUN sed -i "s|XXX|${SID}|g" ${BUILD_DIR}/doc/conf/unrealircd.conf

# Copy over the assets
COPY assets/* ${BUILD_DIR}/assets/
RUN  mv       ${BUILD_DIR}/assets/*.db    ${BUILD_DIR}/data/
RUN  mv       ${BUILD_DIR}/assets/tls.crt ${BUILD_DIR}/conf/tls.crt
RUN  mv       ${BUILD_DIR}/assets/tls.key ${BUILD_DIR}/conf/tls.key

# Create ircd user and group
RUN groupadd -r ircd && useradd -r -g ircd ircd

# Set permissions
RUN chown -R ircd:ircd ${BUILD_DIR}

# Expose standard IRC client ports
EXPOSE 6660-6669 7000

# Expose TLS client ports
EXPOSE 6697 9000

# Expose server-only port
EXPOSE ${LEAF_PORT}

# Switch to ircd user
USER ircd

# Set entrypoint
ENTRYPOINT ["/opt/unrealircd/bin/unrealircd", "start"]