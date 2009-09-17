//# MSSplit.cc: Program to split an MS into smaller parts
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
//# $Id: MSSplit.cc 7365 2006-01-02 12:39:30Z diepen $

#include <lofar_config.h>
#include <Blob/BlobOStream.h>
#include <Blob/BlobOBufStream.h>
#include <Blob/BlobArray.h>
#include <Common/LofarLogger.h>

#include <ms/MeasurementSets/MeasurementSet.h>
#include <ms/MeasurementSets/MSAntenna.h>
#include <ms/MeasurementSets/MSAntennaColumns.h>
#include <ms/MeasurementSets/MSDataDescription.h>
#include <ms/MeasurementSets/MSDataDescColumns.h>
#include <ms/MeasurementSets/MSField.h>
#include <ms/MeasurementSets/MSFieldColumns.h>
#include <ms/MeasurementSets/MSSpectralWindow.h>
#include <ms/MeasurementSets/MSSpWindowColumns.h>
#include <measures/Measures/MDirection.h>
#include <measures/Measures/MeasConvert.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/ScalarColumn.h>
#include <tables/Tables/ArrayColumn.h>
#include <casa/Arrays/Vector.h>
#include <casa/Arrays/ArrayLogical.h>
#include <casa/Utilities/GenSort.h>
#include <casa/IO/RegularFileIO.h>
#include <casa/Exceptions/Error.h>

#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace LOFAR;
using namespace casa;
using namespace std;

void getFreq (MS& ms, int ddid, int& nrchan,
	      double& startFreq, double& endFreq, double& stepFreq)
{
  // Get the frequency domain of the given data descriptor id
  // which gives the spwid.
  MSDataDescription mssub1(ms.dataDescription());
  ROMSDataDescColumns mssub1c(mssub1);
  int spw = mssub1c.spectralWindowId()(ddid);
  MSSpectralWindow mssub(ms.spectralWindow());
  ROMSSpWindowColumns mssubc(mssub);
  Vector<double> chanFreq = mssubc.chanFreq()(spw);
  Vector<double> chanWidth = mssubc.chanWidth()(spw);
  // So far, only equal frequency spacings are possible.
  if (! allEQ (chanWidth, chanWidth(0))) {
    throw AipsError("Channels must have equal spacings");
  }
  nrchan    = chanWidth.nelements();
  stepFreq  = chanWidth(0);
  startFreq = chanFreq(0) - stepFreq/2;
  endFreq   = startFreq + nrchan*stepFreq;
}

void getPhaseRef (MS& ms, double& ra, double& dec)
{
  MSField mssub(ms.field());
  ROMSFieldColumns mssubc(mssub);
  // Use the phase reference of the first field.
  MDirection phaseRef = mssubc.phaseDirMeasCol()(0)(IPosition(1,0));
  // Get RA and DEC in J2000.
  MDirection dir = MDirection::Convert (phaseRef, MDirection::J2000)();
  Quantum<Vector<double> > angles = dir.getAngle();
  ra  = angles.getBaseValue()(0);
  dec = angles.getBaseValue()(1);
}

void doIt (const string& in, const string& out, const string& column)
{
  // Open the table and make sure it is in the correct order.
  MS ms(in);
  Block<String> cols(3);
  cols[0] = "TIME";
  cols[1] = "ANTENNA1";
  cols[2] = "ANTENNA2";
  Table tab = ms.sort(cols);
  // Get all unique baselines.
  Vector<Int> a1;
  Vector<Int> a2;
  {
    Block<String> colsb(2);
    colsb[0] = "ANTENNA1";
    colsb[1] = "ANTENNA2";
    Table tabb = tab.sort(colsb, Sort::Ascending,
			  Sort::QuickSort+Sort::NoDuplicates);
    ROScalarColumn<Int> ant1(tabb,"ANTENNA1");
    ROScalarColumn<Int> ant2(tabb,"ANTENNA2");
    a1 = ant1.getColumn();
    a2 = ant2.getColumn();
  }
  // Get all unique times.
  ROScalarColumn<double> time(tab, "TIME");
  Vector<double> tim1 = time.getColumn();
  Vector<uInt> index;
  uInt nt = GenSortIndirect<double>::sort (index, tim1, Sort::Ascending,
					   Sort::InsSort+Sort::NoDuplicates);
  Vector<double> tim2(nt);
  Vector<double> interval2(nt);
  {
    ROScalarColumn<double> intvCol(tab, "INTERVAL");
    Vector<double> interval1 = intvCol.getColumn();
    for (uInt i=0; i<nt; i++) {
      tim2[i] = tim1[index[i]];
      interval2[i] = interval1[index[i]];
    }
  }
  // Check if they span the entire table.
  if (nt * a1.nelements() != tab.nrow()) {
    throw AipsError("#rows in MS " + in + " mismatches #times * #baselines");
  }

  // Open the data file.
  // Read all data and write it.
  int npol,nfreq;
  {
    RegularFileIO fil(RegularFile(out+".dat"), ByteIO::New);
    ROArrayColumn<Complex> cold(tab,column);
    for (uInt row=0; row<tab.nrow(); row++) {
      Array<Complex> data (cold(row));
      fil.write (data.nelements()*sizeof(Complex), data.data());
      if (row == 0) {
	npol = data.shape()[0];
	nfreq = data.shape()[1];
      } else {
	ASSERT (npol == data.shape()[0]  &&  nfreq == data.shape()[1]);
      }
    }
  }
  {
    // Write all UVW coordinates in a separate file .
    RegularFileIO fil(RegularFile(out+".uvw"), ByteIO::New);
    ROArrayColumn<double> coluvw(tab,"UVW");
    Array<double> uvw(coluvw.getColumn());
    fil.write (uvw.nelements()*sizeof(double), uvw.data());
  }
  // Now write out all descriptive data.
  {
    int nchan;
    double startFreq, endFreq, stepFreq, ra, dec;
    getFreq (ms, 0, nchan, startFreq, endFreq, stepFreq);
    getPhaseRef (ms, ra, dec);
    ASSERT (nchan == nfreq);
    string name(out+".des");
    std::ofstream ostr(name.c_str());
    BlobOBufStream bbs(ostr);
    BlobOStream bos(bbs);
    bos.putStart("ms.des", 1);
    bos << ra << dec << npol << nfreq << startFreq << endFreq << stepFreq;
    bos << a1 << a2;
    bos << tim2;
    bos << interval2;
    MSAntenna mssub(ms.antenna());
    ROMSAntennaColumns mssubc(mssub);
    bos << mssubc.position().getColumn();
    bos.putEnd();
  }
  cout << "Wrote " << npol << " polarizations" << endl;
  cout << "      " << nfreq << " frequency channels" << endl;
  cout << "      " << a1.nelements() << " baselines" << endl;
  cout << "      " << tim2.nelements() << " times" << endl;
  cout << " into file " << out << " (.dat, .uvw and .des)" << endl;
}

int main(int argc, char** argv)
{
  try {
    if (argc < 3) {
      cout << "Run as:  MSSplit in out [datacolumn]" << endl;
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
