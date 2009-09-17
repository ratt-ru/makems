//#  testLogger.cc: Program to test the interface to the LofarLogger.
//#
//#  Copyright (C) 2004
//#  ASTRON (Netherlands Foundation for Research in Astronomy)
//#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//#  This program is free software; you can redistribute it and/or modify
//#  it under the terms of the GNU General Public License as published by
//#  the Free Software Foundation; either version 2 of the License, or
//#  (at your option) any later version.
//#
//#  This program is distributed in the hope that it will be useful,
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#  GNU General Public License for more details.
//#
//#  You should have received a copy of the GNU General Public License
//#  along with this program; if not, write to the Free Software
//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//#  $Id: testLogger.cc 11649 2008-08-26 07:18:27Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>
#include <Common/lofar_iostream.h>
#include <Common/LofarLogger.h>			// contains the API of the LofarLogger
#include <Common/StringUtil.h>

#include <sys/time.h>					// gettimeofday

using namespace std;
using namespace LOFAR;


//# 
//# Routine for calculating time differences
//#
double	timeDiff (struct timeval *b, struct timeval *e) {
	double diff = e->tv_sec - b->tv_sec + (((double)e->tv_usec - b->tv_usec)/1000000);
	return diff;
};

//#
//# Define some testclass with a local context logger for tracing
//#
class	myTestClass {
public:
	void	show() { 
		LOG_INFO ("Operator info from within the TestClass object");
		LOG_TRACE_FLOW("This trace-message is placed on a seperate logger");
	}
	
private:
	int		myIntVar;
	ALLOC_TRACER_CONTEXT				// attach seperate trace logger to this class

};
// create a logger 'my_test_class' and attach it to myTestclass.
#ifdef ENABLE_TRACER
INIT_TRACER_CONTEXT(myTestClass, "my_test_class");
#endif

class foo {
public:
	void	show() {
		LOG_TRACE_VAR  ("This message is NOT shown, its level is too low");
		LOG_TRACE_COND ("This message IS shown, its level is just high enough");
		LOG_TRACE_FLOW ("This message is also shown");
	}
private:
	ALLOC_TRACER_CONTEXT
};
#ifdef ENABLE_TRACER
INIT_TRACER_CONTEXT(foo, "foo");
#endif

//#
//# In functions you can also use the lifetime tracer although it is ment for
//# classes in the first place.
//#
int	myFunc (int param1) {
	// create a tracer object that logs construction and destruction
	LOG_TRACE_LIFETIME(TRACE_LEVEL_FLOW, "lifetime logging");

	LOG_INFO ("Operator info from within a function");
	return (param1 * param1);
}

//
// Add a class that is statically initialized and logs in constructor and
// destructor to test the order of initialization of the log4Cplus module.
//
class	staticInitializedClass {
public:
	staticInitializedClass() {
		LOG_INFO ("constructor:this line will not be shown since there are no appenders connected yet to the LCS.common logger");
	};

	~staticInitializedClass() {
		LOG_INFO ("destructor: logged after/during exit of program");
	};

	void show() {
		LOG_INFO ("in staticInitializedClass");
	};

private:
	ALLOC_TRACER_CONTEXT
};

#ifdef ENABLE_TRACER
INIT_TRACER_CONTEXT(staticInitializedClass, "sIclass");
#endif

// create an static instance of the class
static staticInitializedClass	mySiClass;

int main (int, char *argv[]) {
	int		maxLoop = 10;

	// Read in the log-environment configuration
	// We should always start with this.
	//INIT_LOGGER_AND_WATCH("testLogger", 10000);
	//INIT_VAR_LOGGER("testLogger", "testfile.log");
#ifdef HAVE_LOG4CXX
	INIT_LOGGER("testLogger.in_log4cxx");
#else
	INIT_LOGGER("testLogger");
#endif

	// Show operator were are on the air
	LOG_INFO (formatString("Program %s has started", argv[0]));

	// Show the use of the threshold of the loggers.
	//
	// The property-file creates a logger named TRC.foo with threshold LEVEL_COND
	// This threshold determines which message you will see:
	foo		fooObject;
	fooObject.show();

	// When we log to a logger that was NOT in the property-file the logger
	// is created on the fly and inherits the values of its ancestor.
	LOG_INFO ("Visible because of inheritance from 'foo'");

	LOG_TRACE_FLOW("Demo of the time is takes to log a message");
	struct timeval		startTime, endTime;

	LOG_INFO ("Log without any arguments");
	gettimeofday(&startTime, 0L);
	for (int i = 0; i < maxLoop; ++i) {
		LOG_INFO ("Log without any arguments");
	}
	gettimeofday(&endTime, 0L);
	LOG_INFO (formatString("This took %f milliseconds", 
									timeDiff(&startTime, &endTime) * maxLoop));

	LOG_INFO ("Log with printf-like arguments");
	gettimeofday(&startTime, 0L);
	for (int i = 0; i < maxLoop; ++i) {
		LOG_INFO (formatString("Log with %s arguments", "printf-like"));
	}
	gettimeofday(&endTime, 0L);
	LOG_INFO (formatString("This took %f milliseconds", 
									timeDiff(&startTime, &endTime) * maxLoop));

	LOG_INFO ("Log with leftshift arguments");
	gettimeofday(&startTime, 0L);
	for (int i = 0; i < maxLoop; ++i) {
		LOG_INFO_STR ("Log with " << "leftshift" << " arguments");
	}
	gettimeofday(&endTime, 0L);
	LOG_INFO_STR ("This took "  << timeDiff(&startTime, &endTime) * maxLoop <<
					" milliseconds");

	LOG_DEBUG("Also show some debug output");
	LOG_DEBUG_STR("Debug lines should contain" << "'file' and 'line' information");

	LOG_TRACE_FLOW("About to call myFunc which has a lifetime tracer");
	int		dummyVar = myFunc(5);
	LOG_TRACE_VAR(formatString("myFunc returned %d", dummyVar));
	
	LOG_TRACE_FLOW("Demo of local Logger in myTestClass:");
	myTestClass		mc;
	mc.show();

	// Show the importance of placing the messages on the right level.
	LOG_TRACE_FLOW("Each type of tracing has its own level");
	for (int i = 0; i < 5; ++i) {
		LOG_TRACE_LOOP(formatString("Loop counter = %d", i));
	}

	LOG_INFO("We now set the Threshold of the TRACE logger to "
					  "TRACE_LEVEL_VAR so the loop counter don't show up anymore");

#if defined(HAVE_LOG4CPLUS)
	// You normally do this by editing your propertyfile.
	log4cplus::Logger::getInstance("TRC").setLogLevel(TRACE_LEVEL_VAR);
#elif !defined(HAVE_LOG4CXX)
	getLFDebugContext().setLevel(TRACE_LEVEL_VAR);
#endif

	for (int i = 0; i < 5; ++i) {
		LOG_TRACE_LOOP(formatString("Loop counter = %d", i));
	}

	LOG_TRACE_FLOW ("Finally showing the use of exceptions and assertions");
	try {
		THROW(LOFAR::Exception,"Just THROWing an exception");
	}
	catch(LOFAR::Exception&	ex) {
		LOG_WARN_STR("Caught exception: " << ex << endl);
		// ... handle the exception here ...
	}

	int loops = 1;
	try {
		ASSERTSTR(loops == 0, "Trying an ASSERTion");
	}
	catch(LOFAR::Exception&	ex) {
		LOG_WARN_STR("Caught exception: " << ex << endl);
		// ... handle the exception here ...
	}

	LOG_INFO ("Trying an 'FAILWHEN'");
	try {
		FAILWHEN(loops != 0);
	}
	catch(LOFAR::Exception&	ex) {
		LOG_WARN_STR("Caught exception: " << ex << endl);
		// ... handle the exception here ...
	}

	// show that the static initialized object is still there
	mySiClass.show();

	LOG_INFO("Normal termination of program");

	return 0;

	// during termination of this program the mySiClass will be deleted and
	// the logging of the destructor should still come in the logfile.
	// If not the static initialisation of log4Cplus is not right.
}

