# docker build . -t metashell:0.1 --build-arg J=2          # Build
# docker run -v $PWD:/tmp/devenv -i metashell:0.1          # Run
# docker run -v $PWD:/tmp/devenv -it metashell:0.1 bash    # Enter dev env

FROM debian:11

ARG ROOT_DIR=/tmp/devenv
ARG J

WORKDIR $ROOT_DIR

RUN apt-get update && apt-get install -y locales \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

ENV LANG en_US.utf8
ENV DEBIAN_FRONTEND=noninteractive
ENV NO_SUDO=1

COPY install_build_dependencies.sh .
COPY tools tools
RUN ./install_build_dependencies.sh

ENV BUILD_THREADS=${J:-1}

CMD ./build.sh
