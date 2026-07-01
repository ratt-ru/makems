# installation inside a virtual environment

## initial setup

### on Adastra

```sh
module load gsl cmake
export CC=gcc
export CXX=g++
```

## load and create the virtual environment

```sh
if test -z "${VIRTUAL_ENV:-}"; then
    export UV_CACHE_DIR=$PWD/../uv-cache
    # create venv if not exists yet
    if ! test -d venv; then
        uv venv -p 3.12 venv
    fi
    # load venv
    source venv/bin/activate
    uv pip install numpy==1.26.4 # for boost 1.74.0
fi

test -n "$VIRTUAL_ENV" || ( echo "ERROR: load the venv first"; exit 1)
```

## install casacore_data

```sh
if ! test -d casacore_data; then
    (
    mkdir -p casacore_data
    cd casacore_data
    curl -O -L https://www.astron.nl/iers/WSRT_Measures.ztar
    tar -zxvf WSRT_Measures.ztar
    )
fi
export CASACORE_DATA=$PWD/casacore_data
```

## setup shell variables

```sh
export CFLAGS="-I$(python -c "import numpy; print(numpy.get_include())")"
python_version=$(python -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')")
python_version_long=$(python -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}')")
export CPLUS_INCLUDE_PATH="${CPLUS_INCLUDE_PATH:+CPLUS_INCLUDE_PATH:}$HOME/.local/share/uv/python/cpython-${python_version_long}-linux-x86_64-gnu/include/python${python_version}"
export CPLUS_INCLUDE_PATH="$VIRTUAL_ENV/include:$CPLUS_INCLUDE_PATH"

mkdir -p sources
cd sources
```

## install boost

```sh
(
 curl -O -L -C- https://archives.boost.io/release/1.74.0/source/boost_1_74_0.tar.gz
 if ! test -d boost_1_74_0; then
 tar -zxvf boost_1_74_0.tar.gz 
 ( cd boost_1_74_0
   patch -p1 -i ../../patches/boost-with-python-3.10.patch
   patch -p1 -i ../../patches/boost-1.73-py310.txt
   patch -p1 -i ../../patches/python_jam.patch
 )
 fi
 cd boost_1_74_0/
 ./bootstrap.sh --prefix=$VIRTUAL_ENV --with-libraries=all --with-python=$(command -v python)  variant=release
 ./b2 variant=release threading=multi link=shared runtime-link=shared install
 )
```

## install cfitsio 

```sh
(
 git clone https://github.com/healpy/cfitsio.git || true
 cd cfitsio/
 ./configure --prefix=$VIRTUAL_ENV
 make -j $(nproc)
 make install
 )
```

## install wcslib 

```sh
(
 curl -O -L -C- https://ftp.eso.org/pub/dfs/pipelines/libraries/wcslib/wcslib-8.4.tar.bz2
 if ! test -d wcslib-8.4; then
 tar xvf wcslib-8.4.tar.bz2
 fi
 cd wcslib-8.4
 ./configure --prefix=$VIRTUAL_ENV
 make -j $(nproc)
 make install
 )
```
    
## install fftw3

```sh
(
 curl -O -L -C- https://fftw.org/fftw-3.3.10.tar.gz
 if ! test -d fftw-3.3.10; then
 tar xvf fftw-3.3.10.tar.gz
 fi
 cd fftw-3.3.10
 CFLAGS=-fPIC ./configure --prefix=$VIRTUAL_ENV --enable-threads
 make -j $(nproc)
 make install

 CFLAGS=-fPIC ./configure --prefix=$VIRTUAL_ENV --enable-float --enable-threads
 make -j $(nproc)
 make install
 )
```

## install openblas

```sh
(
 git clone -b v0.3.30 https://github.com/OpenMathLib/OpenBLAS || true
 cd OpenBLAS/
 make -j $(nproc)
 make install PREFIX=$VIRTUAL_ENV
 )
```

## install casacore

```sh
(
 git clone https://github.com/casacore/casacore.git || true
 cd casacore/
 git checkout v3.7.1
 patch -p1 -i ../../patches/fix-datatype-constexpr.patch || true
 cmake -S casacore -B casacore-build -DCMAKE_INSTALL_PREFIX=$VIRTUAL_ENV -DBUILD_PYTHON=OFF -DBUILD_PYTHON3=ON -DBUILD_TESTING=OFF -DDATA_DIR=$CASACORE_DATA -DBOOST_ROOT=$VIRTUAL_ENV -Dboost_python310_DIR=$VIRTUAL_ENV -DCMAKE_VERBOSE_MAKEFILE=ON
 cmake --build casacore-build -j$(nproc)
 cmake --install casacore-build
 )
```

## install makems

```sh
cmake -S LOFAR -B build/gnu_opt  -DUSE_LOG4CPLUS=OFF -DBUILD_TESTING=OFF  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_MODULE_PATH:PATH=$PWD/LOFAR/CMake -DCASACORE_ROOT_DIR=$VIRTUAL_ENV -DCMAKE_INSTALL_PREFIX=$VIRTUAL_ENV
cmake --build build/gnu_opt -j$(nproc)
cmake --install build/gnu_opt
```
