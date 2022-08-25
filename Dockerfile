FROM alpine:latest as builder

RUN set -x \
    && apk add --no-cache make file gcc musl-dev

COPY . /tmp/mustpl

WORKDIR /tmp/mustpl

ARG VERSION="undefined"

# compile
RUN make version="$VERSION"

# running tests is not required, but it's strongly recommended
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
    && echo 'nobody:x:10001:10001::/nonexistent:/sbin/nologin' > ./etc/passwd \
    && echo 'nogroup:x:10001:' > ./etc/group

# use empty filesystem
FROM scratch as runtime

ARG VERSION="undefined"

LABEL \
    # Docs: <https://github.com/opencontainers/image-spec/blob/master/annotations.md>
    org.opencontainers.image.title="mustpl" \
    org.opencontainers.image.description="Logic-less CLI templating tool - mustpl" \
    org.opencontainers.image.url="https://github.com/tarampampam/mustpl" \
    org.opencontainers.image.source="https://github.com/tarampampam/mustpl" \
    org.opencontainers.image.vendor="tarampampam" \
    org.opencontainers.version="$VERSION" \
    org.opencontainers.image.licenses="MIT"

# use the unprivileged user
USER 10001:10001

# import from builder
COPY --from=builder /tmp/rootfs /

ENTRYPOINT ["/bin/mustpl"]
