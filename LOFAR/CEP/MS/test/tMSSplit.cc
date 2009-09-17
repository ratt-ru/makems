//# tMSSplit.cc: Program to check MSSplit
//#
//# Copyright (C) 2004
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
//# $Id: tMSSplit.cc 7365 2006-01-02 12:39:30Z diepen $

#include <lofar_config.h>
#include <Blob/BlobIStream.h>
#include <Blob/BlobIBufStream.h>
#include <Blob/BlobArray.h>
#include <Common/LofarLogger.h>

#include <tables/Tables/Table.h>
#include <tables/Tables/ScalarColumn.h>
#include <tables/Tables/ArrayColumn.h>
#include <casa/Arrays/Vector.h>
#include <casa/Arrays/Matrix.h>
#include <casa/IO/RegularFileIO.h>
#include <casa/Exceptions/Error.h>

#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace LOFAR;
using namespace casa;
using namespace std;

void doIt (const string& in, const string& out, const string& column)
{
  // Read in all descriptive data.
  int nfreq, npol, nbasel, ntim;
  Vector<double> tim;
  Matrix<Int> basel;
  {
    Vector<int> a1, a2;
    Vector<double> interval;
    double ra, dec, startFreq, endFreq, stepFreq;
    Matrix<double> pos;
    string name(out+".des");
    std::ifstream ostr(name.c_str());
    BlobIBufStream bbs(ostr);
    BlobIStream bos(bbs);
    bos.getStart("ms.des");
    bos >> ra >> dec >> npol >> nfreq >> startFreq >> endFreq >> stepFreq;
    bos >> a1 >> a2;
    bos >> tim;
    bos >> interval;
    bos >> pos;
    bos.getEnd();
    // Convert baselines to indices in a matrix.
    ntim = tim.nelements();
    nbasel = a1.nelements();
    basel.resize (nbasel, nbasel);
    basel = -1;
    for (int i=0; i<nbasel; i++) {
      basel(a1[i], a2[i]) = i;
    }
  }
  // Open the table.
  Table tab(in);
  ROScalarColumn<Int> ant1(tab, "ANTENNA1");
  ROScalarColumn<Int> ant2(tab, "ANTENNA2");
  ROScalarColumn<double> timeCol(tab, "TIME");
  ROArrayColumn<Complex> cold(tab, column);
  // Read in all data.
  Array<Complex> data(IPosition(4, npol, nfreq, nbasel, ntim));
  {
    RegularFileIO fil(RegularFile(out+".dat"), ByteIO::Old);
    fil.read (npol*nfreq*nbasel*ntim*sizeof(Complex), data.data());
  }
  int nrpt = 0;
  double lastTime = -1;
  int timeIndex = -1;
  for (uInt row=0; row<tab.nrow(); row++) {
    double time = timeCol(row);
    if (time != lastTime) {
      timeIndex = -1;
      for (int i=0; i<ntim; i++) {
	if (time == tim(i)) {
	  timeIndex = i;
	  break;
	}
      }
    }
    int blIndex = basel(ant1(row), ant2(row));
    ASSERT (timeIndex >= 0);
    ASSERT (blIndex >= 0);
    Matrix<Complex> msdata = cold(row);
    const Complex* dataPtr = data.data() +
                             (timeIndex*nbasel + blIndex) * nfreq * npol;
    for (int i=0; i<nfreq; i++) {
      for (int j=0; j<npol; j++) {
	ASSERT (msdata(j,i) == *dataPtr++);
	nrpt++;
      }
    }
  }
  cout << "Checked " << nrpt << " data points" << endl;
}

int main(int argc, char** argv)
{
  try {
    if (argc < 3) {
      cout << "Run as:  tMSSplit in out [datacolumn]" << endl;
      cout << "   datacolumn defaults to MODEL_DATA" << endl;
      return 0;
    }
    string column("MODEL_DATA");
    if (argc > 3) {
      column = argv[3];
    }
    doIt (argv[1], argv[2], column);
  } catch (exception& x) {
    cout << "Unexpected expection: " << x.what() << endl;
    return 1;
  }
  return 0;
}
