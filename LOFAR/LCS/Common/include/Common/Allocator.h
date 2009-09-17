//# Allocator.h: Abstract base class for LOFAR memory (de)allocator
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
//# $Id: Allocator.h 7529 2006-02-01 08:15:11Z diepen $

#ifndef LOFAR_COMMON_ALLOCATOR_H
#define LOFAR_COMMON_ALLOCATOR_H

// \file
// Abstract base class for %LOFAR memory (de)allocator

#include <unistd.h>

namespace LOFAR {

  // \addtogroup Common
  // @{

  // This class abstracts the way in which memory is allocated and
  // deallocated. In this way it is possible to use different memory models
  // in a transparant way.
  // A virtual function mechanism is used because it must be possible
  // to make the choice at run-time.
  //
  // The normal allocation is on the heap, which is done by the concrete
  // class HeapAllocator.
  
  class Allocator
    {
    public:
      Allocator()
	{}
      
      virtual ~Allocator();
      
      // Clone the allocator.
      virtual Allocator* clone() const = 0;
      
      // Allocate memory.
      // It is possible to specify how the data need to be aligned
      // to a maximum of 256 bytes. The alignment must be a power of 2.
      virtual void* allocate (size_t nbytes, unsigned int alignment=0) = 0;
      
      // Deallocate memory.
      virtual void deallocate (void* data) = 0;
    };
  
  
  
  // This class is a concrete allocation of an Allocator.
  // It allocates and deallocates memory on the heap.
  
  class HeapAllocator: public Allocator
    {
    public:
      HeapAllocator()
	{}
      
      virtual ~HeapAllocator();
      
      // Clone the allocator.
      virtual HeapAllocator* clone() const;
      
      // Allocate memory.
      // It is possible to specify how the data need to be aligned
      // to a maximum of 256 bytes. The alignment must be a power of 2.
      virtual void* allocate (size_t nbytes, unsigned int alignment=0);
      
      // Deallocate memory.
      virtual void deallocate (void* data);
    };
  
  // @}

} // end namespace LOFAR

#endif
