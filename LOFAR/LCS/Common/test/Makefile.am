# programs to run through supplied checktools
CHECKTOOLPROGS	=	testLogger tAllocator tDataConvert tTypeNames \
			tFileLocator tStringUtil tStreamUtil \
			tHexdump testSocket tComplex tProcess \
			tRunOnNode tTimer tNumeric tSingleton \
			tObjectFactory tSystemUtil tSaxpy tPrettyUnits \
			tParameterValue tParameterSet tParameterSetPerf \
			tBacktrace tReadLine

check_PROGRAMS	=	$(CHECKTOOLPROGS)

TESTSCRIPTS	= 	testLogger.sh tAllocator.sh tDataConvert.sh tTypeNames.sh \
			tFileLocator.sh tStringUtil.sh tStreamUtil.sh \
			tHexdump.sh testSocket.sh tComplex.sh tProcess.sh \
			tRunOnNode.sh tTimer.sh tNumeric.sh tSingleton.sh \
			tObjectFactory.sh tPrettyUnits.sh \
			tParameterValue.sh tParameterSet.sh \
			tParameterSetPerf.sh tgetparsetvalue.sh \
			tReadLine.sh

TESTS		=	$(TESTSCRIPTS)

EXTRA_DIST	=	$(TESTSCRIPTS) \
			testLogger.log_prop testLogger.debug \
			testLogger.in_log4cxx.log_prop \
			tDataConvert.stdout \
			tStringUtil.stdout \
			tStreamUtil.stdout \
			tHexdump.log_prop tHexdump.run tHexdump.stdout \
			testSocket.run \
			tComplex.stdout \
			tProcess.log_prop tProcess.run tProcess.stdout \
			tRunOnNode.log_prop tRunOnNode.stdout \
			tNumeric.run tNumeric.stdout \
			tObjectFactory.stdout tPrettyUnits.stdout \
			tParameterSet.in_merge_nocase \
			tParameterSet.in_merge_normal \
			tParameterSet.in_param_nocase \
			tParameterSet.in_param_normal \
			tParameterSet.stdout \
			tParameterSetPerf.run \
			tgetparsetvalue.parset \
			tgetparsetvalue.run \
			tgetparsetvalue.stdout \
			tReadLine.run tReadLine.in tReadLine.stdout

testLogger_SOURCES		= testLogger.cc
tAllocator_SOURCES		= tAllocator.cc
tDataConvert_SOURCES		= tDataConvert.cc
tTypeNames_SOURCES		= tTypeNames.cc
tFileLocator_SOURCES		= tFileLocator.cc
tStringUtil_SOURCES		= tStringUtil.cc
tStreamUtil_SOURCES		= tStreamUtil.cc
tHexdump_SOURCES		= tHexdump.cc
testSocket_SOURCES		= testSocket.cc
tComplex_SOURCES		= tComplex.cc
tProcess_SOURCES		= tProcess.cc
tRunOnNode_SOURCES		= tRunOnNode.cc
tTimer_SOURCES			= tTimer.cc
tNumeric_SOURCES		= tNumeric.cc
tSingleton_SOURCES		= tSingleton.cc
tObjectFactory_SOURCES		= tObjectFactory.cc \
				  Shapes.h Shapes.cc \
				  Coordinates.h Coordinates.cc
tSystemUtil_SOURCES		= tSystemUtil.cc
tSaxpy_SOURCES			= tSaxpy.cc
tPrettyUnits_SOURCES		= tPrettyUnits.cc
tBacktrace_SOURCES		= tBacktrace.cc
tParameterValue_SOURCES		= tParameterValue.cc
tParameterSet_SOURCES		= tParameterSet.cc
tParameterSetPerf_SOURCES	= tParameterSetPerf.cc
tReadLine_SOURCES		= tReadLine.cc

LDADD				= ../src/libcommon.la
DEPENDENCIES			= ../src/libcommon.la $(LOFAR_DEPEND)

include $(top_srcdir)/Makefile.common
