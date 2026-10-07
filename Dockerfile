# SuperNETS UnrealIRCd - Developed by acidvegas (https://git.supernets.org/supernets/unrealircd)
# unrealircd/Dockerfile

# Use a bare minimal Alpine Linux base image
FROM alpine:3.24

# Install build dependencies
RUN apk add --no-cache build-base pkgconf openssl openssl-dev pcre2-dev argon2-dev libsodium-dev c-ares-dev curl-dev ca-certificates libcap

# Copy the UnrealIRCd source code to the container
COPY src /tmp/unrealircd

# Create a user and directory for UnrealIRCd
RUN addgroup -g 1000 unrealircd && adduser -D -u 1000 -G unrealircd -s /bin/sh unrealircd && mkdir -p /opt/unrealircd && chown -R unrealircd:unrealircd /tmp/unrealircd /opt/unrealircd

# Switch to the user
USER unrealircd

# User-Agent for remote includes
ARG SUPERNETS_USER_AGENT

# Configure UnrealIRCd
RUN test -n "$SUPERNETS_USER_AGENT" \
    && cd /tmp/unrealircd \
    && sed -i "s|^#define SUPERNETS_USER_AGENT .*|#define SUPERNETS_USER_AGENT \"$SUPERNETS_USER_AGENT\"|" include/config.h \
    && echo "" | ./Config -nointro \
    && make -j$(nproc) \
    && make install \
    && rm -rf /tmp/unrealircd

# Grant the bind capability for privileged ports (ircd still runs non-root)
USER root
RUN setcap cap_net_bind_service=+ep /opt/unrealircd/bin/unrealircd
USER unrealircd

# Expose the required ports
EXPOSE 1 6667 6697 7000

# Set the entrypoint to start UnrealIRCd
ENTRYPOINT ["/opt/unrealircd/bin/unrealircd", "-F"]
