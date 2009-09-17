//# lofar_datetime.h: namespace wrapper for Boost.Date_Time Posix classes
//#
//#  Copyright (C) 2002
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
//#  $Id: lofar_datetime.h 11381 2008-06-23 15:55:25Z loose $

#ifndef LOFAR_COMMON_DATETIME_H
#define LOFAR_COMMON_DATETIME_H

// \file
// namespace wrapper for Boost.Date_Time Posix classes

#if !defined(HAVE_BOOST)
#error Boost.Date_Time is required.
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <time.h>

namespace LOFAR
{
  using namespace boost::posix_time;

	// there is no function in boost to convert a ptime to a time_t.
	inline time_t	to_time_t(ptime aPtime) {
		struct tm 	stm = to_tm(aPtime);
		return (mktime(&stm));
	}
}

#endif
