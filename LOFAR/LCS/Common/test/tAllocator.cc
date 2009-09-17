//#  tAllocator.cc: test program for the Allocator class
//#
//#  Copyright (C) 2006
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
//#  $Id: tAllocator.cc 7529 2006-02-01 08:15:11Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

//# Includes
#include <Common/Allocator.h>
#include <Common/LofarLogger.h>

using namespace LOFAR;
using namespace std;

void checkalloc (HeapAllocator& allc, int align=0)
{
  char* ptr = (char*) allc.allocate (20, align);
  if (align > 0) {
    ASSERT ((ptr - (char*)0) % align == 0);
  }
  allc.deallocate (ptr);
}

int main(int /*argc*/, const char* argv[])
{
  INIT_LOGGER(argv[0]);
  try {
    HeapAllocator allc;
    checkalloc (allc);
    checkalloc (allc,1);
    checkalloc (allc,2);
    checkalloc (allc,4);
    checkalloc (allc,8);
    checkalloc (allc,16);
    checkalloc (allc,32);
    checkalloc (allc,64);
    checkalloc (allc,128);
    checkalloc (allc,256);
  } catch (Exception& e) {
    LOG_ERROR_STR(e);
    return 1;
  }
  LOG_INFO("Program terminated successfully");
  return 0;
}
