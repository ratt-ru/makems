//#  tParameterSetPerf.cc: Performance test program for class ParameterSet
//#
//#  Copyright (C) 2008
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
//#  $Id: tParameterSetPerf.cc 12371 2008-12-23 13:18:31Z loose $

#include <lofar_config.h>
#include <Common/ParameterSet.h>
#include <Common/LofarLogger.h>
#include <Common/Timer.h>

using namespace LOFAR;

void test()
{
  // Time how long it takes to read in 1000 lines.
  NSTimer timer1;
  timer1.start();
  ParameterSet ps("tParameterSetPerf_tmp.parset");
  timer1.stop();
  timer1.print (cout);
  // Time how long it takes to get a vector out.
  // Do something with the vector to avoid that the compler optimizes it out.
  NSTimer timer2;
  timer2.start();
  int sz=0;
  for (uint i=0; i<1000; ++i) {
    vector<int32> vec = ps.getInt32Vector ("a0.b0.c0");
    sz += vec.size();
  }
  timer2.stop();
  timer2.print (cout);
  cout << sz << endl;
}

int main()
{
  try {
    INIT_LOGGER("tParameterSetPerf");
    test();
  } catch (std::exception& x) {
    cout << "Unexpected exception: " << x.what() << endl;
    return 1;
  }
  return 0;
}
