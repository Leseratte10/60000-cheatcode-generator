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


FROM scratch AS export-stage
COPY --from=compile_xenial /projectroot/*.elf /
COPY --from=compile_xenial /projectroot/*.exe /
