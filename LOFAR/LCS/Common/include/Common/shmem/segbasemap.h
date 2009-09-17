/*  segbasemap.h
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
 * $Id: segbasemap.h 7330 2005-12-27 10:02:33Z loose $
 *
 * $Log$
 * Revision 1.3  2005/10/04 11:22:50  cvs
 * %lofarcvsmv%: Moved from LOFAR/LCS/Common/src/shmem, creating new revision
 *
 * Revision 1.2  2005/02/10 10:40:02  loose
 * %[ER: 236]%
 *
 * Enhanced the documentation:
 * - Defined a number of groups within the package group "Common".
 * - Added (sub)grouping to several source files.
 * - Updated comment style to C++ // style.
 *
 * Revision 1.1  2003/08/21 11:20:33  diepen
 * Moved Common to LCS
 *
 * Revision 1.2  2002/05/22 13:32:55  wierenga
 * %[BugId: 4]%
 * Fix errors and warnings found by KAI compiler.
 *
 * Revision 1.1  2002/05/22 11:24:38  wierenga
 * %[BugId: 4]%
 * Moved shmem_alloc code from BaseSim/src/ShMem to Common/shmem.
 *
 * Revision 1.1  2002/05/15 14:47:57  wierenga
 * New version of TH_ShMem TranportHolder.
 * Semaphore implementation working properly now.
 * Great performance on dual-processor machine ~1000M per sec.
 *
 *
 */

#ifndef _SEGBASEMAP_H
#define _SEGBASEMAP_H

/** \file
 */

#ifdef __cplusplus
extern "C" {
#endif

void* shmbrk(int size);
void* shm_segbasemap_find(void* address);
void  shm_segbasemap_add (void* base);

#ifdef __cplusplus
}
#endif

#endif
