//#  tReadLine.cc: Test program for the readLine functions
//#
//#  Copyright (C) 2009
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
//#  $Id: tReadLine.cc 12536 2009-01-28 14:49:05Z diepen $

#include <lofar_config.h>
#include <Common/ReadLine.h>
#include <Common/lofar_iostream.h>
#include <Common/lofar_fstream.h>

using namespace LOFAR;

int main (int argc, char* argv[])
{
  // Note that GNU readline writes on stdout.
  // Therefore output is written into a file which is handled in tReadLine.run.
  ofstream ostr("tReadLine_tmp.stdout");
  string line;
  if (argc <= 1) {
    while (readLine(line)) {
      ostr << line << endl;
    }
  } else {
    while (readLineSkip(line, "> ", argv[1])) {
      ostr << line << endl;
    }
  }
  return 0;
}
