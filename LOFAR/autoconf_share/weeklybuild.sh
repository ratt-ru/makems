#!/bin/sh
#
# Weekly LOFAR build on lofar17:
#

export INSTALLDIR=/data/LOFAR/installed/weekly
export BUILDROOT=/home/lofar/weekly

# remove old LOFAR tree
cd $BUILDROOT
rm -rf LOFAR

#check out LOFAR tree
svn co https://svn.astron.nl/LOFAR/trunk LOFAR

#tag new tree with week[year][weeknr]
#cvs tag week`date +%g%W` LOFAR    

cd LOFAR

#Make sure aips++ environment is known
#source /lofarbuild/aips++/prod/aipsinit.sh


# GNU_DEBUG  Build
#
#source $INSTALLDIR/gnu_debug/lofarinit.sh
rm -rf $INSTALLDIR/gnu_debug/*
autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnu_debug" -build "gnu_debug" $(cat lofarconf.in) > $BUILDROOT/`uname -n`-week`date +%g%W`-gnu_debug.log 2>&1


# GNU_DEBUG_VALGRIND Build
#
rm -rf $INSTALLDIR/gnu_valgrind/*
autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnu_debug" -build "gnu_debug" -checktool "/usr/local/bin/valgrind --tool=memcheck --num-callers=50 --leak-check=yes --track-fds=yes --log-file=$INSTALLDIR/gnu_valgrind/[PROGNAME].valgrind" $(cat lofarconf.in) > $BUILDROOT/`uname -n`-week`date +%g%W`-gnu_valgrind.log 2>&1

# GNU_DEBUG_DISTCHECK
#
#autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnu_debug" -build "gnu_debug" -distcheck Appl LCS CEP MAC/{Test/Suite,GCF/GCFCommon,GCF/TM,APL/RTCCommon,APL/PAC/BeamServer,APL/PAC/BS_Protocol,APL/PAC/CAL_Protocol,APL/PAC/CalServer,APL/PIC/RSPDriver,APL/PIC/RSP_Protocol} Demo > $BUILDROOT/`uname -n`-week`date +%g%W`-gnu_distcheck.log 2>&1

# GNU_OPT Build
#
#source $INSTALLDIR/gnu_opt/lofarinit.sh
rm -rf $INSTALLDIR/gnu_opt/*
autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnu_opt" -build "gnu_opt" $(cat lofarconf.in) > $BUILDROOT/`uname -n`-week`date +%g%W`-gnu_opt.log 2>&1

# GNU_NOLOG4CPLUS
#
#source $INSTALLDIR/gnunew_nolog4cplus/lofarinit.sh
rm -rf $INSTALLDIR/gnunew_nolog4cplus/*
autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnunew_nolog4cplus" -build "gnunew_nolog4cplus" $(cat lofarconf.in) > $BUILDROOT/`uname -n`-week`date +%g%W`-gnunew_nolog4cplus.log 2>&1

# GNUNEW_DEBUG
#
#source $INSTALLDIR/gnunew_debug/lofarinit.sh
rm -rf $INSTALLDIR/gnunew_debug/*
autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnunew_debug" -build "gnunew_debug" $(cat lofarconf.in) > $BUILDROOT/`uname -n`-week`date +%g%W`-gnunew.log 2>&1

# GNU_MPICH
#
#source $INSTALLDIR/gnu_mpich/lofarinit.sh
rm -rf $INSTALLDIR/gnu_mpich/*
autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnu_mpich" -build "gnu_mpich" LCS/Common LCS/databases/PL LCS/Transport LCS/databases/TransportPL LCS/ACC CEP/tinyCEP CEP/CEPFrame > $BUILDROOT/`uname -n`-week`date +%g%W`-gnu_mpich.log 2>&1

# GNU_FAKEBGL
#
#source $INSTALLDIR/gnu_fakebgl/lofarinit.sh
rm -rf $INSTALLDIR/gnu_fakebgl/*
autoconf_share/rub -k -noupdate -release=main -confopt "--prefix=$INSTALLDIR/gnu_fakebgl" -build "gnu_fakebgl" LCS/Common LCS/Transport CEP/tinyCEP > $BUILDROOT/`uname -n`-week`date +%g%W`-gnu_fakebgl.log 2>&1

# ICC_DEBUG
#
#source $INSTALLDIR/icc_debug/lofarinit.sh
#rm -rf $INSTALLDIR/icc_debug/*
#autoconf_share/rub -system -noupdate -release=main -confopt "--prefix=$INSTALLDIR/icc_debug" -build "icc_debug" Appl LCS CEP Demo > $BUILDROOT/`uname -n`-week`date +%g%W`-iccdebug.log 2>&1

# DOCUMENTATION
#
#source $INSTALLDIR/gnu_opt/lofarinit.sh
rm -rf $INSTALLDIR/docxxhtml
mkdir -p $INSTALLDIR/docxxhtml
export DOXYGEN="/usr/local/doxygen/bin/doxygen";make docxx  PACKAGES="LCS CEP MAC SAS Demo" > $BUILDROOT/`uname -n`-week`date +%g%W`-docxx.log 2>&1


cd $BUILDROOT

#Prepare the buildresults on lofar0
LOFAR/autoconf_share/scanBuildLog -d LOFAR -f build.`uname -n`.


#Prepare the buildresults op dop40
LOFAR/autoconf_share/scanBuildLog -d LOFAR -f build.`uname -n`. -t lofarsw@dop40.astron.nl -s software

# To do b4 copied to lofar.org:
cd $INSTALLDIR/docxxhtml;./installdox -l doxygen.tag@http://www.lofar.org/software/docxxhtml/doxygen > /dev/null

cd $INSTALLDIR;tar -czvf docxx.tgz docxxhtml

cd $BUILDROOT/LOFAR;mkdir dep;cd dep
source $INSTALLDIR/gnu_debug/lofarinit.sh
makedep.sh;cd ../;tar -czvf dep.tgz dep

#send to and unpack on dop40 and put dep && docx in place in software/
scp dep.tgz lofarsw@dop40.astron.nl:software/uploads
cd $INSTALLDIR
scp docxx.tgz lofarsw@dop40.astron.nl:software/uploads


#unpack on dop40 and put docxx in place in software
ssh lofarsw@dop40.astron.nl -n "cd software;tar -xzvf uploads/docxx.tgz"
ssh lofarsw@dop40.astron.nl -n "cd software/TreeBrowser;tar -xzvf ../uploads/dep.tgz"
