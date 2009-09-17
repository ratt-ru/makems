//# Semaphore.h: semaphore implementation on top of pthreads
//#
//# Copyright (C) 2006
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
//# $Id: Semaphore.h 13127 2009-04-20 07:23:45Z romein $ 

#ifndef  LOFAR_LCS_COMMON_SEMAPHORE_H
#define  LOFAR_LCS_COMMON_SEMAPHORE_H

#if 1 || defined HAVE_THREADS // broken in build environment

#include <pthread.h>
#include <boost/noncopyable.hpp>
 
class Semaphore: boost::noncopyable
{
  public:
    Semaphore(unsigned level = 0);
    ~Semaphore();

    void up(unsigned count = 1);
    void down(unsigned count = 1);
    
  private:
    pthread_mutex_t mutex;
    pthread_cond_t  condition;
    unsigned	    level;
};


inline Semaphore::Semaphore(unsigned level)
:
  level(level)
{
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&condition, 0);
}


inline Semaphore::~Semaphore()
{
  pthread_cond_destroy(&condition);
  pthread_mutex_destroy(&mutex);
}


inline void Semaphore::up(unsigned count)
{
  pthread_mutex_lock(&mutex);
  level += count;
  pthread_cond_broadcast(&condition); // pthread_cond_signal() is incorrect
  pthread_mutex_unlock(&mutex);
}


inline void Semaphore::down(unsigned count)
{
  pthread_mutex_lock(&mutex);

  while (level < count)
    pthread_cond_wait(&condition, &mutex);

  level -= count;
  pthread_mutex_unlock(&mutex);
}

#endif
#endif
