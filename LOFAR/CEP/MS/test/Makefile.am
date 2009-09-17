check_PROGRAMS		= tMSSplit

# programs to run through supplied checktools
CHECKTOOLPROGS		= tMSSplit

TESTS			= tMSSplit.sh tcombinevds.sh tmakems.sh

tMSSplit_SOURCES	= tMSSplit.cc
tMSSplit_DEPENDENCIES	= $(LOFAR_DEPEND)

EXTRA_DIST	= tcombinevds.run tcombinevds.stdout \
		  tcombinevds.in_vds1 tcombinevds.in_vds2 \
		  tmakems.run tmakems.parset tmakems.stdout \
		  tmakems.in_antenna.tgz 

include $(top_srcdir)/Makefile.common

