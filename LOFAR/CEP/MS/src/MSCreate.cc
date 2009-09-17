// MSCreate.cc: Implementation for creating a MeasurementSet
//
//  Copyright (C) 2005
//  ASTRON (Netherlands Foundation for Research in Astronomy)
//  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  $Id: MSCreate.cc 12842 2009-03-09 08:11:29Z diepen $

#include <lofar_config.h>

#include <MS/MSCreate.h>
#include <Common/LofarLogger.h>

#include <ms/MeasurementSets.h>
#include <tables/Tables/IncrementalStMan.h>
#include <tables/Tables/StandardStMan.h>
#include <tables/Tables/TiledColumnStMan.h>
////#include <tables/Tables/BitFlagsEngine.h>
#include <tables/Tables/TiledStManAccessor.h>
#include <tables/Tables/SetupNewTab.h>
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/ArrColDesc.h>
#include <tables/Tables/TableRecord.h>
#include <casa/Arrays/Vector.h>
#include <casa/Arrays/Cube.h>
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/ArrayMath.h>
#include <casa/Containers/Block.h>
#include <casa/Containers/Record.h>
#include <measures/Measures/MPosition.h>
#include <measures/Measures/MBaseline.h>
#include <measures/Measures/Muvw.h>
#include <measures/Measures/MeasTable.h>
#include <measures/Measures/Stokes.h>
#include <measures/Measures/MCBaseline.h>
#include <measures/Measures/MeasConvert.h>
#include <casa/Quanta/MVEpoch.h>
#include <casa/Quanta/MVDirection.h>
#include <casa/Quanta/MVPosition.h>
#include <casa/Quanta/MVBaseline.h>
#include <casa/OS/Time.h>
#include <casa/OS/SymLink.h>
#include <casa/BasicSL/Constants.h>
#include <casa/Utilities/Assert.h>
#include <casa/Exceptions/Error.h>
#include <casa/Arrays/Slicer.h>
#include <casa/Arrays/Slice.h>

using namespace LOFAR;
using namespace casa;

MSCreate::MSCreate (const std::string& msName,
		    double startTime, double timeStep, int nfreq, int ncorr,
		    int nantennas, const Matrix<double>& antPos,
		    bool writeAutoCorr,
		    int tileSizeFreq, int tileSizeRest,
                    const std::string& flagColumn, int nflagBits)
: itsWriteAutoCorr (writeAutoCorr),
  itsNrBand        (0),
  itsNrField       (0),
  itsNrAnt         (nantennas),
  itsNrFreq        (nfreq),
  itsNrCorr        (ncorr),
  itsNrTimes       (0),
  itsTimeStep      (timeStep),
  itsStartTime     (startTime),
  itsNrPol         (0),
  itsNrChan        (0),
  itsPolnr         (0),
  itsAntBL         (0),
  itsFrame         (0),
  itsPhaseDir      (0),
  itsMS            (0),
  itsMSCol         (0)
{
  AlwaysAssert (nantennas > 0, AipsError);
  AlwaysAssert (nfreq > 0, AipsError);
  // Keep the antenna positions in ITRF coordinates.
  Block<MPosition> antMPos(nantennas);
  for (int i=0; i<nantennas; i++) {
    antMPos[i] = MPosition (MVPosition(antPos(0,i), antPos(1,i), antPos(2,i)),
			    MPosition::ITRF);
  }
  // Use the middle antenna as the array position.
  // Setup the frame for the UVW calculations.
  itsArrayPos = new MPosition(antMPos[nantennas/2]);
  itsFrame = new MeasFrame(*itsArrayPos);
  // Create the MS.
  createMS (msName, antMPos, tileSizeFreq, tileSizeRest, flagColumn, nflagBits);
  itsNrPol  = new Block<Int>;
  itsNrChan = new Block<Int>;
  itsPolnr  = new Block<Int>;
  // Fill the baseline vector for each antenna.
  fillBaseLines();
}

MSCreate::~MSCreate()
{
  if (itsMS != 0) {
    updateTimes();
  }
  delete itsNrPol;
  delete itsNrChan;
  delete itsPolnr;
  delete itsAntBL;
  delete itsArrayPos;
  delete itsFrame;
  delete itsPhaseDir;
  delete itsMSCol;
  delete itsMS;
}

int MSCreate::nrPolarizations() const
{
  return itsMS->polarization().nrow();
}


void MSCreate::createMS (const String& msName,
			 const Block<MPosition>& antPos,
			 int tileSizeFreq, int tileSizeRest,
                         const String& flagColumn, int nflagBits)
{
  // Create an integer flag column?
  if (flagColumn.empty()) {
    nflagBits = 0;
  }
  IPosition dataShape(2,itsNrCorr,itsNrFreq);
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
  td.rwColumnDesc(tsmNames[0]).setShape (dataShape);
  td.defineHypercolumn("TiledData", 3, tsmNames);
  tsmNames[0] = MS::columnName(MS::FLAG);
  td.rwColumnDesc(tsmNames[0]).setShape (dataShape);
  td.defineHypercolumn("TiledFlag", 3, tsmNames);////
  if (nflagBits <= 0) {
    td.defineHypercolumn("TiledFlag", 3, tsmNames);////
  } else {
    if (nflagBits == 8) {
      td.addColumn(ArrayColumnDesc<uChar>(flagColumn, 2));
    } else if (nflagBits == 16) {
      td.addColumn(ArrayColumnDesc<Short>(flagColumn, 2));
    } else {
      td.addColumn(ArrayColumnDesc<Int>(flagColumn, 2));
    }
    tsmNames[0] = flagColumn;
    td.rwColumnDesc(flagColumn).setShape (dataShape);
    td.defineHypercolumn("TiledIntFlag", 3, tsmNames);////
  }
  ////td.defineHypercolumn("TiledFlag", 3, tsmNames);
  tsmNames[0] = MS::columnName(MS::UVW);
  td.defineHypercolumn("TiledUVW", 2, tsmNames);
  // Setup the new table.
  // Most columns use the IncrStMan; some use others.
  SetupNewTable newTab(msName, td, Table::New);
  IncrementalStMan incrStMan("ISMData");
  newTab.bindAll (incrStMan);
  StandardStMan    stanStMan;
  newTab.bindColumn(MS::columnName(MS::ANTENNA1), stanStMan);
  newTab.bindColumn(MS::columnName(MS::ANTENNA2), stanStMan);
  // Use a TiledColumnStMan for the data, flags and UVW.
  // Store all pol and freq in a single tile.
  // In this way the data appear in separate files that can be mmapped.
  // Flags are stored as bits, so take care each tile has multiple of 8 flags.
  int tsf = tileSizeFreq;
  int tsr = tileSizeRest;
  if (tsf <= 0) {
    tsf = itsNrFreq;    // default is all channels
  }
  if (tsr <= 0) {
    tsr = std::max (1, 4096/tsf);
  }
  IPosition dataTileShape(3,4,tsf,tsr);
  TiledColumnStMan tiledData("TiledData", dataTileShape);
  newTab.bindColumn(MS::columnName(MS::DATA), tiledData);
  TiledColumnStMan tiledUVW("TiledUVW", IPosition(3,128));
  newTab.bindColumn(MS::columnName(MS::UVW), tiledUVW);
  TiledColumnStMan tiledFlag("TiledFlag", IPosition(3,4,tsf,8*tsr));
  newTab.bindColumn(MS::columnName(MS::FLAG), tiledFlag);
  if (nflagBits > 0) {
    TiledColumnStMan tiledIntFlag("TiledIntFlag", IPosition(3,4,tsf,8*tsr));////
    newTab.bindColumn(flagColumn, tiledIntFlag);////
    ////if (nflagBits == 8) {
    ////  BitFlagsEngine<uChar> fbe(MS::columnName(MS::FLAG), flagColumn);
    ////  newTab.bindColumn(MS::columnName(MS::FLAG), fbe);
    ////} else if (nflagBits == 16) {
    ////  BitFlagsEngine<Short> fbe(MS::columnName(MS::FLAG), flagColumn);
    ////  newTab.bindColumn(MS::columnName(MS::FLAG), fbe);
    ////} else {
    ////  BitFlagsEngine<Int> fbe(MS::columnName(MS::FLAG), flagColumn);
    ////  newTab.bindColumn(MS::columnName(MS::FLAG), fbe);
    ////}
  }
  // Create the MS and its subtables.
  // Get access to its columns.
  itsMS = new MeasurementSet(newTab);
  itsMSCol = new MSMainColumns(*itsMS);
  // Create all subtables.
  // Do this after the creation of optional subtables,
  // so the MS will know about those optional sutables.
  itsMS->createDefaultSubtables (Table::New);
  // Fill various subtables.
  fillAntenna (antPos);
  fillFeed();
  fillProcessor();
  fillObservation();
  fillState();
  // Find out which datamanagers contain DATA, FLAG and UVW.
  // Create symlinks for them, but only if no tiling in frequency.
  if (tsf >= itsNrFreq) {
    Record dminfo = itsMS->dataManagerInfo();
    for (uint i=0; i<dminfo.nfields(); ++i) {
      const Record& dm = dminfo.subRecord(i);
      String slname;
      if (dm.asString("NAME") == "TiledData") {
	slname = "/vis.DATA";
      } else if (dm.asString("NAME") == "TiledFlag") {
	slname = "/vis.flg";
      } else if (dm.asString("NAME") == "TiledUVW") {
	slname = "/vis.uvw";
      }
      if (! slname.empty()) {
	ostringstream ostr;
	ostr << "table.f" << i << "_TSM0";
	SymLink sl(msName+slname);
	sl.create (ostr.str());
      }
    }
  }
}

int MSCreate::addBand (int npolarizations, int nchannels,
		       double refFreq, double chanWidth)
{
  AlwaysAssert (nchannels > 0, AipsError);
  Vector<double> chanWidths(nchannels);
  chanWidths = chanWidth;
  Vector<double> chanFreqs(nchannels);
  indgen (chanFreqs, refFreq - (nchannels-1)*chanWidth/2., chanWidth);
  return addBand (npolarizations, nchannels, refFreq, chanFreqs, chanWidths);
}

int MSCreate::addBand (int npolarizations, int nchannels,
		       double refFreq, const double* chanFreqs,
		       const double* chanWidths)
{
  AlwaysAssert (nchannels > 0, AipsError);
  IPosition shape(1, nchannels);
  Vector<double> freqs (shape, const_cast<double*>(chanFreqs), SHARE);
  Vector<double> widths(shape, const_cast<double*>(chanWidths), SHARE);
  return addBand (npolarizations, nchannels, refFreq, freqs, widths);
}

int MSCreate::addBand (int npolarizations, int nchannels,
		       double refFreq, const Vector<double>& chanFreqs,
		       const Vector<double>& chanWidths)
{
  AlwaysAssert (npolarizations==1 || npolarizations==2 || npolarizations==4,
		AipsError);
  AlwaysAssert (nchannels > 0, AipsError);
  AlwaysAssert (npolarizations==itsNrCorr && nchannels==itsNrFreq, AipsError);
  // Find out if this nr of polarizations has already been given.
  Int polnr = -1;
  for (Int i=0; i<itsNrBand; i++) {
    if (npolarizations == (*itsNrPol)[i]) {
      polnr = (*itsPolnr)[i];
      break;
    }
  }
  // If not, add an entry to the POLARIZATION subtable.
  if (polnr < 0) {
    polnr = addPolarization (npolarizations);
  }
  // Add a row to the DATADESCRIPTION subtable.
  MSDataDescription msdd = itsMS->dataDescription();
  MSDataDescColumns msddCol(msdd);
  uInt rownr = msdd.nrow();
  msdd.addRow();
  msddCol.spectralWindowId().put (rownr, rownr);
  msddCol.polarizationId().put (rownr, polnr);
  msddCol.flagRow().put (rownr, False);
  // Add a row to the SPECTRALWINDOW subtable.
  // Find the total bandwidth from the minimum and maximum.
  Vector<double> stFreqs = chanFreqs - chanWidths/2.;
  Vector<double> endFreqs = chanFreqs + chanWidths/2.;
  double totalBW = max(endFreqs) - min(stFreqs);
  MSSpectralWindow msspw = itsMS->spectralWindow();
  MSSpWindowColumns msspwCol(msspw);
  msspw.addRow();
  msspwCol.numChan().put (rownr, nchannels);
  msspwCol.name().put (rownr, "");
  msspwCol.refFrequency().put (rownr, refFreq);
  msspwCol.chanFreq().put (rownr, chanFreqs);
  msspwCol.chanWidth().put (rownr, chanWidths);
  msspwCol.measFreqRef().put (rownr, MFrequency::TOPO);
  msspwCol.effectiveBW().put (rownr, chanWidths);
  msspwCol.resolution().put (rownr, chanWidths);
  msspwCol.totalBandwidth().put (rownr, totalBW);
  msspwCol.netSideband().put (rownr, 0);
  msspwCol.ifConvChain().put (rownr, 0);
  msspwCol.freqGroup().put (rownr, 0);
  msspwCol.freqGroupName().put (rownr, "");
  msspwCol.flagRow().put (rownr, False);
  // Now add the band to the internal blocks.
  itsNrBand++;
  itsNrPol->resize (itsNrBand);
  itsNrChan->resize (itsNrBand);
  itsPolnr->resize (itsNrBand);
  (*itsNrPol)[itsNrBand-1] = npolarizations;
  (*itsNrChan)[itsNrBand-1] = nchannels;
  (*itsPolnr)[itsNrBand-1] = polnr;
  msspw.flush();
  msdd.flush();
  return itsNrBand-1;
}

int MSCreate::addPolarization (int npolarizations)
{
  MSPolarization mspol = itsMS->polarization();
  MSPolarizationColumns mspolCol(mspol);
  uInt rownr = mspol.nrow();
  Vector<Int> corrType(npolarizations);
  corrType(0) = Stokes::XX;
  if (npolarizations == 2) {
    corrType(1) = Stokes::YY;
  } else if (npolarizations == 4) {
    corrType(1) = Stokes::XY;
    corrType(2) = Stokes::YX;
    corrType(3) = Stokes::YY;
  }
  Matrix<Int> corrProduct(2, npolarizations);
  for (Int i=0; i<npolarizations; i++) {
    corrProduct(0,i) = Stokes::receptor1(Stokes::type(corrType(i)));
    corrProduct(1,i) = Stokes::receptor2(Stokes::type(corrType(i)));
  }
  // Fill the columns.
  mspol.addRow();
  mspolCol.numCorr().put (rownr, npolarizations);
  mspolCol.corrType().put (rownr, corrType);
  mspolCol.corrProduct().put (rownr, corrProduct);
  mspolCol.flagRow().put (rownr, False);
  mspol.flush();
  return rownr;
}

int MSCreate::addField (double ra, double dec)
{
  MVDirection radec (Quantity(ra,"rad"), Quantity(dec,"rad"));
  MDirection indir(radec, MDirection::J2000);
  if (itsPhaseDir == 0) {
    itsPhaseDir = new Block<MDirection>();
  }
  itsPhaseDir->resize (itsNrField+1);
  (*itsPhaseDir)[itsNrField] = indir;
  Vector<MDirection> outdir(1);
  outdir(0) = indir;
  // Put the direction into the FIELD subtable.
  {
    MSField msfield = itsMS->field();
    MSFieldColumns msfieldCol(msfield);
    uInt rownr = msfield.nrow();
    msfield.addRow();
    msfieldCol.name().put (rownr, "BEAM_" + String::toString(rownr));
    msfieldCol.code().put (rownr, "");
    msfieldCol.time().put (rownr, itsStartTime);
    msfieldCol.numPoly().put (rownr, 0);
    msfieldCol.delayDirMeasCol().put (rownr, outdir);
    msfieldCol.phaseDirMeasCol().put (rownr, outdir);
    msfieldCol.referenceDirMeasCol().put (rownr, outdir);
    msfieldCol.sourceId().put (rownr, -1);
    msfieldCol.flagRow().put (rownr, False);
  }
  // Put the direction for each antenna into the POINTING subtable.
  {
    MSPointing mspointing = itsMS->pointing();
    MSPointingColumns mspointingCol(mspointing);
    uInt rownr = mspointing.nrow();
    mspointing.addRow(itsNrAnt);
    for (Int i=0; i<itsNrAnt; i++) {
      mspointingCol.antennaId().put (rownr, i);
      mspointingCol.time().put (rownr, itsStartTime);
      mspointingCol.interval().put (rownr, 0.);
      mspointingCol.name().put (rownr, "");
      mspointingCol.numPoly().put (rownr, 0);
      mspointingCol.timeOrigin().put (rownr, itsStartTime);
      mspointingCol.directionMeasCol().put (rownr, outdir);
      mspointingCol.targetMeasCol().put (rownr, outdir);
      mspointingCol.tracking().put (rownr, False);
      rownr++;
    }
  }
  itsNrField++;
  return itsNrField-1;
}

void MSCreate::fillAntenna (const Block<MPosition>& antPos)
{
  // Determine constants for the ANTENNA subtable.
  Vector<Double> antOffset(3);
  antOffset = 0;
  // Fill the ANTENNA subtable.
  MSAntenna msant = itsMS->antenna();
  MSAntennaColumns msantCol(msant);
  msant.addRow (itsNrAnt);
  for (Int i=0; i<itsNrAnt; i++) {
    msantCol.name().put (i, "ST_" + String::toString(i));
    msantCol.station().put (i, "LOFAR");
    msantCol.type().put (i, "GROUND-BASED");
    msantCol.mount().put (i, "ALT-AZ");
    msantCol.positionMeas().put (i, antPos[i]);
    msantCol.offset().put (i, antOffset);
    msantCol.dishDiameter().put (i, 150);
    msantCol.flagRow().put (i, False);
  }
  msant.flush();
}

void MSCreate::fillFeed()
{
  // Determine constants for the FEED subtable.
  Int nRec = 2;
  Matrix<Double> feedOffset(2,nRec);
  feedOffset = 0;
  Matrix<Complex> feedResponse(nRec,nRec);
  feedResponse = Complex(0.0,0.0);
  for (Int rec=0; rec<nRec; rec++) {
    feedResponse(rec,rec) = Complex(1.0,0.0);
  }
  Vector<String> feedType(nRec);
  feedType(0) = "X";
  feedType(1) = "Y";
  Vector<Double> feedPos(3);
  feedPos = 0.0;
  Vector<Double> feedAngle(nRec);
  feedAngle = -C::pi_4;                      // 0 for parallel dipoles
  // Fill the FEED subtable.
  MSFeed msfeed = itsMS->feed();
  MSFeedColumns msfeedCol(msfeed);
  msfeed.addRow (itsNrAnt);
  for (Int i=0; i<itsNrAnt; i++) {
    msfeedCol.antennaId().put (i, i);
    msfeedCol.feedId().put (i, 0);
    msfeedCol.spectralWindowId().put (i, -1);
    msfeedCol.time().put (i, itsStartTime + itsNrTimes*itsTimeStep/2.);
    msfeedCol.interval().put (i, itsNrTimes*itsTimeStep);
    msfeedCol.beamId().put (i, -1);
    msfeedCol.beamOffset().put (i, feedOffset);
    msfeedCol.polarizationType().put (i, feedType);
    msfeedCol.polResponse().put (i, feedResponse);
    msfeedCol.position().put (i, feedPos);
    msfeedCol.receptorAngle().put (i, feedAngle);
    msfeedCol.numReceptors().put (i, 2);
  }
  msfeed.flush();
}

void MSCreate::fillObservation()
{
  MSObservation msobs = itsMS->observation();
  MSObservationColumns msobsCol(msobs);
  Vector<String> corrSchedule(1);
  corrSchedule = "corrSchedule";
  Vector<Double> timeRange(2);
  timeRange(0) = itsStartTime;
  timeRange(1) = itsStartTime + itsNrTimes*itsTimeStep;
  // Data is public one year after end of observation.
  Double releaseDate = timeRange(1) + 365.25*24*60*60;
  // Fill the columns
  msobs.addRow();
  msobsCol.telescopeName().put (0, "LOFAR");
  msobsCol.timeRange().put (0, timeRange);
  msobsCol.observer().put (0, "MSCreate");
  msobsCol.scheduleType().put (0, "LOFAR");
  msobsCol.schedule().put (0, corrSchedule);
  msobsCol.project().put (0, "MSCreate");
  msobsCol.releaseDate().put (0, releaseDate);
  msobsCol.flagRow().put (0, False);
  msobs.flush();
}

void MSCreate::fillProcessor()
{
  MSProcessor msproc = itsMS->processor();
  MSProcessorColumns msprocCol(msproc);
  // Fill the columns
  msproc.addRow();
  msprocCol.type().put (0, "CORRELATOR");
  msprocCol.subType().put (0, "");
  msprocCol.typeId().put (0, -1);
  msprocCol.modeId().put (0, -1);
  msprocCol.flagRow().put (0, False);
  msproc.flush();
}

void MSCreate::fillState()
{
  MSState msstate = itsMS->state();
  MSStateColumns msstateCol(msstate);
  // Fill the columns
  msstate.addRow();
  msstateCol.sig().put (0, True);
  msstateCol.ref().put (0, False);
  msstateCol.cal().put (0, 0.);
  msstateCol.load().put (0, 0.);
  msstateCol.subScan().put (0, 0);
  msstateCol.obsMode().put (0, "");
  msstateCol.flagRow().put (0, False);
  msstate.flush();
}

void MSCreate::updateTimes()
{
  // Calculate the interval, end, and central time.
  Double interval = itsNrTimes*itsTimeStep;
  Double endTime = itsStartTime + interval;
  Double midTime = (itsStartTime + endTime) / 2;
  // Update all rows in FEED subtable.
  {
    MSFeed mssub = itsMS->feed();
    MSFeedColumns mssubCol(mssub);
    Vector<Double> val(mssub.nrow());
    val = midTime;
    mssubCol.time().putColumn (val);
    val = interval;
    mssubCol.interval().putColumn (val);
  }
  // Update all rows in POINTING subtable.
  {
    MSPointing mssub = itsMS->pointing();
    MSPointingColumns mssubCol(mssub);
    Vector<Double> val(mssub.nrow());
    val = midTime;
    mssubCol.time().putColumn (val);
    val = interval;
    mssubCol.interval().putColumn (val);
  }
  // Update all rows in OBSERVATION subtable.
  {
    MSObservation msobs = itsMS->observation();
    MSObservationColumns msobsCol(msobs);
    Vector<Double> timeRange(2);
    timeRange(0) = itsStartTime;
    timeRange(1) = itsStartTime + itsNrTimes*itsTimeStep;
    for (uInt i=0; i<msobs.nrow(); i++) {
      msobsCol.timeRange().put (i, timeRange);
    }
  }
}

void MSCreate::writeTimeStep()
{
  // Make sure all bands are equally sized.
  for (int i=1; i<itsNrBand; ++i) {
    ASSERT ((*itsNrPol)[i] == (*itsNrPol)[0]);
    ASSERT ((*itsNrChan)[i] == (*itsNrChan)[0]);
  }
  // Find the shape of the data array in each table row.
  IPosition shape(2, (*itsNrPol)[0], (*itsNrChan)[0]);
  Array<Bool> defFlags(shape);
  defFlags = False;
  Array<Complex> defData(shape);
  defData = Complex();
  int nrbasel = itsNrAnt*(itsNrAnt-1)/2;
  if (itsWriteAutoCorr) {
    nrbasel += itsNrAnt;
  }

  // Add the number of rows needed.
  int rowNumber = itsMS->nrow();
  itsMS->addRow (nrbasel*itsNrBand*itsNrField);
  Array<Float> sigma(IPosition(1, shape(0)));
  sigma = 1;
  Array<Float> weight(IPosition(1, shape(0)));
  weight = 1;
  Double time = itsStartTime + itsNrTimes*itsTimeStep + itsTimeStep/2;
  itsNrTimes++;
  // Calculate the UVW for all stations.
  // First store time in frame.
  Quantity qtime(time, "s");
  itsFrame->set (MEpoch(qtime, MEpoch::UTC));
  for (int field=0; field<itsNrField; ++field) {
    itsFrame->set ((*itsPhaseDir)[field]);
    vector<Vector<Double> > antuvw(itsNrAnt);
    for (int j=0; j<itsNrAnt; ++j) {
      MBaseline& mbl = (*itsAntBL)[j];
      mbl.getRefPtr()->set(*itsFrame);      // attach frame
      MBaseline::Convert mcvt(mbl, MBaseline::J2000);
      MVBaseline bas = mcvt().getValue();
      MVuvw jvguvw(bas, (*itsPhaseDir)[field].getValue());
      antuvw[j] = Muvw(jvguvw, Muvw::J2000).getValue().getVector();
    }
    
    Vector<double> myuvw(3);
    for (int band=0; band<itsNrBand; ++band) {
      defData = Complex(float(band), float(itsNrTimes));
      for (int j=0; j<itsNrAnt; ++j) {
	int st = (itsWriteAutoCorr ? j : j+1);
	for (int i=st; i<itsNrAnt; ++i) {
	  myuvw = antuvw[i] - antuvw[j];
	  itsMSCol->data().put(rowNumber, defData);
	  itsMSCol->flag().put(rowNumber, defFlags);
	  itsMSCol->flagRow().put (rowNumber, False);
	  itsMSCol->time().put (rowNumber, time);
	  itsMSCol->antenna1().put (rowNumber, j);
	  itsMSCol->antenna2().put (rowNumber, i);
	  itsMSCol->feed1().put (rowNumber, 0);
	  itsMSCol->feed2().put (rowNumber, 0);
	  itsMSCol->dataDescId().put (rowNumber, band);
	  itsMSCol->processorId().put (rowNumber, 0);
	  itsMSCol->fieldId().put (rowNumber, field);
	  itsMSCol->interval().put (rowNumber, itsTimeStep);
	  itsMSCol->exposure().put (rowNumber, itsTimeStep);
	  itsMSCol->timeCentroid().put (rowNumber, time);
	  itsMSCol->scanNumber().put (rowNumber, 0);
	  itsMSCol->arrayId().put (rowNumber, 0);
	  itsMSCol->observationId().put (rowNumber, 0);
	  itsMSCol->stateId().put (rowNumber, 0);
	  itsMSCol->uvw().put (rowNumber, myuvw);
	  itsMSCol->weight().put (rowNumber, weight);
	  itsMSCol->sigma().put (rowNumber, sigma);
	  rowNumber++;
	}
      }
    }
  }
}

void MSCreate::writeTimeStep2()
{
  int nrbasel = itsNrAnt*(itsNrAnt-1)/2;
  int rowNumber = itsMS->nrow();
  // Only one band and field can be used.
  ASSERT(itsNrBand==1);
  ASSERT(itsNrField==1);
  const int bandId=0;
  const int fieldId=0;
  // Find the shape of the data array in each table row.
  IPosition shape(2, (*itsNrPol)[bandId], (*itsNrChan)[bandId]);
  // Add the number of rows needed.
  itsMS->addRow (nrbasel);

  // Write everything but the data and the flags.
  Array<Float> sigma(IPosition(1, shape(0)));
  sigma = 0;
  Array<Float> weight(IPosition(1, shape(0)));
  weight = 1;
  Double time = itsStartTime + itsNrTimes*itsTimeStep + itsTimeStep/2;
  itsNrTimes++;
  // Calculate the UVW for all stations.
  // First store time in frame.
  Quantity qtime(time, "s");
  itsFrame->set (MEpoch(qtime, MEpoch::UTC));
  itsFrame->set ((*itsPhaseDir)[0]);
  vector<Vector<Double> > antuvw(itsNrAnt);
  for (int j=0; j<itsNrAnt; ++j) {
    MBaseline& mbl = (*itsAntBL)[j];
    mbl.getRefPtr()->set(*itsFrame);      // attach frame
    MBaseline::Convert mcvt(mbl, MBaseline::J2000);
    MVBaseline bas = mcvt().getValue();
    MVuvw jvguvw(bas, (*itsPhaseDir)[0].getValue());
    antuvw[j] = Muvw(jvguvw, Muvw::J2000).getValue().getVector();
  }
    
  Vector<double> myuvw(3);
  for (int j=0; j<itsNrAnt; ++j) {
    int st = (itsWriteAutoCorr ? j : j+1);
    for (int i=st; i<itsNrAnt; ++i) {
      myuvw = antuvw[i] - antuvw[j];
      itsMSCol->flagRow().put (rowNumber, False);
      itsMSCol->time().put (rowNumber, time);
      itsMSCol->antenna1().put (rowNumber, j);
      itsMSCol->antenna2().put (rowNumber, i);
      itsMSCol->feed1().put (rowNumber, 0);
      itsMSCol->feed2().put (rowNumber, 0);
      itsMSCol->dataDescId().put (rowNumber, bandId);
      itsMSCol->processorId().put (rowNumber, 0);
      itsMSCol->fieldId().put (rowNumber, fieldId);
      itsMSCol->interval().put (rowNumber, itsTimeStep);
      itsMSCol->exposure().put (rowNumber, itsTimeStep);
      itsMSCol->timeCentroid().put (rowNumber, time);
      itsMSCol->scanNumber().put (rowNumber, 0);
      itsMSCol->arrayId().put (rowNumber, 0);
      itsMSCol->observationId().put (rowNumber, 0);
      itsMSCol->stateId().put (rowNumber, 0);
      itsMSCol->uvw().put (rowNumber, myuvw);
      itsMSCol->weight().put (rowNumber, weight);
      itsMSCol->sigma().put (rowNumber, sigma);
      rowNumber++;
    }
  }

  // Set the row number to the first row of this time slot.
  rowNumber -= nrbasel;
  // If first time, set the cache size for the tiled data and flags.
  if (rowNumber == 0) {
    ROTiledStManAccessor accData(*itsMS, "TiledData");
    accData.setCacheSize (0, itsNrAnt*(itsNrAnt-1)/2);
    ////    accData.setCacheSize (0, 1);
    ROTiledStManAccessor accFlag(*itsMS, "TiledFlag");
    accFlag.setCacheSize (0, itsNrAnt*(itsNrAnt-1)/2);
    ////    accFlag.setCacheSize (0, 1);
  }
  const int chanPerGroup = 1;
  // Write the data in groups of chanPerGroup channels.
  // Find the shape of the data array in each channel.
  IPosition shape2(3, (*itsNrPol)[bandId], chanPerGroup, nrbasel);
  Array<Bool> defFlags(shape2);
  defFlags = False;
  Array<Complex> defData(shape2);
  defData = Complex();
  // Write the data for all channels.
  for (int i=0; i<shape[1]/chanPerGroup; ++i) {
    itsMSCol->data().putColumnRange(Slicer(Slice(rowNumber,nrbasel)),
				    Slicer(IPosition(2,0,i*chanPerGroup),
					   IPosition(2,shape[0],chanPerGroup)),
				    defData);
    itsMSCol->flag().putColumnRange(Slicer(Slice(rowNumber,nrbasel)),
				    Slicer(IPosition(2,0,i*chanPerGroup),
					   IPosition(2,shape[0],chanPerGroup)),
				    defFlags);
  }
}

void MSCreate::fillBaseLines()
{
  MSAntenna msan(itsMS->antenna());
  MSAntennaColumns msanc(msan);
  Int nr = msan.nrow();
  itsAntBL = new Block<MBaseline>(nr);
  for (uInt j=0; j<msan.nrow(); j++) {
    Vector<Double> antpos = msanc.position()(j);
    MVPosition blpos(antpos(0), antpos(1), antpos(2));
    (*itsAntBL)[j] = MBaseline (MVBaseline(blpos), MBaseline::ITRF);
  }
}

void MSCreate::addImagerColumns()
{
  // Find data shape and tile shape.
  IPosition shape(2,itsNrCorr,itsNrFreq);
  ROTiledStManAccessor tsm(*itsMS, "TiledData");
  IPosition dataTileShape(tsm.getTileShape(0));
  {
    TableDesc td;
    String colName = MS::columnName(MS::CORRECTED_DATA);
    td.addColumn (ArrayColumnDesc<Complex>(colName, "corrected data", shape,
                                           ColumnDesc::FixedShape));
    TiledColumnStMan stMan("TiledCorrectedData", dataTileShape);
    itsMS->addColumn (td, stMan);
  }
  {
    TableDesc td;
    String colName = MS::columnName(MS::MODEL_DATA);
    td.addColumn (ArrayColumnDesc<Complex>(colName, "model data", shape,
                                           ColumnDesc::FixedShape));
    TiledColumnStMan stMan("TiledModelData", dataTileShape);
    itsMS->addColumn (td, stMan);
  }
  {
    TableDesc td;
    String colName = MS::columnName(MS::IMAGING_WEIGHT);
    td.addColumn (ArrayColumnDesc<Float>(colName, "imaging weight",
                                         IPosition(1, shape[1]),
                                         ColumnDesc::FixedShape));
    TiledColumnStMan stMan("TiledImagingWeight", dataTileShape.getLast(2));
    itsMS->addColumn (td, stMan);
  }
  // Set keyword for casa::VisSet.
  // Sort out the channel selection.
  MSSpWindowColumns msSpW(itsMS->spectralWindow());
  Matrix<Int> selection(2, msSpW.nrow());
  // Fill in default selection (all bands and channels).
  selection.row(0) = 0;    //start
  selection.row(1) = msSpW.numChan().getColumn(); 
  ArrayColumn<Complex> mcd(*itsMS,  MS::columnName(MS::MODEL_DATA));
  mcd.rwKeywordSet().define ("CHANNEL_SELECTION",selection);
}
