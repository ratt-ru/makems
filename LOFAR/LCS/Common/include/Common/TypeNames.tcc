//# TypeNames.tcc: Return a string giving the type name to be stored in blobs
//#
//# Copyright (C) 2003
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
//# $Id: TypeNames.tcc 13877 2009-08-31 09:20:19Z diepen $


#ifndef COMMON_TYPENAMES_TCC
#define COMMON_TYPENAMES_TCC

//# Includes
#include <Common/TypeNames.h>

namespace LOFAR
{
  template<typename T>
  const std::string& typeName (T const* const*)
    {
      static std::string str ("array<" + typeName((const T*)0) + ">");
      return str;
    }
}

#endif
