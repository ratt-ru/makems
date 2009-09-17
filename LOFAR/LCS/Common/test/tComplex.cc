//# tComplex.cc: Test program for complex performance
//#
//# Copyright (C) 2005
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
//# $Id: tComplex.cc 5570 2005-07-01 16:16:29Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/lofar_complex.h>
#include <complex>
#include <Common/Timer.h>

#if defined __SSE2__
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

using namespace std;
using namespace LOFAR;
using namespace LOFAR::TYPES;


inline complex<double> makedblcomplex(double re, double im)
  { return complex<double>(re,im); }
#define MAKEDBLCOMPLEX(re,im) complex<double>(re,im)


// Define a class holding the builtin complex type.
class mdcomplex
{
public:
  mdcomplex() : itsValue(makedcomplex(0,0)) {}
  mdcomplex(double re, double im) : itsValue(makedcomplex(re,im)) {}
  mdcomplex(dcomplex val) : itsValue(val) {}
  mdcomplex operator* (const mdcomplex that) {return mdcomplex(itsValue*that.itsValue);}
  friend mdcomplex conj (const mdcomplex that) {return mdcomplex(LOFAR::conj(that.itsValue));}
private:
  dcomplex itsValue;
};


// Check if the complex operations give correct results.
void check()
{
  {
    complex<double> c1(1,2), c2(3,4);
    cout << c1 << c2 << endl;
    cout << c1*c2 << c1+c2 << c1-c2 << conj(c1*c2) << c1/c2 << endl;
    cout << sin(c1) << endl;
  }
  {
    dcomplex c1a (makedcomplex(1,2));
    dcomplex c1b (makedcomplex(4,5));
    dcomplex c1c = makedcomplex(6,7);
    dcomplex c1d (makedcomplex(7,8));
    cout << c1a << c1b << c1c << c1d << endl;
    complex<double> cc1(1,2);
    complex<double> cc2(makedblcomplex(3,4));
    dcomplex c1 = *(dcomplex*)(&cc1);
    dcomplex c2 = *(dcomplex*)(&cc2);
    //    dcomplex c2 = 3+4i;
    cout << c1 << c2 << endl;
    dcomplex cc;
    cc = LOFAR::conj(c1*c2);
    cout << c1*c2 << c1+c2 << c1-c2 << cc << c1/c2 << endl;
    cout << sin(c1) << endl;

    u16complex u1a (makeu16complex(10,20));
    cout << u1a << endl;
  }
}

void* allocDCArray (double*& ptr, int nrdc)
{
  // Allocate an array of complex of doubles,
  // Take care it is aligned on 16 byte boundary (for the SSE instructions).
  ptr = new double[2*nrdc+1];
  double* ptr1 = ptr;
  // Make sure data is on 16-byte boundary.
  if ((ptrdiff_t)(ptr1) & 0x000F) ptr1++;
  return ptr1;
}

void* allocDCArray (double*& ptr, int nrdc, complex<double> v)
{
  double* ptr1 = static_cast<double*>(allocDCArray(ptr, nrdc));
  for (int i=0; i<2*nrdc; i+=2) {
    ptr1[i]   = real(v);
    ptr1[i+1] = imag(v);
  }
  return ptr1;
}

void freeDCArray (double* ptr)
{
  delete [] ptr;
}

void checkDCArray (const void* ptr, int nrdc, complex<double> v)
{
  const complex<double>* arr = static_cast<const complex<double>*>(ptr);
  for (int i=0; i<nrdc; ++i) {
    if (arr[i] != v) {
      cout << i << " found=" << arr[i] << " expected=" << v << endl;
    }
  }
}

int main (int argc, const char* argv[])
{
  check();
  int nriter = 1;
  int nrval = 1001;
  if (argc > 1) {
    istringstream istr(argv[1]);
    istr >> nriter;
  }
  if (argc > 2) {
    istringstream istr(argv[2]);
    istr >> nrval;
  }
  cout << "nriter=" << nriter << " nrval=" << nrval << endl;
  cout << ">>>" << endl;

  const complex<double> v1(1,2);
  const complex<double> v2(3,4);
  {
    // Test filling of std::complex using macro.
    complex<double>* arr1 = new complex<double>[nrval];
    {
      NSTimer timer("std::complex fi");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr1[i] = MAKEDBLCOMPLEX(i,i);
	}
      }
      timer.stop();
      timer.print (cout);
    }
    delete[] arr1;
  }
  {
    // Test filling of std::complex using inline function.
    complex<double>* arr1 = new complex<double>[nrval];
    {
      NSTimer timer("std::complex ff");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr1[i] = makedblcomplex (i,i);
	}
      }
      timer.stop();
      timer.print (cout);
    }
    delete[] arr1;
  }
  {
    // Test filling of builtin complex using macro.
    dcomplex* arr1 = new dcomplex[nrval];
    {
      NSTimer timer("dcomplex mfill  ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr1[i] = makedcomplex (i,i);
	}
      }
      timer.stop();
      timer.print (cout);
    }
    delete[] arr1;
  }
  {
    // Test filling of dcomplex (usually builtin) using inline function.
    dcomplex* arr1 = new dcomplex[nrval];
    {
      NSTimer timer("dcomplex ffill  ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr1[i] = makedcomplex (i,i);
	}
      }
      timer.stop();
      timer.print (cout);
    }
    delete[] arr1;
  }
  {
    // Test multiplication of std::complex.
    typedef complex<double> MyComplex;
    double *ptr1, *ptr2, *ptr3;
    MyComplex* arr1 = static_cast<MyComplex*>(allocDCArray(ptr1, nrval, v1));
    MyComplex* arr2 = static_cast<MyComplex*>(allocDCArray(ptr2, nrval, v2));
    MyComplex* arr3 = static_cast<MyComplex*>(allocDCArray(ptr3, nrval));
    {
      NSTimer timer("std::complex * ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3[i] = arr1[i] * arr2[i];
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*v2);
    }
    {
      NSTimer timer("std::complex *~");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3[i] = arr1[i] * conj(arr2[i]);
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*conj(v2));
    }
    freeDCArray (ptr1);
    freeDCArray (ptr2);
    freeDCArray (ptr3);
  }
  {
    // Test multiplication of a complex built on top of builtin complex..
    typedef mdcomplex MyComplex;
    double *ptr1, *ptr2, *ptr3;
    MyComplex* arr1 = static_cast<MyComplex*>(allocDCArray(ptr1, nrval, v1));
    MyComplex* arr2 = static_cast<MyComplex*>(allocDCArray(ptr2, nrval, v2));
    MyComplex* arr3 = static_cast<MyComplex*>(allocDCArray(ptr3, nrval));
    {
      NSTimer timer("mdcomplex *    ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3[i] = arr1[i] * arr2[i];
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*v2);
    }
//     {
//       NSTimer timer("mdcomplex *~   ");
//       timer.start();
//       for (int j=0; j<nriter; ++j) {
// 	for (int i=0; i<nrval; ++i) {
// 	  arr3[i] = arr1[i] * conj(arr2[i]));
// 	}
//       }
//       timer.stop();
//       timer.print (cout);
//     }
    freeDCArray (ptr1);
    freeDCArray (ptr2);
    freeDCArray (ptr3);
  }
#ifdef LOFAR_BUILTIN_COMPLEXFP
  {
    // Test multiplication of builtin complex.
    typedef __complex__ double MyComplex;
    double *ptr1, *ptr2, *ptr3;
    MyComplex* arr1 = static_cast<MyComplex*>(allocDCArray(ptr1, nrval, v1));
    MyComplex* arr2 = static_cast<MyComplex*>(allocDCArray(ptr2, nrval, v2));
    MyComplex* arr3 = static_cast<MyComplex*>(allocDCArray(ptr3, nrval));
    {
      NSTimer timer("_Complex *     ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3[i] = arr1[i] * arr2[i];
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*v2);
    }
    {
      NSTimer timer("_Complex *~    ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3[i] = arr1[i] * ~arr2[i];
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*conj(v2));
    }
    freeDCArray (ptr1);
    freeDCArray (ptr2);
    freeDCArray (ptr3);
  }
#endif
  {
    // Test multiplication of dcomplex.
    typedef dcomplex MyComplex;
    double *ptr1, *ptr2, *ptr3;
    MyComplex* arr1 = static_cast<MyComplex*>(allocDCArray(ptr1, nrval, v1));
    MyComplex* arr2 = static_cast<MyComplex*>(allocDCArray(ptr2, nrval, v2));
    MyComplex* arr3 = static_cast<MyComplex*>(allocDCArray(ptr3, nrval));
    {
      NSTimer timer("dcomplex *     ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3[i] = arr1[i] * arr2[i];
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*v2);
    }
    {
      NSTimer timer("dcomplex *~    ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3[i] = arr1[i] * LOFAR::conj(arr2[i]);
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*conj(v2));
    }
    freeDCArray (ptr1);
    freeDCArray (ptr2);
    freeDCArray (ptr3);
  }
  {
    double* arr1r = new double[nrval];
    double* arr1i = new double[nrval];
    double* arr2r = new double[nrval];
    double* arr2i = new double[nrval];
    double* arr3r = new double[nrval];
    double* arr3i = new double[nrval];
    for (int i=0; i<nrval; ++i) {
      arr1r[i] = real(v1);
      arr1i[i] = imag(v1);
      arr2r[i] = real(v2);
      arr2i[i] = imag(v2);
    }
    {
      NSTimer timer("real,imag *    ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3r[i] = arr1r[i] * arr2r[i] - arr1i[i] * arr2i[i];
	  arr3i[i] = arr1i[i] * arr2r[i] + arr1r[i] * arr2i[i];
	}
      }
      timer.stop();
      timer.print (cout);
      complex<double> v(arr3r[0], arr3i[0]);
      checkDCArray (&v, 1, v1*v2);
    }
    {
      NSTimer timer("real,imag *~   ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	for (int i=0; i<nrval; ++i) {
	  arr3r[i] = arr1r[i] * arr2r[i] + arr1i[i] * arr2i[i];
	  arr3i[i] = arr1i[i] * arr2r[i] - arr1r[i] * arr2i[i];
	}
      }
      timer.stop();
      timer.print (cout);
      complex<double> v(arr3r[0], arr3i[0]);
      checkDCArray (&v, 1, v1*conj(v2));
    }
    delete [] arr1r;
    delete [] arr1i;
    delete [] arr2r;
    delete [] arr2i;
    delete [] arr3r;
    delete [] arr3i;
  }
#if defined __SSE2__
  {
    typedef dcomplex MyComplex;
    double *ptr1, *ptr2, *ptr3;
    MyComplex* arr1 = static_cast<MyComplex*>(allocDCArray(ptr1, nrval, v1));
    MyComplex* arr2 = static_cast<MyComplex*>(allocDCArray(ptr2, nrval, v2));
    MyComplex* arr3 = static_cast<MyComplex*>(allocDCArray(ptr3, nrval));
    {
      NSTimer timer("_Complex * SSE ");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	__m128d *src1 = (__m128d *) arr1;
	__m128d *src2 = (__m128d *) arr2;
	__m128d *dest = (__m128d *) arr3;
	size_t  count = nrval - 1;
	for (size_t i = 0; i < count; i += 2) {
	  __m128d ar = _mm_unpacklo_pd(src1[i], src1[i+1]);
	  __m128d ai = _mm_unpackhi_pd(src1[i], src1[i+1]);
	  __m128d br = _mm_unpacklo_pd(src2[i], src2[i+1]);
	  __m128d bi = _mm_unpackhi_pd(src2[i], src2[i+1]);
	  __m128d cr = _mm_sub_pd(_mm_mul_pd(ar, br), _mm_mul_pd(ai, bi));
	  __m128d ci = _mm_add_pd(_mm_mul_pd(ar, bi), _mm_mul_pd(ai, br));
	  dest[i]    = _mm_unpacklo_pd(cr,ci);
	  dest[i+1]  = _mm_unpackhi_pd(cr,ci);
	}
	if ((count & 1) == 0) {         // uneven array length
	  arr3[count] = arr1[count] * arr2[count];
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*v2);
    }
    {
      NSTimer timer("_Complex *~ SSE");
      timer.start();
      for (int j=0; j<nriter; ++j) {
	__m128d *src1 = (__m128d *) arr1;
	__m128d *src2 = (__m128d *) arr2;
	__m128d *dest = (__m128d *) arr3;
	size_t  count = nrval - 1;
	for (size_t i = 0; i < count; i += 2) {
	  __m128d ar = _mm_unpacklo_pd(src1[i], src1[i+1]);
	  __m128d ai = _mm_unpackhi_pd(src1[i], src1[i+1]);
	  __m128d br = _mm_unpacklo_pd(src2[i], src2[i+1]);
	  __m128d bi = _mm_unpackhi_pd(src2[i], src2[i+1]);
	  __m128d cr = _mm_add_pd(_mm_mul_pd(ar, br), _mm_mul_pd(ai, bi));
	  __m128d ci = _mm_sub_pd(_mm_mul_pd(br, ai), _mm_mul_pd(bi, ar));
	  dest[i]    = _mm_unpacklo_pd(cr,ci);
	  dest[i+1]  = _mm_unpackhi_pd(cr,ci);
	}
	if ((count & 1) == 0) {         // uneven array length
	  arr3[count] = arr1[count] * conj(arr2[count]);
	}
      }
      timer.stop();
      timer.print (cout);
      checkDCArray (arr3, nrval, v1*conj(v2));
    }
    freeDCArray (ptr1);
    freeDCArray (ptr2);
    freeDCArray (ptr3);
  }
#endif

  cout << "<<<" << endl;
  return 0;
}
