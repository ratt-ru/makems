#!/bin/sh
#
# System build script
#
#


#
# Print usage message
#
usage ()
{
  echo "Usage: `basename $0` [OPTION]..."
  echo "Perform a system build of the LOFAR software"
  echo " Options are:"
  echo "  -b <build-dir>     root directory of the build"
  echo "  -c <config-file>   configuration file that describes which packages"
  echo "                       should be built for which variants"
  echo "  -f                 force removal of directories"
  echo "  -i <install-dir>   installation directory"
  echo "  -r <repos-root>    root directory or URL of the code repository"
  echo "  -s <source-dir>    root directory of the source code tree"
  echo "  -t <test-type>     run tests; n(o); c(heck); m(emcheck); d(istcheck)"
  echo "  -u <upload-dir>    upload directory for documentation and build"
  echo "                       results; format <user>@<host>:[<dir>]"
  echo "  -v cvs|svn         version control system used"
  exit 1
}


#
# Get command line options. Initialize parameters to sensible defaults 
# in case they were not supplied.
#
init ()
{
  echo "--> Initialization phase ..."

  # Get command line options
  while getopts b:c:fi:r:s:t:u:v: opt
  do
    case $opt in
      b) BUILDDIR="$OPTARG" ;;
      c) CFGFILE="$OPTARG" ;;
      f) FORCE="yes" ;;
      i) INSTALLDIR="$OPTARG" ;;
      r) VCSROOT="$OPTARG" ;;
      s) SOURCEDIR="$OPTARG" ;;
      t) case "$OPTARG" in 
           n|no) CHECK="nocheck" ;;
           c|check) CHECK="check" ;;
           m|memcheck) CHECK="memcheck" ;;
           d|distcheck) CHECK="distcheck" ;;
           *) usage ;;
         esac ;;
      u) UPLOADDIR="$OPTARG" ;;
      v) VCS="$OPTARG" ;;
      ?) usage ;;
    esac
  done
  shift $(($OPTIND - 1))

  CWD=${PWD}
  SOURCEDIR=${SOURCEDIR:=${PWD}}
  BUILDDIR=${BUILDDIR:=${SOURCEDIR}}
  INSTALLDIR=${INSTALLDIR:=${BUILDDIR}/installed}
#  UPLOADDIR=${UPLOADDIR:=${USER}@$(uname -n):${INSTALLDIR}}
  CFGFILE=${CFGFILE:=${SOURCEDIR}/LOFAR/autoconf_share/systembuild.$(uname -n)}
  FORCE=${FORCE:="no"}
  CHECK=${CHECK:=check}
  TEMPDIR=$(mktemp -d) || exit 1
  VCS=${VCS:=svn}

  if [ "$VCS" = "svn" ]; then
    VCSROOT=${VCSROOT:=https://svn.astron.nl}
  elif [ "$VCS" = "cvs" ]; then
    VCSROOT=${VCSROOT:=${CVSROOT:-:pserver:${USER}@cvs.astron.nl:/cvs/cvsroot}}
  else
    usage
  fi

}


#
# Show values of the different variables
#
show ()
{
  echo ""
  echo "Configuration file   : $CFGFILE"
  echo "Source directory     : $SOURCEDIR"
  echo "Build root directory : $BUILDDIR"
  echo "Install directory    : $INSTALLDIR"
  echo "Upload directory     : $UPLOADDIR"
  echo "Temporary directory  : $TEMPDIR"
  echo "Software repository  : $VCSROOT ($VCS)"
  echo "Remove directories   : $FORCE"
  echo "Checks to run        : $CHECK"
  echo ""
}


#
# Prepare build-, install-, and source-directories. These directories will be
# removed first. Next, empty directories will be created.
# 
prepare ()
{
  echo "--> Prepare phase ..."

  # Copy build configuration file to a safe place: our temporary directory.
  cp "$CFGFILE" "$TEMPDIR" || exit 1
  CFGFILE="$TEMPDIR/$(basename $CFGFILE)"

  # Check if any of the directories exist. They will only be removed if
  # FORCE is set to "yes" (i.e., by using the -f option)
  for dir in "$SOURCEDIR" "$BUILDDIR" "$INSTALLDIR"
  do
    if [ -d "$dir" -a "$FORCE" != "yes" ]; then
      echo "error: directory \`$dir' exists, use \`-f' to force removal."
      exit 1
    fi
  done

  # Remove and recreate directories.
  rm -rf   "$SOURCEDIR" "$BUILDDIR" "$INSTALLDIR"
  mkdir -p "$SOURCEDIR" "$BUILDDIR" "$INSTALLDIR" || exit 1
}


#
# Check out minimal LOFAR tree. Let 'rub' do the rest
#
prebuild()
{
  echo "--> Prebuild phase ..."
  if [ "$VCS" = "svn" ]; then
    ( $VCS checkout -N "$VCSROOT/LOFAR/trunk" "$SOURCEDIR/LOFAR" && \
      $VCS checkout "$VCSROOT/LOFAR/trunk/autoconf_share" \
                    "$SOURCEDIR/LOFAR/autoconf_share" ) > /dev/null || exit 1
  else
    ( cd "$SOURCEDIR" && 
      $VCS -d "$VCSROOT" checkout LOFAR/autoconf_share ) > /dev/null || exit 1
  fi
}


#
# Parse the configuration file. Execute a system build for each variant.
#
build ()
{
  echo "--> Build phase ..."
  cd "$SOURCEDIR/LOFAR"
  while read line
  do
    # Strip comments, i.e., anything following a `#' character.
    line=$(echo $line | sed 's,#.*$,,')
    # Use `eval' to expand wildcards shell wildcards;
    # extra `echo' strips off leading and trailing spaces;
    variants=$(echo $(eval echo $line | cut -s -d':' -f1))
    packages=$(echo $(eval echo $line | cut -s -d':' -f2-))
    if [ -n "$packages" ]; then 
      for variant in $variants
      do
        echo "Building variant \"$variant\" for packages \"$packages\""
        # Older versions of rub and rubbuild contain a bug that do an extra
        # shift when reading a "-k" option. A possible workaround is to use
        # "-k -k -j1" instead. The second "-k" will be "eaten" by rub; and 
        # the "-j1" option will be eaten by rubbuild.
        autoconf_share/rub -$VCS $VCS -k -$CHECK \
               -conf -confopt "--prefix=$INSTALLDIR/$variant" \
               -build "$variant::$BUILDDIR" $packages 
        # Save contents of non-variant specific logfiles to a file in our
        # temporary directory.
        cat "$SOURCEDIR/LOFAR/rub.log" >> "$TEMPDIR/rub.log"
        cat "$SOURCEDIR/LOFAR/build.log" >> "$TEMPDIR/build.log"

#      autoconf_share/rub -system -release=main \
#        -confopt "--prefix=$INSTALLDIR/$variant" \
#        -build $variant $packages \
#        > "$BUILDDIR/`uname -n`-`date +%Y%m%d`-$variant.log" 2>&1
      done
    fi
  done < "$CFGFILE"
}


postbuild () 
{
  echo "--> Postbuild phase ..."

  # Copy log files from the temporary directory to the build directory.
  if [ -r $TEMPDIR/rub.log ]; then 
    cp $TEMPDIR/rub.log $BUILDDIR
  fi
  if [ -r $TEMPDIR/build.log ]; then
    cp $TEMPDIR/build.log $BUILDDIR
  fi

  cd "$SOURCEDIR"
  opts="-d $SOURCEDIR/LOFAR"
  filebase="build.$(uname -n).$(echo $BUILDDIR | sed 's,[/@],_,g')"
  opts="$opts -f $filebase"

  host=$(echo $UPLOADDIR | sed -n "s/^\([^@]\+@[^@]\+\):.*$/\1/p")
  if [ -n "$host" ]; then 
    opts="$opts -t $host"
    dir=$(echo $UPLOADDIR | sed -n "s/^$host:\?//p")
    if [ -n "$dir" ]; then
      opts="$opts -s $dir"
    fi
  else
    opts="$opts -l"
  fi
  echo "opts=$opts"
  echo "PWD=$PWD"
  "$SOURCEDIR/LOFAR/autoconf_share/scanBuildLog" $opts #> /dev/null || exit 1
}


#
# Cleanup the mess we've made
#
cleanup ()
{
  echo "--> Cleanup phase ..."
  rm -rf $TEMPDIR;
  cd "$CWD"
}


# 
# Install trap handler
#
trap 'cleanup; trap - 0; exit' 0 1 2 3 15

init $*
show
echo "Build started at: $(date)"
prepare
prebuild
build
postbuild
echo "Build ended at: $(date)"

exit 0

cd $BUILDDIR

#Prepare the buildresults on lofar0
LOFAR/autoconf_share/scanBuildLog -d LOFAR -f lofar17-week`date +%g%W`-


#Prepare the buildresults op dop40
LOFAR/autoconf_share/scanBuildLog -d LOFAR -f lofar17-week`date +%g%W`- -t lofarsw@dop40.astron.nl -s software

# To do b4 copied to lofar.org:
cd $INSTALLDIR/docxxhtml;./installdox -l doxygen.tag@http://www.lofar.org/software/docxxhtml/doxygen > /dev/null

cd $INSTALLDIR;tar -czvf docxx.tgz docxxhtml

cd $BUILDDIR/LOFAR;mkdir dep;cd dep
source $INSTALLDIR/gnu_debug/lofarinit.sh
makedep.sh;cd ../;tar -czvf dep.tgz dep

#send to and unpack on dop40 and put dep && docx in place in software/
scp dep.tgz lofarsw@dop40.astron.nl:software/uploads
cd $INSTALLDIR
scp docxx.tgz lofarsw@dop40.astron.nl:software/uploads


#unpack on dop40 and put docxx in place in software
ssh lofarsw@dop40.astron.nl -n "cd software;tar -xzvf uploads/docxx.tgz"
ssh lofarsw@dop40.astron.nl -n "cd software/TreeBrowser;tar -xzvf ../uploads/dep.tgz"
