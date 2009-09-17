//# DataConvert.h: Global functions to convert data values
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
//# $Id: DataConvert.h 13877 2009-08-31 09:20:19Z diepen $

#ifndef LOFAR_COMMON_DATACONVERT_H
#define LOFAR_COMMON_DATACONVERT_H

// \file
// Global functions to convert data values

// Global functions to convert data values

#include <Common/LofarTypes.h>
#include <Common/DataFormat.h>
#include <complex>

//# If std::complex is used for the complex types, their functions are
//# template specialisations, so they need template<>.
#ifndef LOFAR_BUILTIN_COMPLEXINT
# define LFDC_TMPL_INT template<>
# else
# define LFDC_TMPL_INT
#endif
#ifndef LOFAR_BUILTIN_COMPLEXFP
# define LFDC_TMPL_FP template<>
# else
# define LFDC_TMPL_FP
#endif

namespace LOFAR
{
// \ingroup Common
// \addtogroup DataConvert Data conversion functions
//
  // This file declares functions to convert data from one representation to
  // another, in particular from little endian to big endian (or vice-versa).
  //
  // The functions are defined in a general way for each standard data type,
  // so in principle every conceivable conversion could be done (for example,
  // from the old VAX format to IEEE format). However, currently byte swap
  // is the only conversion needed, so only that one is implemented.
  //
  // Furthermore it contains a function to convert bool values to bits
  // and vice-versa.
  // <group>
  
  // \name Convert the possible native types.
  // These functions can be used in templates.
  // <group>
  void dataConvert (DataFormat, char* inout, uint nrval);
  void dataConvert (DataFormat, int8* inout, uint nrval);
  void dataConvert (DataFormat, uint8* inout, uint nrval);
  void dataConvert (DataFormat, int16* inout, uint nrval);
  void dataConvert (DataFormat, uint16* inout, uint nrval);
  void dataConvert (DataFormat, int32* inout, uint nrval);
  void dataConvert (DataFormat, uint32* inout, uint nrval);
  void dataConvert (DataFormat, int64* inout, uint nrval);
  void dataConvert (DataFormat, uint64* inout, uint nrval);
  void dataConvert (DataFormat, float* inout, uint nrval);
  void dataConvert (DataFormat, double* inout, uint nrval);
  void dataConvert (DataFormat, i4complex *inout, uint nrval);
  template<class T> void dataConvert (DataFormat, std::complex<T>* inout,
				      uint nrval);
  LFDC_TMPL_INT void dataConvert (DataFormat, i16complex* inout, uint nrval);
  LFDC_TMPL_INT void dataConvert (DataFormat, u16complex* inout, uint nrval);
  LFDC_TMPL_FP  void dataConvert (DataFormat, fcomplex* inout, uint nrval);
  LFDC_TMPL_FP  void dataConvert (DataFormat, dcomplex* inout, uint nrval);
  // </group>

  // \name Convert char, int8, or uint8.
  // Currently it simply returns the input.
  // <group>
  char dataConvert (DataFormat, char in);
  int8 dataConvert (DataFormat, int8 in);
  uint8 dataConvert (DataFormat, uint8 in);
  // </group>

  // \name Convert 16 bit integers.
  // <group>
  int16 dataConvert (DataFormat, int16 in);
  uint16 dataConvert (DataFormat, uint16 in);
  void dataConvert16 (DataFormat, void* out, const void* in);
  void dataConvert16 (DataFormat, void* inout);
  void dataConvert16 (DataFormat, void* out, const void* in, uint nrval);
  void dataConvert16 (DataFormat, void* inout, uint nrval);
  // </group>

  // \name Convert 32 bit integers.
  // <group>
  int32 dataConvert (DataFormat, int32 in);
  uint32 dataConvert (DataFormat, uint32 in);
  void dataConvert32 (DataFormat, void* out, const void* in);
  void dataConvert32 (DataFormat, void* inout);
  void dataConvert32 (DataFormat, void* out, const void* in, uint nrval);
  void dataConvert32 (DataFormat, void* inout, uint nrval);
  // </group>

  // \name Convert 64 bit integers.
  // <group>
  int64 dataConvert (DataFormat, int64 in);
  uint64 dataConvert (DataFormat, uint64 in);
  void dataConvert64 (DataFormat, void* out, const void* in);
  void dataConvert64 (DataFormat, void* inout);
  void dataConvert64 (DataFormat, void* out, const void* in, uint nrval);
  void dataConvert64 (DataFormat, void* inout, uint nrval);
  // </group>

  // \name Convert 32 bit floats.
  // <group>
  void dataConvertFloat (DataFormat, void* out, const void* in);
  void dataConvertFloat (DataFormat, void* inout);
  void dataConvertFloat (DataFormat, void* out, const void* in, uint nrval);
  void dataConvertFloat (DataFormat, void* inout, uint nrval);
  // </group>

  // \name Convert 64 bit floats.
  // <group>
  void dataConvertDouble (DataFormat, void* out, const void* in);
  void dataConvertDouble (DataFormat, void* inout);
  void dataConvertDouble (DataFormat, void* out, const void* in, uint nrval);
  void dataConvertDouble (DataFormat, void* inout, uint nrval);
  // </group>

  // \name Swap bytes in 16 bit values.
  // <group>
  int16 byteSwap (int16 in);
  uint16 byteSwap (uint16 in);
  void byteSwap16 (void* out, const void* in);
  void byteSwap16 (void* inout);
  void byteSwap16 (void* out, const void* in, uint nrval);
  void byteSwap16 (void* inout, uint nrval);
  // </group>

  // \name Swap bytes in 32 bit values.
  // <group>
  int32 byteSwap (int32 in);
  uint32 byteSwap (uint32 in);
  void byteSwap32 (void* out, const void* in);
  void byteSwap32 (void* inout);
  void byteSwap32 (void* out, const void* in, uint nrval);
  void byteSwap32 (void* inout, uint nrval);
  // </group>

  // \name Swap bytes in 64 bit values.
  // <group>
  int64 byteSwap (int64 in);
  uint64 byteSwap (uint64 in);
  void byteSwap64 (void* out, const void* in);
  void byteSwap64 (void* inout);
  void byteSwap64 (void* out, const void* in, uint nrval);
  void byteSwap64 (void* inout, uint nrval);
  // </group>

  // Convert bools to bits.
  // startbit gives to first bit to use in the to buffer.
  // It returns the number of bytes used.
  uint boolToBit (void* to, const void* from, uint nvalues, uint startbit=0);

  // Convert bits to bools.
  // startbit gives to first bit to use in the from buffer.
  // It returns the number of bytes used.
  uint bitToBool (void* to, const void* from, uint nvalues, uint startbit=0);

  // </group>

} // end namespace LOFAR


namespace LOFAR
{
  template<class T>
  inline void dataConvert (DataFormat fmt, std::complex<T>* inout, uint nrval)
    { dataConvert (fmt, (T*)inout, 2*nrval); }

  inline void dataConvert (DataFormat, char*, uint)
    {}
  inline void dataConvert (DataFormat, int8*, uint)
    {}
  inline void dataConvert (DataFormat, uint8*, uint)
    {}
  inline void dataConvert (DataFormat fmt, int16* inout, uint nrval)
    { dataConvert16 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat fmt, uint16* inout, uint nrval)
    { dataConvert16 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat fmt, int32* inout, uint nrval)
    { dataConvert32 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat fmt, uint32* inout, uint nrval)
    { dataConvert32 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat fmt, int64* inout, uint nrval)
    { dataConvert64 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat fmt, uint64* inout, uint nrval)
    { dataConvert64 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat fmt, float* inout, uint nrval)
    { dataConvert32 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat fmt, double* inout, uint nrval)
    { dataConvert64 (fmt, inout, nrval); }
  inline void dataConvert (DataFormat, i4complex *, uint)
    {}
  inline void dataConvert (DataFormat fmt, std::complex<int16>* inout, uint nrval)
    { dataConvert16 (fmt, inout, 2*nrval); }
  LFDC_TMPL_INT inline void dataConvert (DataFormat fmt, i16complex* inout, uint nrval)
    { dataConvert16 (fmt, inout, 2*nrval); }
  LFDC_TMPL_INT inline void dataConvert (DataFormat fmt, u16complex* inout, uint nrval)
    { dataConvert16 (fmt, inout, 2*nrval); }
  LFDC_TMPL_FP inline void dataConvert (DataFormat fmt, fcomplex* inout, uint nrval)
    { dataConvertFloat (fmt, inout, 2*nrval); }
  LFDC_TMPL_FP inline void dataConvert (DataFormat fmt, dcomplex* inout, uint nrval)
    { dataConvertDouble (fmt, inout, 2*nrval); }

  inline char dataConvert (DataFormat, char in)
    { return in; }
  inline int8 dataConvert (DataFormat, int8 in)
    { return in; }
  inline uint8 dataConvert (DataFormat, uint8 in)
    { return in; }

  inline int16 dataConvert (DataFormat, int16 in)
    { return byteSwap (in); }
  inline uint16 dataConvert (DataFormat, uint16 in)
    { return byteSwap (in); }
  inline void dataConvert16 (DataFormat, void* out, const void* in)
    { byteSwap16 (out, in); }
  inline void dataConvert16 (DataFormat, void* inout)
    { byteSwap16 (inout); }
  inline void dataConvert16 (DataFormat, void* out, const void* in, uint nrval)
    { byteSwap16 (out, in, nrval); }
  inline void dataConvert16 (DataFormat, void* inout, uint nrval)
    { byteSwap16 (inout, nrval); }

  inline int32 dataConvert (DataFormat, int32 in)
    { return byteSwap (in); }
  inline uint32 dataConvert (DataFormat, uint32 in)
    { return byteSwap (in); }
  inline void dataConvert32 (DataFormat, void* out, const void* in)
    { byteSwap32 (out, in); }
  inline void dataConvert32 (DataFormat, void* inout)
    { byteSwap32 (inout); }
  inline void dataConvert32 (DataFormat, void* out, const void* in, uint nrval)
    { byteSwap32 (out, in, nrval); }
  inline void dataConvert32 (DataFormat, void* inout, uint nrval)
    { byteSwap32 (inout, nrval); }

  inline int64 dataConvert (DataFormat, int64 in)
    { return byteSwap (in); }
  inline uint64 dataConvert (DataFormat, uint64 in)
    { return byteSwap (in); }
  inline void dataConvert64 (DataFormat, void* out, const void* in)
    { byteSwap64 (out, in); }
  inline void dataConvert64 (DataFormat, void* inout)
    { byteSwap64 (inout); }
  inline void dataConvert64 (DataFormat, void* out, const void* in, uint nrval)
    { byteSwap64 (out, in, nrval); }
  inline void dataConvert64 (DataFormat, void* inout, uint nrval)
    { byteSwap64 (inout, nrval); }

  inline void dataConvertFloat (DataFormat, void* out, const void* in)
    { byteSwap32 (out, in); }
  inline void dataConvertFloat (DataFormat, void* inout)
    { byteSwap32 (inout); }
  inline void dataConvertFloat (DataFormat, void* out, const void* in, uint nrval)
    { byteSwap32 (out, in, nrval); }
  inline void dataConvertFloat (DataFormat, void* inout, uint nrval)
    { byteSwap32 (inout, nrval); }

  inline void dataConvertDouble (DataFormat, void* out, const void* in)
    { byteSwap64 (out, in); }
  inline void dataConvertDouble (DataFormat, void* inout)
    { byteSwap64 (inout); }
  inline void dataConvertDouble (DataFormat, void* out, const void* in, uint nrval)
    { byteSwap64 (out, in, nrval); }
  inline void dataConvertDouble (DataFormat, void* inout, uint nrval)
    { byteSwap64 (inout, nrval); }


  inline int16 byteSwap (int16 in)
  {
    int16 v;
    byteSwap16 (&v, &in);
    return v;
  }

  inline uint16 byteSwap (uint16 in)
  {
    uint16 v;
    byteSwap16 (&v, &in);
    return v;
  }

  inline void byteSwap16 (void* out, const void* in)
  {
    ((char*)(out))[0] = ((const char*)(in))[1];
    ((char*)(out))[1] = ((const char*)(in))[0];
  }

  inline void byteSwap16 (void* inout)
  {
    char v0 = ((const char*)(inout))[0];
    ((char*)(inout))[0] = ((const char*)(inout))[1];
    ((char*)(inout))[1] = v0;
  }


  inline int32 byteSwap (int32 in)
  {
    int32 v;
    byteSwap32 (&v, &in);
    return v;
  }

  inline uint32 byteSwap (uint32 in)
  {
    uint32 v;
    byteSwap32 (&v, &in);
    return v;
  }

  inline void byteSwap32 (void* out, const void* in)
  {
    ((char*)(out))[0] = ((const char*)(in))[3];
    ((char*)(out))[1] = ((const char*)(in))[2];
    ((char*)(out))[2] = ((const char*)(in))[1];
    ((char*)(out))[3] = ((const char*)(in))[0];
  }

  inline void byteSwap32 (void* inout)
  {
    char v0 = ((const char*)(inout))[0];
    char v1 = ((const char*)(inout))[1];
    ((char*)(inout))[0] = ((const char*)(inout))[3];
    ((char*)(inout))[1] = ((const char*)(inout))[2];
    ((char*)(inout))[2] = v1;
    ((char*)(inout))[3] = v0;
  }


  inline int64 byteSwap (int64 in)
  {
    int64 v;
    byteSwap64 (&v, &in);
    return v;
  }

  inline uint64 byteSwap (uint64 in)
  {
    uint64 v;
    byteSwap64 (&v, &in);
    return v;
  }

  inline void byteSwap64 (void* out, const void* in)
  {
    ((char*)(out))[0] = ((const char*)(in))[7];
    ((char*)(out))[1] = ((const char*)(in))[6];
    ((char*)(out))[2] = ((const char*)(in))[5];
    ((char*)(out))[3] = ((const char*)(in))[4];
    ((char*)(out))[4] = ((const char*)(in))[3];
    ((char*)(out))[5] = ((const char*)(in))[2];
    ((char*)(out))[6] = ((const char*)(in))[1];
    ((char*)(out))[7] = ((const char*)(in))[0];
  }

  inline void byteSwap64 (void* inout)
  {
    char v0 = ((const char*)(inout))[0];
    char v1 = ((const char*)(inout))[1];
    char v2 = ((const char*)(inout))[2];
    char v3 = ((const char*)(inout))[3];
    ((char*)(inout))[0] = ((const char*)(inout))[7];
    ((char*)(inout))[1] = ((const char*)(inout))[6];
    ((char*)(inout))[2] = ((const char*)(inout))[5];
    ((char*)(inout))[3] = ((const char*)(inout))[4];
    ((char*)(inout))[4] = v3;
    ((char*)(inout))[5] = v2;
    ((char*)(inout))[6] = v1;
    ((char*)(inout))[7] = v0;
  }

} // end namespace LOFAR


#undef LFDC_TMPL_FP
#undef LFDC_TMPL_INT

#endif
