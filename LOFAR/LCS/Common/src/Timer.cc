//# Timer.cc: Accurate timer
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
//# $Id: Timer.cc 12871 2009-03-12 12:54:57Z gels $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>

#include <Common/PrettyUnits.h>
#include <Common/Timer.h>


using namespace std;

namespace LOFAR {


double NSTimer::CPU_speed_in_MHz = NSTimer::get_CPU_speed_in_MHz();


double NSTimer::get_CPU_speed_in_MHz()
{
    // first a few sanity checks
    assert(sizeof(int) == 4);
    assert(sizeof(long long) == 8);

#if (defined __linux__ || defined __blrts__) && \
    (defined __i386__ || defined __x86_64__ || defined __ia64__ || defined __PPC__) && \
    (defined __GNUC__ || defined __INTEL_COMPILER || defined __PATHSCALE__ || defined __xlC__)
    ifstream infile("/proc/cpuinfo");
    char     buffer[256], *colon;

    while (infile.good()) {
	infile.getline(buffer, 256);

#if defined __PPC__
	if (strcmp("cpu\t\t: 450 Blue Gene/P DD2", buffer) == 0)
	    return 850.0;

	if (strcmp("machine\t\t: Blue Gene", buffer) == 0)
	    return 700.0;

	if (strncmp("timebase", buffer, 8) == 0 && (colon = strchr(buffer, ':')) != 0)
	    return atof(colon + 2) / 1e6;
#else
	if (strncmp("cpu MHz", buffer, 7) == 0 && (colon = strchr(buffer, ':')) != 0)
	    return atof(colon + 2);
#endif
    }

    return 0.0;
#else
#warning unsupported architecture
    return 0.0;
#endif
}


double NSTimer::getElapsed() const
{
  double time = total_time / 1e6;
  if (CPU_speed_in_MHz > 0) {
    time /= CPU_speed_in_MHz;
  }
  return time;
}


ostream &NSTimer::print(ostream &str) const
{
    if (itsName.empty()) {
        str << "timer: ";
    } else {
        str << left << setw(25) << itsName << ": " << right;
    }
    if (count == 0) {
	str << "not used"; 
	if (!log_on_destruction)
	  str << endl;
    } else {
        double total = static_cast<double>(total_time);
	if (CPU_speed_in_MHz == 0) {
	    str << "avg = " << total / static_cast<double>(count);
	    str << ", total = " << total_time << " cycles";
	} else {
	    total /= 1e6 * CPU_speed_in_MHz;
	    str << "avg = " << PrettyTime(total / static_cast<double>(count))
		<< ", total = " << PrettyTime(total);
	}
	str << ", count = " << setw(9) << count;
	if (!log_on_destruction)
	  str << endl;
    }
    return str;
}


}  // end namespace LOFAR
