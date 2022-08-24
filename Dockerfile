FROM debian:11-slim as builder

RUN set -x \
    && apt-get update \
    && apt-get install -y make file gcc

COPY . /tmp/mustpl

WORKDIR /tmp/mustpl

ARG BUILD_STATIC="no"
ARG VERSION="0.0.0-undefined"

# compile
RUN make static="$BUILD_STATIC" version="$VERSION"

# exit with error code 1 if the executable is dynamic (not static)
RUN if [ "${BUILD_STATIC}" != "no" ]; then ldd ./mustpl && exit 1 || true; fi

# tests running is not required, but strongly recommended
RUN make test

# print out some info about the binary file
RUN set -x \
    && ./mustpl --version \
    && ./mustpl --help \
    && ls -lh ./mustpl \
    && file ./mustpl

WORKDIR /tmp/rootfs

# prepare the rootfs for scratch
RUN set -x \
    && mkdir -p ./bin ./etc \
    && mv /tmp/mustpl/mustpl ./bin/mustpl \
    && echo 'nobody:x:65534:65534::/nonexistent:/sbin/nologin' > ./etc/passwd \
    && echo 'nogroup:x:65534:' > ./etc/group \
    && if [ "${BUILD_STATIC}" = "no" ]; then \
        for library in $(ldd ./bin/mustpl | cut -d '>' -f 2 | awk '{print $1}'); do \
	        cp --verbose --parents "${library}" . \
        ;done \
    ;fi

# use empty filesystem
FROM scratch as runtime

ARG VERSION="0.0.0-undefined"

LABEL \
    # Docs: <https://github.com/opencontainers/image-spec/blob/master/annotations.md>
    org.opencontainers.image.title="mustpl" \
    org.opencontainers.image.description="Logic-less CLI templating tool - mustpl" \
    org.opencontainers.image.url="https://github.com/tarampampam/mustpl" \
    org.opencontainers.image.source="https://github.com/tarampampam/mustpl" \
    org.opencontainers.image.vendor="tarampampam" \
    org.opencontainers.version="$VERSION" \
    org.opencontainers.image.licenses="MIT"

# use an unprivileged user
USER 65534:65534

# import from builder
COPY --from=builder /tmp/rootfs /

ENTRYPOINT ["/bin/mustpl"]
