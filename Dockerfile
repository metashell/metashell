# docker build . -t metashell:0.1 --build-arg J=2          # Build
# docker run -v $PWD:/tmp/devenv -i metashell:0.1          # Run
# docker run -v $PWD:/tmp/devenv -it metashell:0.1 bash    # Enter dev env

FROM debian:11

ARG ROOT_DIR=/tmp/devenv
ARG J

WORKDIR $ROOT_DIR/build

RUN apt-get update && apt-get install -y locales \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

ENV LANG en_US.utf8
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y --no-install-recommends \
        build-essential \
		git \
        cmake \
		file \
		rpm \
		zip \
		python3 \
		libreadline-dev

ENV J=${J:-1}

CMD mkdir templight ; cd templight \
	&& cmake ../../3rd/templight/llvm \
		-DCMAKE_BUILD_TYPE=Release \
		-DLLVM_ENABLE_TERMINFO=OFF \
		-DLLVM_ENABLE_PROJECTS=clang \
	&& make templight -j $J \
	&& mkdir ../metashell ; cd ../metashell \
	&& cmake ../.. \
		-DCMAKE_BUILD_TYPE=Release \
		-DMETASHELL_NO_DOC_GENERATION=1 \
	&& make -j $J \
	&& cpack -G DEB \
	&& cpack -G RPM
