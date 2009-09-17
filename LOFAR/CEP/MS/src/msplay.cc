//#  msplay.cc: Play with creating and filling an MS
//#
//#  Copyright (C) 2007
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
//#  $Id: msplay.cc 10556 2007-10-12 13:39:48Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <ms/MeasurementSets.h>
#include <tables/Tables/IncrementalStMan.h>
#include <tables/Tables/StandardStMan.h>
#include <tables/Tables/IncrStManAccessor.h>
#include <tables/Tables/StandardStManAccessor.h>
#include <tables/Tables/TiledColumnStMan.h>
#include <tables/Tables/TiledStManAccessor.h>
#include <tables/Tables/SetupNewTab.h>
#include <tables/Tables/TableDesc.h>
#include <casa/Arrays/Array.h>
#include <casa/Exceptions/Error.h>
#include <casa/OS/Timer.h>
#include <vector>

using namespace casa;

int nrcorr=4;

MS* createMS (const String& msName, int nrch, int nrst, int nrcpt)
{
  // Get the MS main default table description.
  TableDesc td = MS::requiredTableDesc();
  // Add the data column and its unit.
  MS::addColumnToDesc(td, MS::DATA, 2);
  td.rwColumnDesc(MS::columnName(MS::DATA)).rwKeywordSet().
                                                      define("UNIT","Jy");
  // Store the data and flags in two separate files using TiledColumnStMan.
  // Also store UVW with TiledColumnStMan.
  Vector<String> tsmNames(1);
  tsmNames[0] = MS::columnName(MS::DATA);
  td.rwColumnDesc(tsmNames[0]).setShape (IPosition(2,nrcorr,nrch));
  td.defineHypercolumn("TiledData", 3, tsmNames);
  tsmNames[0] = MS::columnName(MS::FLAG);
  td.rwColumnDesc(tsmNames[0]).setShape (IPosition(2,nrcorr,nrch));
  td.defineHypercolumn("TiledFlag", 3, tsmNames);
  tsmNames[0] = MS::columnName(MS::UVW);
  td.defineHypercolumn("TiledUVW", 2, tsmNames);
  // Setup the new table.
  // Most columns use the IncrStMan; some use others.
  SetupNewTable newTab(msName, td, Table::New);
  IncrementalStMan incrStMan("ISMData");
  StandardStMan    stanStMan("SSMData", 32768);
  // Use a TiledColumnStMan for the data, flags and UVW.
  // Store all pol in a single tile.
  // Flags are stored as bits, so take care each tile has multiple of 8 flags.
  // Calculate nr of rows per tile.
  // Store (nrst+1)/2 rows in a tile; in that way the baselines span an
  // integer number of tiles.
  int rpt = (nrst+1)/2 * nrch/nrcpt;
  TiledColumnStMan tiledData("TiledData", IPosition(3,4,nrcpt,rpt));
  TiledColumnStMan tiledFlag("TiledFlag", IPosition(3,4,nrcpt,8*rpt));
  TiledColumnStMan tiledUVW("TiledUVW", IPosition(2,3,nrst*(nrst+1)));
  newTab.bindAll (incrStMan);
  newTab.bindColumn(MS::columnName(MS::ANTENNA1),stanStMan);
  newTab.bindColumn(MS::columnName(MS::ANTENNA2),stanStMan);
  newTab.bindColumn(MS::columnName(MS::DATA),tiledData);
  newTab.bindColumn(MS::columnName(MS::FLAG),tiledFlag);
  newTab.bindColumn(MS::columnName(MS::UVW),tiledUVW);
  // Create the MS and its subtables.
  // Get access to its columns.
  MS* msp = new MeasurementSet(newTab);
  // Create all subtables.
  // Do this after the creation of optional subtables,
  // so the MS will know about those optional sutables.
  msp->createDefaultSubtables (Table::New);
  return msp;
}

std::vector<MS*> create (int nrms, int nrch, int nrst, int nrcpt)
{
  std::vector<MS*> msvec;
  msvec.reserve (nrms);
  for (int i=0; i<nrms; ++i) {
    std::ostringstream str;
    str << i;
    msvec.push_back (createMS("a"+String(str.str())+".ms",
			      nrch, nrst, nrcpt));
  }
  return msvec;
}

void init (std::vector<MS*>& msvec,
	   int nrch, int nrbl, int nrtim,
	   bool write, int flushRate)
{
  int nrms = msvec.size();
  Array<Complex> arrd(IPosition(2,nrcorr,nrch));
  Array<Bool> arrf(IPosition(2,nrcorr,nrch));
  Array<Double> arru(IPosition(1,3));
  arrf = False;
  arru = 0.;
  int rownr=0;
  Timer timer;
  for (int i=0; i<nrtim; ++i) {
    for (int k=0; k<nrms; ++k) {
      msvec[k]->addRow(nrbl);
      if (write) {
	ArrayColumn<Complex> cold(*(msvec[k]), "DATA");
	ArrayColumn<Bool> colf(*(msvec[k]), "FLAG");
	ArrayColumn<Double> colu(*(msvec[k]), "UVW");
	for (int j=0; j<nrbl; ++j) {
	  cold.put (rownr+j, arrd);
	  colf.put (rownr+j, arrf);
	  colu.put (rownr+j, arru);
	}
	if (flushRate > 0  &&  (i+1)%flushRate == 0) {
	  msvec[k]->flush();
	}
      }
    }
    rownr += nrbl;
    //    sleep(5);
  }
  timer.show ("init  ");
}

void writeData (std::vector<MS*>& msvec,
		int nrch, int nrbl, int nrtim,
		int flushRate)
{
  int nrms = msvec.size();
  Array<Complex> arrd(IPosition(2,nrcorr,nrch));
  Array<Bool> arrf(IPosition(2,nrcorr,nrch));
  Array<Double> arru(IPosition(1,3));
  arrf = False;
  arru = 0.;
  Timer timer;
  for (int k=0; k<nrms; ++k) {
    ArrayColumn<Complex> cold(*msvec[k], "DATA");
    int rownr = 0;
    for (int i=0; i<nrtim; ++i) {
      for (int j=0; j<nrbl; ++j) {
	cold.put (rownr, arrd);
	++rownr;
      }
      if (flushRate > 0  &&  (i+1)%flushRate == 0) {
	msvec[k]->flush();
      }
    }
  }
  timer.show ("writed");
  timer.mark();
  for (int k=0; k<nrms; ++k) {
    ArrayColumn<Bool> colf(*msvec[k], "FLAG");
    int rownr = 0;
    for (int i=0; i<nrtim; ++i) {
      for (int j=0; j<nrbl; ++j) {
	colf.put (rownr, arrf);
	++rownr;
      }
      if (flushRate > 0  &&  (i+1)%flushRate == 0) {
	msvec[k]->flush();
      }
    }
  }
  timer.show ("writef");
  timer.mark();
  for (int k=0; k<nrms; ++k) {
    ArrayColumn<Double> colu(*msvec[k], "UVW");
    int rownr = 0;
    for (int i=0; i<nrtim; ++i) {
      for (int j=0; j<nrbl; ++j) {
	colu.put (rownr, arru);
	++rownr;
      }
      if (flushRate > 0  &&  (i+1)%flushRate == 0) {
	msvec[k]->flush();
      }
    }
  }
  timer.show ("writeu");
}

int main (int argc, char** argv)
{
  int nrms=0;
  int nrch=0;
  int nrst=0;
  int nrtim=0;
  int nrcpt=0;
  int write=0;
  int flushRate=0;
  if (argc > 1) {
    String str(argv[1]);
    if (str == "-h"  ||  str == "--help") {
      cout << "msplay shows the time it takes to create and fill MS-s.\n";
      cout << "It creates an MS in the way it is done by OLAP.\n";
      cout << "Several parameters (see below) can be varied.\n";
      cout << "It shows the times the various steps take.\n";
      cout << endl;
      cout << "Run as:   msplay nrms nrch nrst nrtim nrcpt write flush\n";
      cout << "  nrms   number of MS-s to create simultaneously   [1]\n";
      cout << "  nrch   number of frequency channels to use       [256]\n";
      cout << "  nrst   number of stations to use                 [8]\n";
      cout << "  nrtim  number of time stamps to write            [4320]\n";
      cout << "  nrcpt  number of channels per tile               [all]\n";
      cout << "  write  0=write while adding rows  1=write later  [0]\n";
      cout << "  flush  flush every <flush> time stamps (0=never) [0]\n";
      cout<< " A value 0 means use the default value.";
      cout << endl;
      return 0;
    }
    istringstream istr(argv[1]);
    istr >> nrms;
  }
  if (argc > 2) {
    istringstream istr(argv[2]);
    istr >> nrch;
  }
  if (argc > 3) {
    istringstream istr(argv[3]);
    istr >> nrst;
  }
  if (argc > 4) {
    istringstream istr(argv[4]);
    istr >> nrtim;
  }
  if (argc > 5) {
    istringstream istr(argv[5]);
    istr >> nrcpt;
  }
  if (argc > 6) {
    istringstream istr(argv[6]);
    istr >> write;
  }
  if (argc > 7) {
    istringstream istr(argv[7]);
    istr >> flushRate;
  }
  if (nrch <= 0) nrch = 1;
  if (nrms <= 0) nrms = 1;
  if (nrst <= 0) nrst = 12;
  int nrbl = nrst*(nrst+1);
  if (nrtim <= 0) nrtim = 4320;
  if (nrcpt <= 0) nrcpt = nrch;
  std::vector<MS*> msvec;
  try {
    msvec = create (nrms, nrch, nrst, nrcpt);
    init (msvec, nrch, nrbl, nrtim, write==0, flushRate);
    if (write != 0) {
      writeData (msvec, nrch, nrbl, nrtim, flushRate);
    }
    ROTiledStManAccessor accd(*(msvec[0]), "TiledData");
    accd.showCacheStatistics(cout);
    ROTiledStManAccessor accf(*msvec[0], "TiledFlag");
    accf.showCacheStatistics(cout);
    ROTiledStManAccessor accu(*msvec[0], "TiledUVW");
    accu.showCacheStatistics(cout);
    ROIncrementalStManAccessor acci(*msvec[0], "ISMData");
    acci.showCacheStatistics(cout);
    ROStandardStManAccessor accs(*msvec[0], "SSMData");
    accs.showCacheStatistics(cout);
    accs.showBaseStatistics(cout);
    Timer timer;
    for (int k=0; k<nrms; ++k) {
      msvec[k]->flush (true);
    }
    timer.show("fsync ");
  } catch (AipsError x) {
    cout << "Caught an exception: " << x.getMesg() << endl;
    return 1;
  } 
  Timer timer;
  for (int k=0; k<nrms; ++k) {
    delete msvec[k];
    msvec[k] = 0;
  }
  timer.show ("close ");
  return 0;                           // exit with success status
}
