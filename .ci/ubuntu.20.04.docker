FROM kernsuite/base:7

RUN apt update
RUN apt install -y libblitz0-dev python3-dev libblas-dev liblapack-dev libqdbm-dev wcslib-dev \
 libfftw3-dev python3-numpy libcfitsio-dev libboost-all-dev libboost-system-dev cmake g++ wget gfortran \
 libncurses5-dev bison libbison-dev flex libreadline6-dev python3-pip rsync casacore-dev casarest casacore-data

RUN mkdir -p /src/
ENV BUILD /src
ADD . /src/makems
RUN mkdir -p $BUILD/makems/LOFAR/build/gnu_opt
WORKDIR $BUILD/makems/LOFAR/build/gnu_opt
RUN cmake -DCMAKE_MODULE_PATH:PATH=$BUILD/makems/LOFAR/CMake \
-DUSE_LOG4CPLUS=OFF -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=/usr ../..
RUN make -j 16
RUN make install

WORKDIR $BUILD/makems/test
RUN makems WSRT_makems.cfg
ENTRYPOINT makems
