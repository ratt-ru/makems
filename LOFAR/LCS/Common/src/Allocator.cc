//# Allocator.cc: Abstract base class for LOFAR memory (de)allocator
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
//# $Id: Allocator.cc 9742 2006-12-15 10:01:15Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/LofarTypes.h>
#include <Common/Allocator.h>
#include <Common/LofarLogger.h>

namespace LOFAR
{

  Allocator::~Allocator()
  {}


  HeapAllocator::~HeapAllocator()
  {}
  
  HeapAllocator* HeapAllocator::clone() const
  {
    return new HeapAllocator();
  }

  void* HeapAllocator::allocate (size_t nbytes, unsigned int alignment)
  {
    // Make sure alignment is power of 2.
    int align = 1;
    if (alignment > 0) {
      align = alignment;
      ASSERT (align <= 256  &&  (align & (align-1)) == 0);
    }
    // Allocate some extra bytes to be sure that the data can be aligned
    // correctly and that the nr of extra bytes can be stored in the byte
    // just before the returned buffer.
    // Usually allocation is always done on a multiple of 8 bytes, but
    // with valgrind that is not the case. So be prepared for everything.
    // Also allocate some extra bytes at the end to ensure that the special
    // Connection class can send multiple of N bytes to optimize performance.
    // The alignment value is used for N.
    uchar* data = new uchar[nbytes + 2*align];
    // Align the buffer as needed.
    ptrdiff_t ptr = (ptrdiff_t(data) & ~(align-1)) + align;
    uchar* newbuf = (uchar*)ptr;
    ptrdiff_t ptrd = newbuf-data;
    DBGASSERT (ptrd > 0  &&  ptrd <= align);
    // Store the nr of bytes 'shifted'. This nr is > 0, so subtract 1
    // to be sure that 256 byte alignment also fits.
    *(newbuf-1) = ptrd-1;
    return newbuf;
  }

  void HeapAllocator::deallocate (void* data)
  {
    if (data != 0) {
      uchar* buf = (uchar*)data;
      buf -= *(buf-1) + 1;
      delete [] buf;
    }
  }

} // end namespace LOFAR
