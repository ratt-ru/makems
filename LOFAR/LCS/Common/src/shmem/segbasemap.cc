//#  segbasemap.cc
//#
//#  Copyright (C) 2002-2004
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
//#  $Id: segbasemap.cc 11748 2008-09-16 11:03:51Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/shmem/segbasemap.h>
#include <Common/lofar_map.h>
#include <stdio.h>
#include <stdlib.h>

using LOFAR::map;

/* sorted list of segment base addresses */
static map<void*, void*> segbase;

/**
 * ~shm_segbasemap_find
 *
 * Find the base address of the segment in which this address lies
 *
 */
void* shm_segbasemap_find(void* address)
{
  map<void*, void*>::const_iterator it;
  void* prev_base_address = 0;
  void* base_address = 0;

  if (segbase.begin() == segbase.end())
  {
    fprintf(stderr, "shm_segbasemap_find: map empty!");
    exit(1);
  }

  for (it = segbase.begin(); it != segbase.end(); ++it)
  {
    if (address < it->first) break;
    prev_base_address = it->first;
    base_address = prev_base_address;
  }

  return base_address;
}

/**
 * ~shm_segbasemap_add
 *
 * Add the segment base address to the segbase map.
 */
void shm_segbasemap_add(void* base)
{
  segbase[base] = base;
}

