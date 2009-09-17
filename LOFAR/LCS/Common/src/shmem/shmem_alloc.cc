/*  shmem_alloc.cc
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
 * $Id: shmem_alloc.cc 5156 2005-05-23 07:57:17Z loose $
 */
 
//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/shmem/shmem_alloc.h>
#include <Common/shmem/segbasemap.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <Common/lofar_map.h>

using LOFAR::map;

// needs to be defined
union semun {
    int val;
};

/* definitions */
#define SHMID_REGISTRY_INITIAL_SIZE 32
#define MORECORE_FAILURE (-1)

/* local typedefs */

typedef struct
{
    int   id;
    char* segment_address;
    int   size;
} t_shmem_chunk;

typedef struct
{
    int            used;
    int            size;
    t_shmem_chunk* chunks;
} t_shmid_registry;

typedef struct
{
  int shmid;
} t_shmem_segment_header;

/* module private variables */
static t_shmid_registry shmid_registry = { 0, 0, NULL };
static pid_t shmem_mypid = 0;

/* segment attachment cache */
static map<int, char*> segcache;

/* local prototypes */
static void  shmid_registry_add(t_shmid_registry* sr,
				int id, void* segment_address, int size);
static void* shmid_registry_get_next_address(t_shmid_registry* sr);
static void  shmid_registry_clean(void);
static void  shmem_signal_handler(int sig);

/**
 * ~shmbrk
 *
 * Replacement of sbrk to allocate a chunk of shared memory.
 *
 */
void* shmbrk(int size)
{
    int shmid;
    char* segment_address = (char*)MORECORE_FAILURE;
    t_shmem_segment_header* hdr = (t_shmem_segment_header*)NULL;

    assert(size >= 0);

    if (size > 0)
    {
	/* get a shared memory segment of the requested size */
	shmid = shmget(IPC_PRIVATE, size, SHM_R | SHM_W | IPC_CREAT | IPC_EXCL);

	if (shmid != -1)
	{
	    /* attach to the new segment */
	    if ((segment_address = (char*)shmat(shmid, 0, 0)) == (char*)-1)
	    {
		/* could not attach, remove segment, ignore return value */
		(void)shmctl(shmid, IPC_RMID, 0);
		segment_address = (char*)MORECORE_FAILURE;
	    }
	    else
	    {
		/* add to the shmid_registry */
	        shm_segbasemap_add(segment_address);
		shmid_registry_add(&shmid_registry,
				   shmid, segment_address, size);
	    }
	}

	if ((void*)MORECORE_FAILURE == segment_address)
	{
	    perror("shmbrk");
	}
	else
	{
	    /*
	     * Put shmid in first four bytes of segment and
	     * increment address with 2 * sizeof(size_t) for correct
	     * alignment although this is more than needed. This is
	     * needed to be able to find out in which segment
	     * allocation has occured.
	     */
	    hdr = (t_shmem_segment_header*)segment_address;
	    hdr->shmid = shmid;

	    assert(sizeof(t_shmem_segment_header) <= 2*sizeof(size_t));
	    segment_address += 2*sizeof(size_t);
	}
    }
    else if (size == 0)
    {
	/* If size == 0 then return address just after previously
	 * returned address. dlmalloc uses this to determine size
	 * of previously allocated chunk.
	 */
	segment_address = (char*)shmid_registry_get_next_address(&shmid_registry);
    }

#if 0
    printf("shmbrk(%d) = %p\n", size, segment_address);
#endif
    
    return (void*)segment_address;
}

/**
 * ~shmem_init
 *
 * Initialize shmid_registry
 *
 */
void shmem_init(void)
{
    shmid_registry.used   = 0;
    shmid_registry.size   = SHMID_REGISTRY_INITIAL_SIZE;
    shmid_registry.chunks =
      (t_shmem_chunk*)malloc(SHMID_REGISTRY_INITIAL_SIZE*sizeof(t_shmem_chunk));
    
    atexit(&shmid_registry_clean);

    signal(SIGINT,  shmem_signal_handler);
    signal(SIGQUIT, shmem_signal_handler);
    signal(SIGSEGV, shmem_signal_handler);

    /* remember my pid */
    shmem_mypid = getpid();
}

/**
 * ~shmem_id
 *
 * Obtain the shmid for the allocated block of memory at
 * address. This is done by first finding the base address
 * of the shared memory segment in which this block is allocated
 * and then finding the shmid stored at that address.
 *
 */
int shmem_id(void* address)
{
    t_shmem_segment_header* hdrptr = NULL;
    char*  segment_address_ptr     = NULL;
    
    assert(NULL != address);

    segment_address_ptr = (char*)address - (2*sizeof(size_t));
    hdrptr = (t_shmem_segment_header*)(*(void**)segment_address_ptr);
    
    return hdrptr->shmid;
}

/**
 * ~shmem_offset
 *
 * Get offset of this malloced block within the segment
 * in which it is allocated.
 *
 */
size_t shmem_offset(void* address)
{
    char* segment_address_ptr = NULL;
    
    assert(NULL != address);
    
    segment_address_ptr = (char*)address - (2*sizeof(size_t));
    
    return ((char*)address - (char*)(*(void**)segment_address_ptr));
}

/**
 * ~shmem_connect
 *
 * Connect to an existing shared memory segment at
 * a specific offset.
 *
 */
void* shmem_connect(int shmid, size_t offset)
{
    char* segment = NULL;

    // find in cache
    if (segcache.end() != segcache.find(shmid))
    {
        // found!
        segment = segcache[shmid];
    }
    else
    {
	if ((segment = (char*)shmat(shmid, 0, SHM_R | SHM_W)) == (void*)-1)
	{
	    perror("shmem_connect");
	    segment = NULL;
	}
	else
	{
	    // add to cache
	    segcache[shmid] = segment;
	}
    }

    return (segment?segment+offset:segment);
}

/**
 * ~shmem_disconnect
 *
 * Disconnect from shared memory segment.
 *
 */
void shmem_disconnect(void* segment, size_t offset)
{
    if (shmdt((char*)segment - offset) < 0)
    {
        perror("shmem_disconnect");
    }
}

/**
 * ~shmid_registry_add
 *
 * Add a shmid to the shmid registry which keeps track of all
 * shmid's to allow cleanup at program exit.
 *
 */
static void shmid_registry_add(t_shmid_registry* sr,
			       int id, void* segment_address, int size)
{
    assert((NULL != sr) && (sr->size > 0));
    if (sr->used >= sr->size)
    {
	/* need to enlarge the registry, lets double size */
	sr->size *= 2;
	sr->chunks = (t_shmem_chunk*)realloc(sr->chunks,
					     sr->size*sizeof(t_shmem_chunk));
    }
    
    sr->chunks[sr->used].id              = id;
    sr->chunks[sr->used].segment_address = (char*)segment_address;
    sr->chunks[sr->used].size            = size;
    sr->used++;
}

/**
 * ~shmid_registry_get_next_address
 *
 * return base address of current chunk incremented by size
 *
 */
static void* shmid_registry_get_next_address(t_shmid_registry* sr)
{
    char* address = (char*)MORECORE_FAILURE;

    if (NULL != sr->chunks)
    {
	address = (  sr->chunks[sr->used-1].segment_address
		   + sr->chunks[sr->used-1].size);
    }

    return (void*)address;
}

/**
 * ~shmid_registry_clean
 *
 * Clean up all shared memory segments that have been allocated.
 */
static void shmid_registry_clean(void)
{
    int i;
    t_shmem_chunk* chunks = shmid_registry.chunks;

    if (NULL == chunks) return;

    for (i = 0; i < shmid_registry.used; i++)
    {
	if (shmctl(chunks[i].id, IPC_RMID, 0) == -1)
	{
	    perror("IPC_RMID failure");
	}
    }

    shmid_registry.used = 0;
    shmid_registry.size = 0;
    free(shmid_registry.chunks);
    shmid_registry.chunks = NULL;
}

/**
 * ~shmem_signal_handler
 *
 * This function is called when a signal is received
 * and the shared memory segments need to be cleaned up.
 *
 */
static void shmem_signal_handler(int sig)
{
    // keep compiler happy
    sig = sig;

    shmid_registry_clean();
    exit(1);
}

#ifndef HAVE_ATOMIC_H
/**
 * ~_shmem_lock
 *
 * Mutex lock routine needed in shmem_cond functions.
 * (Gary Peterson algorithm 1979)
 *
 **/
static void _shmem_lock(volatile shmem_cond_t* condition)
{
    if (condition->owner == shmem_mypid)
    {
      condition->thetry[0] = true;
      condition->turn = 0;
      while ((condition->turn == 0) && condition->thetry[1]) sched_yield();
    }
    else
    {
      condition->thetry[1] = true;
      condition->turn = 1;
      while ((condition->turn == 1) && condition->thetry[0]) sched_yield();
    }
}

/**
 * ~_shmem_unlock
 *
 * Mutex unlock routine needed in shmem_cond functions.
 * (Gary Peterson algorithm 1979)
 *
 **/
static void _shmem_unlock(volatile shmem_cond_t* condition)
{
    if (condition->owner == shmem_mypid)
    {
      condition->thetry[0] = false;
    }
    else
    {
      condition->thetry[1] = false;
    }
}
#endif

/**
 * ~shmem_cond_init
 *
 * Initialize a semaphore (locked)
 *
 **/
void shmem_cond_init(volatile shmem_cond_t* condition)
{
#ifdef HAVE_ATOMIC_H
    atomic_set(condition, 0);
#else
    condition->owner  = shmem_mypid;
    condition->thetry[0] = false;
    condition->thetry[1] = false;
    condition->turn   = -1;
    
    condition->count = 0;
#endif
}

/**
 * ~shmem_cond_signal
 *
 * Signal on the semaphore (unlock)
 *
 */
void shmem_cond_signal(volatile shmem_cond_t* condition)
{
#ifdef HAVE_ATOMIC_H
    atomic_inc(condition);
#else
    _shmem_lock(condition);
    condition->count++;
    _shmem_unlock(condition);
#endif
}

/**
 * ~shmem_cond_wait
 *
 * Wait on a semaphore (until unlocked)
 *
 */
void shmem_cond_wait(volatile shmem_cond_t* condition)
{
#ifdef HAVE_ATOMIC_H
    while (atomic_read(condition) <= 0) sched_yield();
    atomic_dec(condition);
#else
    int count = 1;
    do {
      if (count <= 0) sched_yield();
      _shmem_lock(condition);
      count = condition->count;
      _shmem_unlock(condition);
    } while (count <= 0);
    
    _shmem_lock(condition);
    condition->count--;
    _shmem_unlock(condition);
#endif
}

