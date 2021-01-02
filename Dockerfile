ARG OSX_SDK=MacOSX10.10.sdk
ARG OSX_SDK_SUM=631b4144c6bf75bf7a4d480d685a9b5bda10ee8d03dbf0db829391e2ef858789

ARG OSX_CROSS_COMMIT=a9317c18a3a457ca0a657f08cc4d0d43c6cf8953




# Clear cache: 
# docker builder prune

# Clear unused images
# docker image prune
# docker image prune -a

# Build: 
# DOCKER_BUILDKIT=1 docker build -o output .

# Build an Ubuntu 16.04 xenial x86_64 container
FROM ubuntu:xenial as compile_xenial
ENV DEBIAN_FRONTEND="noninteractive" TZ="Europe/London"
RUN apt-get update -y && apt-get install -y --no-install-recommends libc6-dev libc6-dev-i386 gcc gcc-multilib gcc-mingw-w64-i686 gcc-mingw-w64-x86-64
COPY . /projectroot/
RUN cd /projectroot && \ 
    gcc convert.c -s -o 60000-cheatcode-generator-linux-x86_64.elf && \
    gcc -m32 convert.c -s -o 60000-cheatcode-generator-linux-i386.elf -march=i386 && \
    gcc -m32 convert.c -s -o 60000-cheatcode-generator-linux-i686.elf -march=i686 && \
    gcc -mx32 convert.c -s -o 60000-cheatcode-generator-linux-x32.elf

RUN cd /projectroot && \
    i686-w64-mingw32-gcc convert.c -lws2_32 -DWINDOWS=1 -s -o 60000-cheatcode-generator-win32.exe && \
    x86_64-w64-mingw32-gcc convert.c -lws2_32 -DWINDOWS=1 -s -o 60000-cheatcode-generator-win64.exe



FROM debian:buster AS osx-cross
ENV OSX_CROSS_PATH=/osxcross
ARG OSX_CROSS_COMMIT
ARG OSX_SDK
ARG OSX_SDK_SUM
WORKDIR "${OSX_CROSS_PATH}"
ARG DEBIAN_FRONTEND=noninteractive
# Dependencies for https://github.com/tpoechtrager/osxcross:
RUN apt-get update -qq && apt-get install -y -q --no-install-recommends \
    clang file llvm patch xz-utils make && \
    apt-get install -y -q git && \
    rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/tpoechtrager/osxcross.git . \
&& git checkout -q "${OSX_CROSS_COMMIT}" \
&& rm -rf ./.git
ADD https://s3.dockerproject.org/darwin/v2/${OSX_SDK}.tar.xz "${OSX_CROSS_PATH}/tarballs/${OSX_SDK}.tar.xz"
RUN echo "${OSX_SDK_SUM}"  "${OSX_CROSS_PATH}/tarballs/${OSX_SDK}.tar.xz" | sha256sum -c -
RUN UNATTENDED=yes OSX_VERSION_MIN=10.10 ./build.sh




FROM debian:buster AS mac-final
RUN apt-get update -qq && apt-get install -y -q clang
ENV OSX_CROSS_PATH=/osxcross
COPY --from=osx-cross "${OSX_CROSS_PATH}/." "${OSX_CROSS_PATH}/"
ENV PATH=${OSX_CROSS_PATH}/target/bin:$PATH



COPY . /projectroot/


ENV PATH=${OSX_CROSS_PATH}/target/bin:$PATH
RUN cd /projectroot && \
    i386-apple-darwin14-clang convert.c -s -o binary.i386 && \
    i386-apple-darwin14-strip binary.i386

RUN cd /projectroot && \
    x86_64-apple-darwin14-clang convert.c -s -o binary.x86_64 && \
    x86_64-apple-darwin14-strip binary.x86_64

RUN cd /projectroot && \
    x86_64-apple-darwin14-lipo -create binary.i386 binary.x86_64 -output 60000-cheatcode-generator-mac

FROM scratch AS export-stage
COPY --from=compile_xenial /projectroot/*.elf /
COPY --from=compile_xenial /projectroot/*.exe /
COPY --from=mac-final /projectroot/60000-cheatcode-generator-mac /

