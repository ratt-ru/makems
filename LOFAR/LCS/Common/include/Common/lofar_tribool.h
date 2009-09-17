//#  lofar_tribool.h: namespace wrapper for Boost.Tribool
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
//#  $Id: lofar_tribool.h 11381 2008-06-23 15:55:25Z loose $

#ifndef LOFAR_COMMON_TRIBOOL_H
#define LOFAR_COMMON_TRIBOOL_H

// \file
// namespace wrapper for Boost.Tribool

#if !defined(HAVE_BOOST)
#error Boost.Tribool is required.
#endif

#include <boost/logic/tribool.hpp>

namespace LOFAR
{
  using boost::logic::tribool;
  using boost::logic::indeterminate;
}

#endif
