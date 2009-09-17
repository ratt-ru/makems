//# TypeNames.h: Return a string giving the type name to be stored in blobs
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
//# $Id: TypeNames.h 13877 2009-08-31 09:20:19Z diepen $

#ifndef LOFAR_COMMON_TYPENAMES_H
#define LOFAR_COMMON_TYPENAMES_H

// \file
// Return a string giving the type name to be stored in blobs

//# Includes
#include <Common/LofarTypes.h>
#include <Common/lofar_complex.h>
#include <complex>
#include <string>

namespace LOFAR
{
  // \addtogroup TypeNames
  //
  // These global functions return the name of the basic types.
  // They are meant to get the full id of a templated class when such an
  // object is stored in a blob.
  // As much as possible std::complex and builtin complex types get the same
  // name, so they can be read back from a blob in both ways.
  // <group>

  const std::string& typeName (const void*);
  const std::string& typeName (const bool*);
  const std::string& typeName (const char*);
  const std::string& typeName (const int8*);
  const std::string& typeName (const uint8*);
  const std::string& typeName (const int16*);
  const std::string& typeName (const uint16*);
  const std::string& typeName (const int32*);
  const std::string& typeName (const uint32*);
  const std::string& typeName (const int64*);
  const std::string& typeName (const uint64*);
  const std::string& typeName (const float*);
  const std::string& typeName (const double*);
  const std::string& typeName (const i4complex*);
  const std::string& typeName (const i16complex*);
  const std::string& typeName (const u16complex*);
  const std::string& typeName (const fcomplex*);
  const std::string& typeName (const dcomplex*);
#ifdef LOFAR_BUILTIN_COMPLEXINT
  const std::string& typeName (const std::complex<int16>*);
  const std::string& typeName (const std::complex<uint16>*);
#endif
#ifdef LOFAR_BUILTIN_COMPLEXFP
  const std::string& typeName (const std::complex<float>*);
  const std::string& typeName (const std::complex<double>*);
#endif
  template<typename T> const std::string& typeName (T const* const*);

// </group>

}

// Include templated implementations.
#include <Common/TypeNames.tcc>


#endif
