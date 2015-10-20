#
## Load docker image
#
FROM tatsy/ubuntu-cxx:opencv

#
## Environment variables
#
ENV TERM xterm
ENV BRANCH_NAME @BRANCH_NAME@
ENV PULL_REQUEST @PULL_REQUEST@
ENV CC @C_COMPILER@
ENV CXX @CXX_COMPILER@

#
## Install Gcovr
#
RUN apt-get -qq install python-pip
RUN pip install gcovr

#
## Install cppcheck, cccc, and doxygen
#
RUN apt-get -qq install cppcheck cccc doxygen

#
## Build lime
#
RUN git clone --depth 10 -b $BRANCH_NAME https://github.com/tatsy/lime.git
RUN \
  if [ $PULL_REQUEST != "false" ]; then \
    cd lime && \
    git fetch origin +refs/pull/$PULL_REQUEST/merge && \
    git checkout --quiet --force FETCH_HEAD; \
  fi

RUN \
  cd lime && \
  git submodule update --init --recursive && \
  cmake . && \
  cmake --build .

#
## # of threads used by OpenMP
#
ENV OMP_NUM_THREADS 4

#
## Define working direcotry
#
WORKDIR /root/lime

#
## Run unit tests
#
RUN lcov --directory . --zerocounters
