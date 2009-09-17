//#  lofar_iosfwd.h:
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
//#  $Id: lofar_iosfwd.h 10134 2007-04-23 09:57:59Z loose $

#ifndef LOFAR_COMMON_IOSFWD_H
#define LOFAR_COMMON_IOSFWD_H

// \file

#include <iosfwd>

namespace LOFAR
{
  using std::istream;
  using std::ostream;
  using std::iostream;
  using std::istringstream;
  using std::ostringstream;
  using std::stringstream;
  using std::ifstream;
  using std::ofstream;
  using std::fstream;
  using std::streamsize;
  using std::streampos;
  using std::streamoff;
}

#endif
