//# DataConvert.cc: Global functions to convert data values
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
//# $Id: DataConvert.cc 7491 2006-01-25 14:20:50Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/DataConvert.h>

void LOFAR::byteSwap16 (void* val, uint nrval)
{
  char* v = (char*)val;
  for (uint i=0; i<nrval; i++) {
    LOFAR::byteSwap16 (v);
    v += 2;
  }
}

void LOFAR::byteSwap16 (void* out, const void* in, uint nrval)
{
  char* vout = (char*)out;
  const char* vin = (const char*)in;
  for (uint i=0; i<nrval; i++) {
    LOFAR::byteSwap16 (vout, vin);
    vout += 2;
    vin += 2;
  }
}

void LOFAR::byteSwap32 (void* val, uint nrval)
{
  char* v = (char*)val;
  for (uint i=0; i<nrval; i++) {
    LOFAR::byteSwap32 (v);
    v += 4;
  }
}

void LOFAR::byteSwap32 (void* out, const void* in, uint nrval)
{
  char* vout = (char*)out;
  const char* vin = (const char*)in;
  for (uint i=0; i<nrval; i++) {
    LOFAR::byteSwap32 (vout, vin);
    vout += 4;
    vin += 4;
  }
}

void LOFAR::byteSwap64 (void* val, uint nrval)
{
  char* v = (char*)val;
  for (uint i=0; i<nrval; i++) {
    LOFAR::byteSwap64 (v);
    v += 8;
  }
}

void LOFAR::byteSwap64 (void* out, const void* in, uint nrval)
{
  char* vout = (char*)out;
  const char* vin = (const char*)in;
  for (uint i=0; i<nrval; i++) {
    LOFAR::byteSwap64 (vout, vin);
    vout += 8;
    vin += 8;
  }
}

uint LOFAR::boolToBit (void* to, const void* from, uint nvalues, uint startbit)
{
  if (nvalues == 0) {
    return 0;
  }
  const bool* data = (const bool*)from;
  unsigned char* bits = (unsigned char*)to + startbit/8;
  startbit %= 8;
  //# Fill as many bytes as needed.
  uint nbytes = (nvalues + startbit + 7) / 8;
  uint i,j;
  uint index = 0;
  {
    unsigned char mask = 1;
    mask <<= startbit;
    unsigned char& ch = bits[0];
    //# Take care of correct number of bits in first byte.
    uint nbits = (nvalues-index < 8-startbit  ?  nvalues-index : 8-startbit);
    for (j=0; j<nbits; j++) {
      if (data[index++]) {
	ch |= mask;
      } else {
	ch &= ~mask;
      }
      mask <<= 1;
    }
  }
  for (i=1; i<nbytes; ++i) {
    unsigned char mask = 1;
    unsigned char& ch = bits[i];
    ch = 0;
    //# Take care of correct number of bits in last byte.
    uint nbits = (nvalues-index < 8  ?  nvalues-index : 8);
    for (j=0; j<nbits; j++) {
      if (data[index++]) {
	ch |= mask;
      }
      mask <<= 1;
    }
  }
  return nbytes;
}

uint LOFAR::bitToBool (void* to, const void* from, uint nvalues, uint startbit)
{
  bool* data = (bool*)to;
  const unsigned char* bits = (const unsigned char*)from + startbit/8;
  startbit %= 8;
  //# Fill as many bytes as needed.
  uint nbytes = (nvalues + startbit + 7) / 8;
  uint i,j;
  uint index = 0;
  {
    unsigned char mask = 1;
    mask <<= startbit;
    const unsigned char ch = bits[0];
    //# Take care of correct number of bits in first byte.
    uint nbits = (nvalues-index < 8-startbit  ?  nvalues-index : 8-startbit);
    for (j=0; j<nbits; j++) {
      data[index++] = ((ch & mask) != 0);
      mask <<= 1;
    }
  }
  for (i=1; i<nbytes; ++i) {
    unsigned char mask = 1;
    const unsigned char ch = bits[i];
    //# Take care of correct number of bits in last byte.
    uint nbits = (nvalues-index < 8  ?  nvalues-index : 8);
    for (j=0; j<nbits; j++) {
      data[index++] = ((ch & mask) != 0);
      mask <<= 1;
    }
  }
  return nbytes;
}
