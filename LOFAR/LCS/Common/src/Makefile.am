if HAVE_SHMEM
SUBDIRS	= shmem .
endif

if USE_BACKTRACE
backtrace_SRCS = AddressTranslator.cc Backtrace.cc SymbolTable.cc
endif

lib_LTLIBRARIES 	= libcommon.la

libcommon_la_SOURCES 	= Package__Version.cc \
	Allocator.cc \
	DataConvert.cc \
	Exception.cc \
	FileLocator.cc \
	hexdump.cc \
	KVpair.cc \
	lofar_bitset.cc \
	LofarLogCout.cc \
	LofarLogger.cc \
	Numeric.cc \
	ParameterSet.cc \
	ParameterSetImpl.cc \
	ParameterValue.cc \
	PrettyUnits.cc \
	Process.cc \
	ReadLine.cc \
	RunOnNode.cc \
	Stopwatch.cc \
	StreamUtil.cc \
	StringUtil.cc \
	SystemUtil.cc \
	Timer.cc \
	TypeNames.cc \
	Version.cc \
	Net/FdSet.cc \
	Net/Socket.cc

libcommon_la_SOURCES	+= $(backtrace_SRCS)

if HAVE_SHMEM
libcommon_la_LIBADD	= shmem/libshmem.la
endif

bin_PROGRAMS = versioncommon getparsetvalue

versioncommon_SOURCES      = versioncommon.cc
versioncommon_LDADD        = libcommon.la
versioncommon_DEPENDENCIES = libcommon.la $(LOFAR_DEPEND)

getparsetvalue_SOURCES      = getparsetvalue.cc
getparsetvalue_LDADD        = libcommon.la
getparsetvalue_DEPENDENCIES = libcommon.la $(LOFAR_DEPEND)

include $(top_srcdir)/Makefile.common
