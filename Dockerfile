# SuperNETS UnrealIRCd - Developed by acidvegas (https://git.supernets.org/supernets/unrealircd)
# unrealircd/Dockerfile

# Use a bare minimal Alpine Linux base image
FROM alpine:latest

# Install build dependencies
RUN apk add --no-cache build-base pkgconf openssl openssl-dev pcre2-dev argon2-dev libsodium-dev c-ares-dev curl-dev ca-certificates

# Copy the UnrealIRCd source code to the container
COPY src /tmp/unrealircd

# Create a user and directory for UnrealIRCd
RUN adduser -D -s /bin/sh unrealircd && mkdir -p /opt/unrealircd && chown -R unrealircd:unrealircd /tmp/unrealircd /opt/unrealircd

# Switch to the user
USER unrealircd

# Configure UnrealIRCd
RUN cd /tmp/unrealircd \
    && echo "" | ./Config -nointro \
    && make -j$(nproc) \
    && make install \
    && rm -rf /tmp/unrealircd

# Expose the required ports
EXPOSE 6667 6697 7000

# Set the entrypoint to start UnrealIRCd
ENTRYPOINT ["/opt/unrealircd/bin/unrealircd", "-F"]