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
ENV PYTHON_VERSION @PYTHON_VERSION@
ENV BUILD_STATIC_LIB @BUILD_STATIC_LIB@
ENV PATH /opt/conda/bin:$PATH

#
## update/upgrade
#
RUN apt-get update -q
#RUN apt-get upgrade -q

#
## Install cppcheck, cccc, and doxygen
#
RUN apt-get -qq install cppcheck cccc doxygen

#
## Install Python through Miniconda
#
RUN wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -q -O miniconda3.sh
RUN sh ./miniconda3.sh -b -p /opt/conda
RUN rm ./miniconda3.sh

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
## Download lime source codes
#
RUN git clone --depth 12 -b $BRANCH_NAME https://github.com/tatsy/lime.git #redo
RUN \
  if [ $PULL_REQUEST != "false" ]; then \
    cd lime && \
    git fetch origin +refs/pull/$PULL_REQUEST/merge && \
    git checkout --quiet --force FETCH_HEAD; \
  fi

RUN \
  cd lime && \
  git submodule update --init --recursive

#
## Create conda env and change default env
#
RUN \
  cd lime && \
  sed -i -e "s/python/python=$PYTHON_VERSION/" environment.yml && \
  conda env create -f environment.yml
ENV PATH /opt/conda/envs/lime/bin:$PATH
ENV CONDA_DEFAULT_ENV lime

#
## Build lime
#
RUN \
  cd lime && \
  cmake \
    -D CMAKE_BUILD_TYPE=Release \
    -D LIME_BUILD_TESTS=ON \
    -D LIME_BUILD_EXAMPLES=ON \
    -D LIME_BUILD_STATIC_LIB=$BUILD_STATIC_LIB \
    -D PYTHON_EXECUTABLE=/opt/conda/envs/lime/bin/python \
    -D PYBIND11_PYTHON_VERSION=$PYTHON_VERSION \
    -D LIME_BUILD_PYTHON_MODULE=ON . && \
  cmake --build . -- -j4

#
## Install pylime
#
RUN \
  cd lime && \
  python setup.py install

#
## # of threads used by OpenMP
#
ENV OMP_NUM_THREADS 4

#
## Define working direcotry
#
WORKDIR /root/lime
