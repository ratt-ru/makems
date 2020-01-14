FROM kernsuite/base:5

RUN docker-apt-install libblitz0-dev python-dev libblas-dev liblapack-dev libqdbm-dev wcslib-dev \
 libfftw3-dev python-numpy libcfitsio-dev libboost-all-dev libboost-system-dev cmake g++ wget gfortran \
 libncurses5-dev libsofa1-dev bison libbison-dev flex libreadline6-dev python-pip

# casacore wheels no longer work and we need python 3 support, so build from source

#####################################################################
## BUILD CASACORE FROM SOURCE
#####################################################################
RUN mkdir /src
WORKDIR /src
RUN wget https://github.com/casacore/casacore/archive/v3.2.1.tar.gz
RUN tar xvf v3.2.1.tar.gz
RUN mkdir casacore-3.2.1/build
WORKDIR /src/casacore-3.2.1/build
RUN cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ../
RUN make -j 4
RUN make install
RUN ldconfig
#RUN pip install -U --user --force-reinstall --install-option="--prefix=/usr"  pip setuptools wheel
WORKDIR /src
RUN wget https://github.com/casacore/python-casacore/archive/v3.2.0.tar.gz
RUN tar xvf v3.2.0.tar.gz
WORKDIR /src/python-casacore-3.2.0
RUN pip install .
WORKDIR /
RUN python -c "from pyrap.tables import table as tbl"

#####################################################################
## Get CASACORE ephem data
#####################################################################
RUN mkdir -p /usr/share/casacore/data/
WORKDIR /usr/share/casacore/data/
RUN docker-apt-install rsync
RUN rsync -avz rsync://casa-rsync.nrao.edu/casa-data .

RUN mkdir -p /src/
ENV BUILD /src
ADD . /src/makems
RUN mkdir -p $BUILD/makems/LOFAR/build/gnu_opt
WORKDIR $BUILD/makems/LOFAR/build/gnu_opt
RUN cmake -DCMAKE_MODULE_PATH:PATH=$BUILD/makems/LOFAR/CMake \
-DUSE_LOG4CPLUS=OFF -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release ../..
RUN make -j 16
RUN make install

ENV PATH=/src/makems/LOFAR/build/gnu_opt/CEP/MS/src:${PATH}
WORKDIR $BUILD/makems/test
RUN makems WSRT_makems.cfg
ENTRYPOINT makems
