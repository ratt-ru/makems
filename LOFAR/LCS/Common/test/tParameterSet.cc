//#  tParameterSet.cc: Simple testprogrm to test the ParameterSet class.
//#
//#  Copyright (C) 2002-2003
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
//#  $Id: tParameterSet.cc 12371 2008-12-23 13:18:31Z loose $

//# Always #inlcude <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/lofar_iostream.h>
#include <iterator>

#include <Common/ParameterSet.h>
#include <Common/Exceptions.h>
#include <Common/LofarLogger.h>
#include <Common/StreamUtil.h>

#include <climits>
#include <cfloat>

using namespace std;
using namespace LOFAR;

int doIt(KeyCompare::Mode mode)
{
  string cmpMode   (mode == KeyCompare::NOCASE ? "nocase" : "normal");
  string paramFile ("tParameterSet.in_param_"   + cmpMode);
  string mergeFile ("tParameterSet.in_merge_"   + cmpMode);
  string newsetFile("tParameterSet_tmp.newset." + cmpMode);

  try
  {
    cout << "\n>>>";
    cout << "\nReading in parameterfile '" << paramFile << "'\n";
    ParameterSet		myPS(paramFile, mode);
    cout << "This ParameterSet does " << toUpper(cmpMode) 
	 << " key comparison.\n";
    cout << "<<<\n";

    ParameterSet		myPS2(mode);

    // Try a self-adopt. Should return the same set.
    myPS.adoptCollection(myPS, "Aap.Noot.");

    // Adopt collection myPS in myPS2 using a prefix.
    myPS2.adoptCollection(myPS, "Foo.Bar.");
    ASSERT(myPS.getInt32("a.b.c") == myPS2.getInt32("Foo.Bar.a.b.c"));
    FAILWHEN(myPS2.isDefined("a.b"));

    // Re-init myPS with myPS2 by stripping prefix "Foo.Bar."
    // Should yield the same collection myPS
    myPS.adoptCollection(myPS2.makeSubset("Foo.Bar."));
    ASSERT(myPS.isDefined("a.b.double"));
    FAILWHEN(myPS.isDefined("Foo.Bar.a.b.double"));

    // Adopt buffered myPS in myPS2 using a different prefix.
    string buf;
    myPS.writeBuffer(buf);
    myPS2.clear();
    myPS2.adoptBuffer(buf, "Gnat.Gnu.");
    ASSERT(myPS.getString("a.b.lange_naam") == 
	   myPS2.getString("Gnat.Gnu.a.b.lange_naam"));
    FAILWHEN(myPS2.isDefined("a.b.lange_naam"));

    // Re-init myPS with myPS2 once more, stripping off prefix "Gnat.Gnu."
    // Should yield the same collection myPS again.
    myPS2.makeSubset("Gnat.Gnu.").writeBuffer(buf);
    myPS.adoptBuffer(buf);
    ASSERT(myPS.isDefined("a.b.double"));
    FAILWHEN(myPS.isDefined("Gnat.Gnu.a.b.double"));

    ASSERT(myPS.getBool("Bool", true));
    ASSERT(myPS.getInt16("Int16", SHRT_MIN) == SHRT_MIN);
    ASSERT(myPS.getUint16("Uint16", USHRT_MAX) == USHRT_MAX);
    ASSERT(myPS.getInt32("Int32", INT_MIN) == INT_MIN);
    ASSERT(myPS.getUint32("Uint32", UINT_MAX) == UINT_MAX);
#if HAVE_LONG_LONG
    ASSERT(myPS.getInt64("Int64", LONG_MIN) == LONG_MIN);
    ASSERT(myPS.getUint64("Uint64", ULONG_MAX) == ULONG_MAX);
#endif
    ASSERT(myPS.getFloat("FloatMin", FLT_MIN) == FLT_MIN);
    ASSERT(myPS.getFloat("FloatMax", FLT_MAX) == FLT_MAX);
    ASSERT(myPS.getDouble("DoubleMin", DBL_MIN) == DBL_MIN);
    ASSERT(myPS.getDouble("DoubleMax", DBL_MAX) == DBL_MAX);
    ASSERT(myPS.getString("String", "Hello World") == "Hello World");
    ASSERT(myPS.getTime("Time", 15) == 15);
    ASSERT(myPS.getTime("Time", 18000) == 18000);
    ASSERT(myPS["emptyvec"].getVector().size() == 0);
    ASSERT(myPS.getUint32Vector("emptyvec").size() == 0);

    {
      vector<bool> v(2); v[0] = true; v[1] = false;
      ASSERT(myPS.getBoolVector("BoolVector", v) == v);
    }
    {
      vector<int16> v(2); v[0] = SHRT_MIN; v[1] = SHRT_MAX;
      ASSERT(myPS.getInt16Vector("Int16Vector", v) == v);
    }
    {
      vector<uint16> v(2); v[0] = 0; v[1] = USHRT_MAX;
      ASSERT(myPS.getUint16Vector("Uint16Vector", v) == v);
    }
    {
      vector<int32> v(2); v[0] = INT_MIN; v[1] = INT_MAX;
      ASSERT(myPS.getInt32Vector("Int32Vector", v) == v);
    }
    {
      vector<uint32> v(2); v[0] = 0; v[1] = UINT_MAX;
      ASSERT(myPS.getUint32Vector("Uint32Vector", v) == v);
    }
#if HAVE_LONG_LONG
    {
      vector<int64> v(2); v[0] = LONG_MIN; v[1] = LONG_MAX;
      ASSERT(myPS.getInt64Vector("Int64Vector", v) == v);
    }
    {
      vector<uint64> v(2); v[0] = 0; v[1] = ULONG_MAX;
      ASSERT(myPS.getUint64Vector("Uint64Vector", v) == v);
    }
#endif
    {
      vector<float> v(2); v[0] = FLT_MIN; v[1] = FLT_MAX;
      ASSERT(myPS.getFloatVector("FloatVector", v) == v);
    }
    {
      vector<double> v(2); v[0] = DBL_MIN; v[1] = DBL_MAX;
      ASSERT(myPS.getDoubleVector("DoubleVector", v) == v);
    }
    {
      vector<string> v(2); v[0] = "Hello"; v[1] = "World";
      ASSERT(myPS.getStringVector("StringVector", v) == v);
    }
    {
      vector<time_t> v(2); v[0] = time_t(15); v[1] = time_t(18000);
      ASSERT(myPS.getTimeVector("TimeVector", v) == v);
    }

    cout << "\nShowing some values\n";
    cout << "a.b.c=" 			<< myPS.getInt32("a.b.c") << endl;
    cout << "a.b=" 				<< myPS.getInt32("a.b") << endl;
    cout.precision(20);
    cout << "a.b.double="		<< myPS.getDouble("a.b.double") << endl;
    cout << "a.b.lange_naam="	<< myPS.getString("a.b.lange_naam") << endl;

    cout << "\n>>>";
    cout << "\nMerging ParameterSet with file '" << mergeFile << "'\n";
    cout << "<<<\n";
    myPS.adoptFile(mergeFile);

    cout << "\nShowing the same keys again\n";
    cout << "a.b.c=" 			<< myPS.getInt32("a.b.c") << endl;
    cout << "a.b=" 				<< myPS.getInt32("a.b") << endl;
    cout.precision(20);
    cout << "a.b.double="		<< myPS.getDouble("a.b.double") << endl;
    cout << "a.b.lange_naam="	<< myPS.getString("a.b.lange_naam") << endl;

    cout << "a.b.time1="        << myPS.getTime("a.b.time1") << endl;
    cout << "a.b.time2="        << myPS.getTime("a.b.time2") << endl;
    cout << "a.b.time3="        << myPS.getTime("a.b.time3") << endl;

    cout << "\nThe main ParameterSet contains:\n";
    cout << myPS;

    cout << "Fullname of 'b'=" << myPS.locateModule("b") << endl;

    cout << "isValidVersionNr(1.2.3.4)   = " << isValidVersionNr("1.2.3.4") << endl;
    cout << "isValidVersionNr(1.2.3)     = " << isValidVersionNr("1.2.3") << endl;
    cout << "isValidVersionNr(1.2)       = " << isValidVersionNr("1.2") << endl;
    cout << "isValidVersionNr(stable)    = " << isValidVersionNr("stable") << endl;
    cout << "isValidVersionNrRef(1.2.3)  = " << isValidVersionNrRef("1.2.3") << endl;
    cout << "isValidVersionNrRef(1.2)    = " << isValidVersionNrRef("1.2") << endl;
    cout << "isValidVersionNrRef(stable) = " << isValidVersionNrRef("stable") << endl;
    cout << "isValidVersionNrRef(error)  = " << isValidVersionNrRef("error") << endl;
    cout << "isValidVersionNrRef(1.2.3.AndALotOfGarbageBehindTheLastNumberPart)  = " 
	 << isValidVersionNrRef("1.2.3.AndALotOfGarbageBehindTheLastNumberPart");

    ParameterSet		mySubset = myPS.makeSubset("a.b.");
    cout << "\nCreating a subset 'a.b.'\n";
    cout << "\nSubset a.b. contains:\n";
    cout << mySubset;

    cout << "\nTrying to read a non-existing key\n"; 
    cout << ">>>\n";
    try {
      myPS.getInt32("is.er.niet");
    }
    catch (APSException&) {
      cout << "<<<\n";
      cout << "Told you the key didn't exist." << endl;
    }

    cout << "\n>>>";
    cout << "\nFinally write the parameterset to '" << newsetFile << "'\n";
    cout << "<<<\n";
    myPS.writeFile(newsetFile);

      cout << "\ntesting getInt32Vector\n";
      vector<int32> intVector = myPS.getInt32Vector("vtest.intVector1Dim");
      cout << intVector.size() << " elements in intVector1Dim\n";
      copy (intVector.begin(), intVector.end(), 
	    std::ostream_iterator<int, char>(cout, ","));
      cout << endl;

      cout << "trying to read single int as vector\n";
      intVector = myPS.getInt32Vector("a.b.c");
      cout << intVector.size() << " elements in a.b.c\n";
      copy (intVector.begin(), intVector.end(), 
	    std::ostream_iterator<int, char>(cout, ","));
      cout << endl;

    // Iterate through all keys.
    cout << endl << "Iterate over all keys ..." << endl;
    for (ParameterSet::iterator iter = myPS.begin();
	 iter != myPS.end();
	 iter++) {
      cout << iter->first << endl;
    }

  }
  catch (std::exception& ex) {
    cout << "Unexpected exception: " << ex.what() << endl;
    return 1;
  }
  return 0;
}


int main()
{
  INIT_LOGGER("tParameterSet");
  uint fails(0);
  fails += doIt(KeyCompare::NORMAL);
  fails += doIt(KeyCompare::NOCASE);
  if (fails > 0) {
    cout << fails << " test(s) failed" << endl;
    return 1;
  }
  return 0;
}
