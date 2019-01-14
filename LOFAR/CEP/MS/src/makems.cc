//# makems.cc: Make a distributed MS
//#
//# Copyright (C) 2005
//# ASTRON (Netherlands Foundation for Research in Astronomy)
//# P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//# This program is free software; you can redistribute it and/or modify
//# it under the terms of the GNU General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or
//# (at your option) any later version.
//#
//# This program is distributed in the hope that it will be useful,
//# but WITHOUT ANY WARRANTY; without even the implied warranty of
//# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//# GNU General Public License for more details.
//#
//# You should have received a copy of the GNU General Public License
//# along with this program; if not, write to the Free Software
//# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//# $Id: makems.cc 12842 2009-03-09 08:11:29Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <MS/MSCreate.h>
// #include <MS/VdsMaker.h>
#include <Common/ParameterSet.h>
#include <Common/LofarLogger.h>

#include <casa/Quanta/MVTime.h>
#include <casa/Quanta/MVAngle.h>
#include <casa/Quanta/Quantum.h>
#include <casa/Arrays/Array.h>
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>
#include <casa/OS/Path.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/ScalarColumn.h>
#include <tables/Tables/ArrayColumn.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace LOFAR;
using namespace casacore;
using namespace std;

// Define the variables shared between the functions.
vector<double> itsRa;
vector<double> itsDec;
Array<double>  itsAntPos;
bool   itsWriteAutoCorr;
bool   itsWriteImagerCol;
int    itsNPart;
int    itsNBand;
int    itsNFreq;
int    itsNTime;
int    itsTileSizeFreq;
int    itsTileSizeRest;
int    itsNFlags;
vector<double> itsStartFreq;
vector<double> itsStepFreq;
double itsStartTime;
double itsStepTime;
string itsMsName;
string itsFlagColumn;
string itsVdsPath;
string itsClusterDescName;


void readParms (const string& parset)
{
  ParameterSet params (parset);
  // Get the various parameters.
  itsStepFreq  = params.getDoubleVector ("StepFreq");
  itsStartFreq = params.getDoubleVector ("StartFreq");
  itsStepTime  = params.getDouble ("StepTime");
  string startTimeStr = params.getString ("StartTime");
  Quantity qn;
  ASSERT (MVTime::read (qn, startTimeStr, true));
  itsStartTime = qn.getValue ("s");
  vector<string> raStr  = params.getStringVector ("RightAscension");
  vector<string> decStr = params.getStringVector ("Declination");
  ASSERT (raStr.size() > 0  &&  raStr.size() == decStr.size());
  for (uint i=0; i<raStr.size(); ++i) {
    ASSERT (MVAngle::read (qn, raStr[i], true));
    itsRa.push_back  (qn.getValue ("rad"));
    ASSERT (MVAngle::read (qn, decStr[i], true));
    itsDec.push_back (qn.getValue ("rad"));
  }
  itsNBand = params.getInt32 ("NBands");
  itsNFreq = params.getInt32 ("NFrequencies");
  itsNTime = params.getInt32 ("NTimes");
  itsNPart = params.getInt32 ("NParts");
  // Determine possible tile size. Default is no tiling.
  itsTileSizeFreq = params.getInt32 ("TileSizeFreq", -1);
  itsTileSizeRest = params.getInt32 ("TileSizeRest", -1);
  // Determine nr of bands per part.
  ASSERT (itsNPart > 0);
  ASSERT (itsNBand > 0);
  if (itsNBand > itsNPart) {
    ASSERT (itsNBand%itsNPart == 0);
  } else {
    // If fewer bands than parts, bands are spread over parts which is the
    // same as having as many bands as parts.
    ASSERT (itsNPart%itsNBand == 0);
    itsNBand = itsNPart;
  }
  ASSERT (itsNFreq >= itsNBand);
  ASSERT (itsNFreq%itsNBand == 0);
  ASSERT (itsStepTime > 0);
  // Determine start and step frequency per band.
  int nfpb = itsNFreq/itsNBand;
  ASSERT (itsStepFreq.size() > 0);
  if (itsStepFreq.size() == 1) {
    itsStepFreq = vector<double>(itsNBand, itsStepFreq[0]);
  }
  ASSERT (itsStepFreq.size() == uint(itsNBand));
  ASSERT (itsStartFreq.size() > 0);
  if (itsStartFreq.size() == 1) {
    itsStartFreq.resize (itsNBand);
    for (int i=1; i<itsNBand; ++i) {
      itsStartFreq[i]  = itsStartFreq[i-1] + nfpb*itsStepFreq[i-1];
    }
  }
  ASSERT (itsStartFreq.size() == uint(itsNBand));
  for (int i=0; i<itsNBand; ++i) {
    ASSERT (itsStepFreq[i] > 0);
    ASSERT (itsStartFreq[i] > 0);
  }
  // Get remaining parameters.
  itsWriteAutoCorr  = params.getBool   ("WriteAutoCorr", False);
  itsWriteImagerCol = params.getBool   ("WriteImagerColumns", True);
  itsMsName         = params.getString ("MSName");
  itsFlagColumn     = params.getString ("FlagColumn", "");
  itsNFlags         = params.getInt    ("NFlagBits", 8);
  // Get directory part of MSName.
  string defaultVdsPath;
  string::size_type pos = itsMsName.rfind ('/');
  if (pos != string::npos) {
    defaultVdsPath = itsMsName.substr (pos+1);
  }
  if (defaultVdsPath.empty()) {
    defaultVdsPath = ".";
  }
  itsVdsPath = params.getString ("VDSPath", defaultVdsPath);
  itsClusterDescName = params.getString ("ClusterDescName", "");
  // Get the station info from the given antenna table.
  string tabName = params.getString ("AntennaTableName");
  Table tab(tabName, TableLock(TableLock::AutoNoReadLocking));
  ROArrayColumn<double> posCol(tab, "POSITION");
  itsAntPos = posCol.getColumn();
}


void createMS (int nband, int bandnr, const string& msName)
{
  int nfpb = itsNFreq/itsNBand;
  MSCreate msmaker(msName, itsStartTime, itsStepTime, nfpb, 4,
                   itsAntPos.shape()[1],
		   Matrix<double>(itsAntPos), itsWriteAutoCorr,
		   itsTileSizeFreq, itsTileSizeRest, itsFlagColumn, itsNFlags);
  for (int i=0; i<nband; ++i) {
    // Determine middle of band.
    double freqRef = itsStartFreq[bandnr] + nfpb*itsStepFreq[bandnr]/2;
    msmaker.addBand (4, nfpb, freqRef, itsStepFreq[bandnr]);
    ++bandnr;
  }
  for (uint i=0; i<itsRa.size(); ++i) {
    msmaker.addField (itsRa[i], itsDec[i]);
  }
  for (int i=0; i<itsNTime; ++i) {
    msmaker.writeTimeStep();
  }
  if (itsWriteImagerCol) {
    msmaker.addImagerColumns();
  }
}

string doOne (int seqnr, const string& msName, const string& vdsPath,
              const string& clusterDescName)
{
  int nbpp = itsNBand / itsNPart;
  // Form the MS name.
  // If it contains %d, use that to fill in the seqnr.
  // Otherwise append _seqnr to the name.
  string name;
  if (msName.find ("%d") != string::npos) {
    name = formatString (msName.c_str(), seqnr);
  } else {
    name = msName + toString (seqnr, "_p%d");
  }
  // Create the MS.
  createMS (nbpp, seqnr*nbpp, name);
  // Create the VDS file.
  string vdsName;
  if (vdsPath.empty()) {
    vdsName = name +".vds";
  } else {
    vdsName = vdsPath + '/' + string(Path(name).baseName()) + ".vds";
  }
//  VdsMaker::create (name, vdsName, clusterDescName, string(), false);
  return vdsName;
}

void doAll()
{
  vector<string> vdsNames;
  for (int i=0; i<itsNPart; ++i) {
    vdsNames.push_back (doOne (i, itsMsName, itsVdsPath, itsClusterDescName));
  }
  // Combine the description files.
//  VdsMaker::combine (itsMsName+".gds", vdsNames);
  if (itsNPart == 1) {
    cout << "Created 1 MS part and its VDS file" << endl;
  } else {
    cout << "Created " << itsNPart << " MS parts and their VDS files" << endl;
  }
  cout << "Created global VDS file " << itsMsName+".gds" << endl;
}


int main (int argc, char** argv)
{
  INIT_LOGGER("makems");
  try {
    string parset ("makems.cfg");
    if (argc > 1) {
      parset = argv[1];
    }
    readParms (parset);
    if (argc < 3) {
      // Invoked directly. Create all parts here.
      doAll();
    } else {
      ASSERT (argc == 6);
      // Invoked through socketrun for a distributed run.
      // Arguments: seqnr msdir vdspath clusterdescname
      // Create the given part.
      int seqnr;
      istringstream istr(argv[2]);
      istr >> seqnr;
      string vdsName = doOne (seqnr, argv[3] + ('/' + itsMsName), argv[4],
                              argv[5]);
      // Print vdsName, so script can capture it.
      cout << "vds=" << vdsName << endl;
    }
  } catch (std::exception& x) {
    cout << "Unexpected exception in " << argv[0] << ": " << x.what() << endl;
    return 1;
  }
  return 0;
}
