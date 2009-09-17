//# VdsMaker.h: Class to create the description of an MS
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
//# $Id: VdsMaker.h 12718 2009-02-27 15:17:55Z diepen $

#ifndef LOFAR_MS_VDSMAKER_H
#define LOFAR_MS_VDSMAKER_H

// @file
// Class to create the description of an MS
// @author Ger van Diepen (diepen AT astron nl)

#include <lofar_config.h>
#include <Common/lofar_vector.h>
#include <Common/lofar_string.h>
#include <MWCommon/ClusterDesc.h>
#include <ms/MeasurementSets/MeasurementSet.h>
#include <casa/Arrays/Vector.h>

namespace LOFAR {

  // @ingroup MS
  // @brief Class to create the description of an MS
  // @{

  // Class to create the description of a MeasurementSet (part).
  // The output is a vds file written in parset format
  // (vds = visibility data set).
  // All vds files of an entire MeasurementSet can be combined into
  // a gvds file which is used by the pipeline software to determine
  // on which nodes processses can be run the process the various
  // parts of an MS.
  //
  // The vds file can contain extra info. For example, the program
  // extra information is added for the RFI detection software of
  // Peter Fridman.

  class VdsMaker
  {
  public:
    // Create the description for the given MS and put it in a file
    // with the given name on the given host.
    // If the host name is empty, gethostname() will be used.
    // The given ClusterDesc file will be used to find the file system
    // on which the MS part os located. If the clusterDescName is empty,
    // the file system will be set to unknown.
    // It can be specified if the vectors holding the start and end time
    // of each time stamp should be made part of the VDS file.
    static void create (const string& msName, const string& outName,
			const string& clusterDescName,
			const string& hostName = string(),
                        bool fillTimes = true);

    // Combine the given VDS file into a global VDS file.
    static void combine (const string& gdsName,
                         const vector<string>& vdsNames);

  private:
    // Get the frequency info for each spectral window in the MS.
    // The vectors get the start and end frequency of each channel.
    static void getFreqInfo (casa::MS& ms, vector<int>& nrchan,
			     vector<casa::Vector<double> >& startFreq,
			     vector<casa::Vector<double> >& endFreq);

    // Get the directions of the fields.
    static void getFields (casa::MS& ms,
			   vector<double>& ra, vector<double>& dec);

    // Get the names of the antennae (stations).
    static void getAntNames (casa::MS& ms, vector<string>& antNames);

    // Get the names of the correlations (polarisations).
    static void getCorrInfo (casa::MS& ms, vector<string>& corrTypes);

    // Find out which file contains the DATA column.
    // Determine if the DATA are stored in a TSM file of itself.
    // Determine the cube and tile shape.
    static void getDataFileInfo (casa::MS& ms, string& name, bool& regular,
				 vector<int>& tileShape,
				 vector<int>& cubeShape);

    // Find the file system on which the given file is located.
    // If the host name is empty, gethostname() will be used.
    static string findFileSys (const string& fileName,
			       const CEP::ClusterDesc& cdesc,
			       const string& hostName);
  };

  // @}

} // end namespace

#endif
