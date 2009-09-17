//# tTimer.cc: Test program for class NSTimer
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
//# $Id: tTimer.cc 10938 2008-02-05 10:48:41Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <cmath>
#include <iostream>
#include <unistd.h>
#include <Common/Timer.h>

double a = 1;

using namespace LOFAR;

int main()
{
    {
      NSTimer timer("second", true);

      timer.start();
      sleep(1);
      timer.stop();
    }
    class NSTimer timer("test");

    for (int i = 0; i < 100000; i ++) {
	timer.start();
	a = pow(a, a);
	timer.stop();
    }

    std::cout << timer;

    return 0;
}
