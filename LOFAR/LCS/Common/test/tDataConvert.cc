//# tDataConvert.cc: Test program for DataConvert functions
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
//# $Id: tDataConvert.cc 7491 2006-01-25 14:20:50Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>


#include <Common/DataConvert.h>
#include <Common/LofarLogger.h>
#include <iomanip>

using namespace LOFAR;

int main()
{
  try {
    INIT_LOGGER("tDataConvert");
    {
      // Swap 2 byte integer and check if correct.
      // Double swapping should give the original.
      uint16 v1 = 0xbecd;
      uint16 v2 = v1;
      ASSERT (byteSwap(byteSwap(v1)) == v2);
      byteSwap16 (&v2, &v1, 1);
      ASSERT (v2 == 0xcdbe);
    }
    {
      // Swap 4 byte integer and check if correct.
      // Double swapping should give the original.
      uint32 v1 = 0xbecd0256;
      uint32 v2 = v1;
      ASSERT (byteSwap(byteSwap(v1)) == v2);
      byteSwap32 (&v2, &v1, 1);
      ASSERT (v2 == 0x5602cdbe);
      byteSwap16 (&v2, &v1, 2);
      ASSERT (v2 == 0xcdbe5602);
    }
    {
      // Swap 8 byte integer and check if correct.
      // Double swapping should give the original.
      uint64 v1 = 0xbecd025613890574ULL;
      uint64 v2 = v1;
      ASSERT (byteSwap(byteSwap(v1)) == v2);
      byteSwap64 (&v2, &v1, 1);
      ASSERT (v2 == 0x740589135602cdbeULL);
      byteSwap32 (&v2, &v1, 2);
      ASSERT (v2 == 0x5602cdbe74058913ULL);
    }
    {
      // Swap 2 byte integers in a vector.
      // Double swapping should give the original.
      uint16 v1[3];
      v1[0]=0x0123; v1[1]=0x4567; v1[2]=0x89ab;
      uint16 v2[3];
      byteSwap16 (v2, v1, 3);
      ASSERT (v2[0] == 0x2301);
      ASSERT (v2[1] == 0x6745);
      ASSERT (v2[2] == 0xab89);
      byteSwap16 (v2, 3);
      ASSERT (v2[0] == v1[0]);
      ASSERT (v2[1] == v1[1]);
      ASSERT (v2[2] == v1[2]);
    }
    {
      // Swap 4 byte integers in a vector.
      // Double swapping should give the original.
      uint32 v1[3];
      v1[0]=0x01233210; v1[1]=0x45677654; v1[2]=0x89abba98;
      uint32 v2[3];
      byteSwap32 (v2, v1, 3);
      ASSERT (v2[0] == 0x10322301);
      ASSERT (v2[1] == 0x54766745);
      ASSERT (v2[2] == 0x98baab89);
      byteSwap32 (v2, 3);
      ASSERT (v2[0] == v1[0]);
      ASSERT (v2[1] == v1[1]);
      ASSERT (v2[2] == v1[2]);
    }
    {
      // Swap 8 byte integers in a vector.
      // Double swapping should give the original.
      uint64 v1[3];
      v1[0]=0x0123321045677654ULL; v1[1]=0x1234567887654321ULL;
      v1[2]=0x89abcdeffedcba98ULL;
      uint64 v2[3];
      byteSwap64 (v2, v1, 3);
      ASSERT (v2[0] == 0x5476674510322301ULL);
      ASSERT (v2[1] == 0x2143658778563412ULL);
      ASSERT (v2[2] == 0x98badcfeefcdab89ULL);
      byteSwap64 (v2, 3);
      ASSERT (v2[0] == v1[0]);
      ASSERT (v2[1] == v1[1]);
      ASSERT (v2[2] == v1[2]);
    }
    {
      // Swap (possibly) builtin complex numbers.
      // Double swapping should give the original.
      u16complex v[2];
      v[0] = makeu16complex(1, 3);
      v[1] = makeu16complex(22, 44);
      dataConvert (LittleEndian, v, 2);
      dataConvert (LittleEndian, v, 2);
      ASSERT (v[0] == makeu16complex(1, 3));
      ASSERT (v[1] == makeu16complex(22, 44));
    }
    {
      // Swap (possibly) builtin complex numbers.
      // Double swapping should give the original.
      fcomplex v[2];
      v[0] = makefcomplex(-1.2, 3.4);
      v[1] = makefcomplex(22.11, -44.33);
      dataConvert (LittleEndian, v, 2);
      dataConvert (LittleEndian, v, 2);
      ASSERT (v[0] == makefcomplex(-1.2, 3.4));
      ASSERT (v[1] == makefcomplex(22.11, -44.33));
    }
    {
      // Swap (possibly) builtin complex numbers.
      // Double swapping should give the original.
      dcomplex v[2];
      v[0] = makedcomplex(-1.2, 3.4);
      v[1] = makedcomplex(22.11, -44.33);
      dataConvert (LittleEndian, v, 2);
      dataConvert (LittleEndian, v, 2);
      ASSERT (v[0] == makedcomplex(-1.2, 3.4));
      ASSERT (v[1] == makedcomplex(22.11, -44.33));
    }
    {
      // Swap std complex numbers.
      // Double swapping should give the original.
      std::complex<uint16> v[2];
      v[0] = std::complex<uint16>(1, 3);
      v[1] = std::complex<uint16>(22, 44);
      dataConvert (BigEndian, v, 2);
      dataConvert (BigEndian, v, 2);
      ASSERT (v[0] == std::complex<uint16>(1, 3));
      ASSERT (v[1] == std::complex<uint16>(22, 44));
    }
    {
      // Swap std complex numbers.
      // Double swapping should give the original.
      std::complex<float> v[2];
      v[0] = std::complex<float>(-1.2, 3.4);
      v[1] = std::complex<float>(22.11, -44.33);
      dataConvert (BigEndian, v, 2);
      dataConvert (BigEndian, v, 2);
      ASSERT (v[0] == std::complex<float>(-1.2, 3.4));
      ASSERT (v[1] == std::complex<float>(22.11, -44.33));
    }
    {
      // Swap std complex numbers.
      // Double swapping should give the original.
      std::complex<double> v[2];
      v[0] = std::complex<double>(-1.2, 3.4);
      v[1] = std::complex<double>(22.11, -44.33);
      dataConvert (BigEndian, v, 2);
      dataConvert (BigEndian, v, 2);
      ASSERT (v[0] == std::complex<double>(-1.2, 3.4));
      ASSERT (v[1] == std::complex<double>(22.11, -44.33));
    }
    {
      bool bufb[50];
      bool bufb2[100];
      uchar bufc[20];
      for (int i=0; i<20; i++) {
	bufc[i] = 0;
      }
      for (int i=0; i<50; i++) {
	bufb[i] = (i%3 == 0);
      }
      int nrb = boolToBit (bufc, bufb, 50);
      for (int i=0; i<nrb; i++) {
	cout << ushort(bufc[i]) << ' ';
      }
      cout << endl;
      int nrb2 = bitToBool (bufb2, bufc, 50);
      ASSERT (nrb2 == nrb);
      for (int i=0; i<50; i++) {
	ASSERT (bufb2[i] == bufb[i]);
      }
      nrb2 = bitToBool (bufb2, bufc, 50-1, 1);
      ASSERT (nrb2 == nrb);
      for (int i=0; i<50-1; i++) {
	ASSERT (bufb2[i] == bufb[i+1]);
      }
      nrb2 = bitToBool (bufb2, bufc, 40-2, 2);
      ASSERT (nrb2 == nrb-2);
      for (int i=0; i<40-2; i++) {
	ASSERT (bufb2[i] == bufb[i+2]);
      }
      nrb = boolToBit (bufc, bufb, 50, 25);
      for (int i=0; i<nrb+3; i++) {
	cout << ushort(bufc[i]) << ' ';
      }
      cout << endl;
      nrb2 = bitToBool (bufb2, bufc, 50+25, 0);
      ASSERT (nrb2 == nrb+3);
      ASSERT (bufb2[0]);
      for (int i=0; i<50; i++) {
	ASSERT (bufb2[i+25] == bufb[i]);
      }
      bufc[0] = 8; bufc[1]=4;
      bitToBool (bufb, bufc, 16, 0);
      for (int i=0; i<16; i++) {
	cout << bufb[i] << ' ';
      }
      cout << endl;
	
    }
  } catch (std::exception& x) {
    std::cout << "Unexpected exception: " << x.what() << std::endl;
    return 1;
  }
  std::cout << "OK" << std::endl;
  return 0;
}
