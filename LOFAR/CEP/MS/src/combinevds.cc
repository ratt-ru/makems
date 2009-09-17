//# combinevds.cc: Program to combine the description of MS parts
//#
//# Copyright (C) 2008
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
//# $Id: combinevds.cc 12718 2009-02-27 15:17:55Z diepen $

#include <lofar_config.h>
#include <MS/VdsMaker.h>
#include <Common/LofarLogger.h>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace LOFAR;
using namespace LOFAR::CEP;

int main (int argc, const char* argv[])
{
  try {
    if (argc < 3) {
      cout << "Run as:  combinevds outName in1 in2 ..." << endl;
      return 0;
    }
    // Form the vector of VDSnames.
    vector<string> vdsNames;
    vdsNames.reserve (argc-2);
    for (int i=2; i<argc; ++i) {
      vdsNames.push_back (argv[i]);
    }
    // Combine them.
    VdsMaker::combine (argv[1], vdsNames);

  } catch (exception& x) {
    cout << "Unexpected expection: " << x.what() << endl;
    return 1;
  }
  return 0;
}
