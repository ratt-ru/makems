//# makevds.cc: Program to write the description of an MS
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
//# $Id: makevds.cc 13947 2009-09-07 06:23:03Z diepen $

#include <lofar_config.h>
#include <MS/VdsMaker.h>
#include<stdexcept>
#include <iostream>

using namespace std;


int main(int argc, const char* argv[])
{
  try {
    if (argc < 3  ||  argv[1][0] == '\0'  ||  argv[2][0] == '\0') {
      cout << "Run as:  makevds clusterdesc ms [msvds] [hostname] [writetimes]" << endl;
      cout << "  default vds name is <ms>.vds" << endl;
      cout << "  default host name is gethostname()" << endl;
      cout << "  default writetimes is false (0)" << endl;
      return 0;
    }
    string msvds;
    if (argc > 3) {
      msvds = argv[3];
    } else {
      // Use MSname.vds, but remove possible trailing slash.
      msvds = string(argv[2]);
      if (msvds[msvds.size()-1] == '/') {
        msvds = msvds.substr(0, msvds.size()-1);
      }
      msvds += ".vds";
    }
    string hostName;
    if (argc > 4) {
      hostName = argv[4];
    }
    bool writeTimes = false;
    if (argc > 5) {
      writeTimes = (argv[5][0]=='t' || argv[5][0]=='T' || argv[5][0]=='1');
    }
    LOFAR::VdsMaker::create (argv[2], msvds, argv[1], hostName, writeTimes);
  } catch (exception& x) {
    cout << "Unexpected expection: " << x.what() << endl;
    return 1;
  }
  return 0;
}
