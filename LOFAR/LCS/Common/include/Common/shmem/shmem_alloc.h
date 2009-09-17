/*  shmem_alloc.h
 *
 *  Copyright (C) 2002
 *  ASTRON (Netherlands Foundation for Research in Astronomy)
 *  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: shmem_alloc.h 7330 2005-12-27 10:02:33Z loose $
 */

#ifndef LOFAR_COMMON_SHMEM_ALLOC_H_
#define LOFAR_COMMON_SHMEM_ALLOC_H_

/** \file
 */

#include <Common/shmem/dlmalloc.h>
#include <stddef.h>

#ifdef HAVE_ATOMIC_H

// use atomic operations that are also used in linux kernel
// make sure it also works on SMP machines
#define __SMP__
#include <asm/atomic.h>

#else

#include <sys/types.h>
#include <unistd.h>

#endif

/**
  \ingroup Common
  \addtogroup shmem
  @{
*/

#define shmem_calloc               dlcalloc
#define shmem_free                 dlfree
#define shmem_cfree                dlcfree
#define shmem_malloc               dlmalloc
#define shmem_memalign             dlmemalign
#define shmem_realloc              dlrealloc
#define shmem_valloc               dlvalloc
#define shmem_pvalloc              dlpvalloc
#define shmem_mallinfo             dlmallinfo
#define shmem_mallopt              dlmallopt
#define shmem_malloc_trim          dlmalloc_trim
#define shmem_malloc_stats         dlmalloc_stats
#define shmem_usable_size          dlmalloc_usable_size
#define shmem_independent_calloc   dlindependent_calloc
#define shmem_independent_comalloc dlindependent_comalloc

#ifdef __cplusplus
extern "C"
{
#endif

/** \name Shared memory operations */
/** @{ */
void   shmem_init(void);
int    shmem_id(void* address);
size_t shmem_offset(void* address);
void*  shmem_connect(int shmid, size_t offset);
void   shmem_disconnect(void* segment, size_t offset);
/** @} */

/** Semaphore typedef */
#ifdef HAVE_ATOMIC_H
typedef atomic_t shmem_cond_t;
#else
typedef struct
{
  pid_t owner;
  bool  thetry[2];
  int   turn;
  int   count;
} shmem_cond_t;
#endif

/** \name Semaphore operations */
/** @{ */
void shmem_cond_init(volatile shmem_cond_t* condition);
void shmem_cond_signal(volatile shmem_cond_t* condition);
void shmem_cond_wait(volatile shmem_cond_t* condition);
/** @} */

/** @} */ /* end doxygen group shmem */

#ifdef __cplusplus
}
#endif
  
#endif
