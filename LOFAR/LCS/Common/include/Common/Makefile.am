if HAVE_SHMEM
SUBDIRS	= shmem .
endif

if USE_BACKTRACE
backtrace_HDRS = AddressTranslator.h SymbolTable.h
endif

pkginclude_Netdir = $(pkgincludedir)/Net

pkginclude_HEADERS = Package__Version.h  \
	Allocator.h \
	Backtrace.h \
	CheckConfig.h \
	ComplexBuiltinFP.h \
	ComplexBuiltinInt.h \
	ComplexC99.h \
	ComplexStdFP.h \
	ComplexStdInt.h \
	DataConvert.h \
	DataFormat.h \
	Exception.h \
	Exceptions.h \
	FileLocator.h \
	hexdump.h \
	i4complex.h \
	KVpair.h \
	lofar_algorithm.h \
	lofar_bitset.h \
	LofarConstants.h \
	lofar_complex.h \
	lofar_datetime.h \
	lofar_deque.h \
	lofar_fstream.h \
	lofar_iomanip.h \
	lofar_iosfwd.h \
	lofar_iostream.h \
	lofar_list.h \
	LofarLocators.h \
	LofarLog4Cplus.h \
	LofarLog4Cxx.h \
	LofarLogCout.h \
	LofarLogger.h \
	lofar_map.h \
	lofar_math.h \
	lofar_numeric.h \
	lofar_set.h \
	lofar_smartptr.h \
	lofar_sstream.h \
	lofar_stack.h \
	lofar_string.h \
	lofar_thread.h \
	lofar_tribool.h \
	lofar_typeinfo.h \
	LofarTypedefs.h \
	LofarTypes.h \
	lofar_vector.h \
	Numeric.h \
	ObjectFactory.h \
	ParameterValue.h \
	ParameterSet.h \
	ParameterSetImpl.h \
	PrettyUnits.h \
	Process.h \
	ReadLine.h \
	RunOnNode.h \
	Semaphore.h \
	Singleton.h \
	Stopwatch.h \
	StreamUtil.h \
	StringUtil.h \
	SystemUtil.h \
	Timer.h \
	TypeNames.h \
	TypeNames.tcc \
	Version.h 

pkginclude_Net_HEADERS = \
	Net/FdSet.h \
	Net/Socket.h 

noinst_HEADERS = $(backtrace_HDRS)

include $(top_srcdir)/Makefile.common
