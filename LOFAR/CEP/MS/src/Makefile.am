lib_LTLIBRARIES		= libbbms.la

libbbms_la_SOURCES	= Package__Version.cc  \
			  MSCreate.cc \
			  VdsMaker.cc

bin_PROGRAMS 		= versionms MSSplit \
			  makevds \
			  combinevds \
			  makems \
			  msplay

MSSplit_SOURCES		= MSSplit.cc
MSSplit_DEPENDENCIES	= $(LOFAR_DEPEND)

makevds_SOURCES		= makevds.cc
makevds_LDADD		= libbbms.la
makevds_DEPENDENCIES	= libbbms.la $(LOFAR_DEPEND)

combinevds_SOURCES	= combinevds.cc
combinevds_LDADD	= libbbms.la
combinevds_DEPENDENCIES	= libbbms.la $(LOFAR_DEPEND)

makems_SOURCES		= makems.cc
makems_LDADD		= libbbms.la
makems_DEPENDENCIES	= libbbms.la $(LOFAR_DEPEND)

msplay_SOURCES		= msplay.cc
msplay_LDADD		= libbbms.la
msplay_DEPENDENCIES	= libbbms.la $(LOFAR_DEPEND)

glishdir		= $(libexecdir)/glish
dist_glish_SCRIPTS	= mssplit.g statconv.g

scriptdir		= $(bindir)
dist_script_SCRIPTS	= mssplit makemsdistr makemsdistr-part

versionms_SOURCES      = versionms.cc
versionms_LDADD        = libbbms.la
versionms_DEPENDENCIES = libbbms.la $(LOFAR_DEPEND)

include $(top_srcdir)/Makefile.common
