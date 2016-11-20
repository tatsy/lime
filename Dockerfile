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
## update/upgrade
#
RUN apt-get update -qq
RUN apt-get upgrade -qq

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
## Install Google test
#
RUN git clone https://github.com/google/googletest.git
RUN \
  cd googletest && \
  mkdir build && cd build && \
  cmake -D gtest_disable_pthreads=ON .. && \
  make && make install

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
  git submodule update --init --recursive
RUN \
  cd lime && \
  cmake -D LIME_BUILD_TESTS=ON -D GTEST_ROOT=/usr/local . && \
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
