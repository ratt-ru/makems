Building and using the makems tool using Docker
========================

docker build -t makems .

Then run it mounting in a volume where your parset is located to check whether it works

docker run -it --rm -v [datapath]:/data makems /data/parset.cfg

Building the makems tool
========================

Prerequisites: a subset of the MeqTrees stuff (cmake 2.6, blitz, etc.)

The build, in a nutshell:

$ cd $HOME 
$ git clone git@github.com:ska-sa/makems.git
$ cd makems/LOFAR
$ mkdir -p build/gnu_opt
$ cd build/gnu_opt
$ cmake -DCMAKE_MODULE_PATH:PATH=$HOME/makems/LOFAR/CMake \
  -DUSE_LOG4CPLUS=OFF -DBUILD_TESTING=OFF ../..
$ make

The executable is then built as build/gnu_opt/CEP/MS/makems.


Using the makems tool
=====================

makems.pdf describes use of the tool (ignore the parts about building a
distributed MS, as we compile without this capability.) In a nutshell, there is
a makems.cfg file, and an ANTENNA table (referenced from that file.) For an
example, go into makems/doc/examples, and run "makems makems.cfg". This will create
an MS called test.MS_p0.

Also see the makems/doc/makeant tool, and documentation therein.
