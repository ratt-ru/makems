FROM kernsuite/base:4
RUN docker-apt-install \
    casacore-dev \
    casacore-tools \
    casarest \
    libboost-all-dev \
    libreadline6-dev
RUN docker-apt-install \
    build-essential \
    cmake 

RUN mkdir -p /opt/src/
ENV BUILD /opt/src
ADD . /opt/src/makems
RUN mkdir -p $BUILD/makems/LOFAR/build/gnu_opt
WORKDIR $BUILD/makems/LOFAR/build/gnu_opt
RUN cmake -DCMAKE_MODULE_PATH:PATH=$BUILD/makems/LOFAR/CMake \
-DUSE_LOG4CPLUS=OFF -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release ../..
RUN make -j 16
RUN make install

ENV PATH=/opt/src/makems/LOFAR/build/gnu_opt/CEP/MS/src:${PATH}
ENTRYPOINT makems