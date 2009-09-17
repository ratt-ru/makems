//# Timer.h: Accurate timer
//#
//# Copyright (C) 2004
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
//# $Id: Timer.h 12871 2009-03-12 12:54:57Z gels $

#ifndef LOFAR_COMMON_TIMER_H
#define LOFAR_COMMON_TIMER_H

// \file
// Very accurate timer for elapsed times.

#include <cstdlib>
#include <cstring>
#include <iostream>

#include <Common/LofarLogger.h>

#if defined __ia64__ && defined __INTEL_COMPILER
#include <ia64regs.h>
#endif


namespace LOFAR {

  // Low-overhead and high-resolution interval timer for use on i386, x86_64,
  // ia64, and powerpc platforms, using the processor's timestamp counter that
  // is incremented each cycle.
  // Put timer.start() and timer.stop() calls around the piece of
  // code to be timed; make sure that start() and stop() calls alternate.
  // A timer can be started and stopped multiple times; both the average and
  // total time, as well as the number of iterations are printed.
  // The measured time is real time (as opposed to user or system time).
  // The timer can be used to measure from 10 nanosecond to a century interval.

  class NSTimer {
  public:
    // Construct.
    // The given name will be used when the timer is printed.
    NSTimer (const std::string& name = std::string(),
	     bool print_on_destruction = false,
	     bool log_on_destruction = false);

    // Destruct.
    // The time is printed on stderr if print_on_destruction is true.
    ~NSTimer();

    // Start the timer.
    void start();
    // Stop the timer
    void stop();

    // Reset the timer to zero.
    void reset();

    // Print the timer.
    // <group>
    std::ostream& print (std::ostream &) const;
    friend std::ostream& operator<< (std::ostream&, const NSTimer&);
    // </group>

    // Get the elapsed time (in seconds).
    double getElapsed() const;

    // Get the total number of times start/stop is done.
    unsigned long long getCount() const;

  private:
    void print_time(std::ostream &, const char *which, double time) const;

    union {
      long long	   total_time;
      struct {
#if defined __PPC__
	int	   total_time_high, total_time_low;
#else
	int	   total_time_low, total_time_high;
#endif
      };
    };

#if defined __i386__ && defined __INTEL_COMPILER && defined _OPENMP
    union {
      unsigned long long count;
      struct {
	int count_low, count_high;
      };
    };
#else
    unsigned long long count;
#endif

    std::string itsName;
    bool print_on_destruction;
    bool log_on_destruction;

    static double CPU_speed_in_MHz;

    static double get_CPU_speed_in_MHz();
  };



  inline void NSTimer::reset()
  {
    total_time = 0;
    count      = 0;
  }

  inline unsigned long long NSTimer::getCount() const
  {
    return count;
  }

  inline NSTimer::NSTimer(const std::string& name, bool print_on_destruction, bool log_on_destruction)
    :
    itsName(name),
    print_on_destruction(print_on_destruction),
    log_on_destruction(log_on_destruction)
  {
    reset();
  }

  inline NSTimer::~NSTimer()
  {
    if (print_on_destruction) {
      if (log_on_destruction) {
        std::stringstream logStr;
        LOG_DEBUG_STR(print(logStr).rdbuf());
      } else
        print(std::clog);
    }
  }

  inline std::ostream& operator<< (std::ostream& str, const NSTimer& timer)
  {
    return timer.print(str);
  }


  inline void NSTimer::start()
  {
#if defined __x86_64__ && defined __INTEL_COMPILER && defined _OPENMP
    asm volatile
    (
	"rdtsc\n\t"
	"shlq $32,%%rdx\n\t"
	"leaq (%%rax,%%rdx),%%rax\n\t"
	"lock;subq %%rax,%0"
    :
	"+m" (total_time)
    :
    :
	"rax", "rdx"
    );
#elif defined __i386__ && defined __INTEL_COMPILER && defined _OPENMP
    asm volatile
    (
	"rdtsc\n\t"
	"lock;subl %%eax,%0\n\t"
	"lock;sbbl %%edx,%1"
    :
	"+m" (total_time_low), "+m" (total_time_high)
    :
    :
	"eax", "edx"
    );
#elif (defined __i386__ || defined __x86_64__) && (defined __GNUC__ || defined __INTEL_COMPILER)
    asm volatile
    (
	"rdtsc\n\t"
	"subl %%eax, %0\n\t"
	"sbbl %%edx, %1"
    :
	"+m" (total_time_low), "+m" (total_time_high)
    :
    :
	"eax", "edx"
    );
#elif (defined __i386__ || defined __x86_64__) && defined __PATHSCALE__
    unsigned eax, edx;

    asm volatile ("rdtsc" : "=a" (eax), "=d" (edx));

    total_time -= ((unsigned long long) edx << 32) + eax;
#elif defined __ia64__ && defined __INTEL_COMPILER
    total_time -= __getReg(_IA64_REG_AR_ITC);
#elif defined __ia64__ && defined __GNUC__
    long long time;
    asm volatile ("mov %0=ar.itc" : "=r" (time));
    total_time -= time;
#elif defined __PPC__ && (defined __GNUC__ || defined __xlC__)
    int high, low, retry;

    asm
    (
	"0:\n\t"
	"mfspr %0,269\n\t"
	"mfspr %1,268\n\t"
	"mfspr %2,269\n\t"
	"cmpw %2,%0\n\t"
	"bne 0b\n\t"
	"subfc %3,%1,%3\n\t"
	"subfe %4,%0,%4"
    :
	"=r" (high), "=r" (low), "=r" (retry),
	"=r" (total_time_low), "=r" (total_time_high)
    :
	"3" (total_time_low), "4" (total_time_high)
    );
#endif
  }


  inline void NSTimer::stop()
  {
#if defined __x86_64__ && defined __INTEL_COMPILER && defined _OPENMP
    asm volatile
    (
	"rdtsc\n\t"
	"shlq $32,%%rdx\n\t"
	"leaq (%%rax,%%rdx),%%rax\n\t"
	"lock;addq %%rax,%0"
    :
	"+m" (total_time)
    :
    :
	"rax", "rdx"
    );
#elif defined __i386__ && defined __INTEL_COMPILER && defined _OPENMP
    asm volatile
    (
	"rdtsc\n\t"
	"lock;addl %%eax, %0\n\t"
	"lock;adcl %%edx, %1"
    :
	"+m" (total_time_low), "+m" (total_time_high)
    :
    :
	"eax", "edx"
    );
#elif (defined __i386__ || defined __x86_64__) && (defined __GNUC__ || defined __INTEL_COMPILER)
    asm volatile
    (
	"rdtsc\n\t"
	"addl %%eax, %0\n\t"
	"adcl %%edx, %1"
    :
	"+m" (total_time_low), "+m" (total_time_high)
    :
    :
	"eax", "edx"
    );
#elif (defined __i386__ || defined __x86_64__) && defined __PATHSCALE__
    unsigned eax, edx;

    asm volatile ("rdtsc\n\t" : "=a" (eax), "=d" (edx));
    total_time += ((unsigned long long) edx << 32) + eax;
#elif defined __ia64__ && defined __INTEL_COMPILER
    total_time += __getReg(_IA64_REG_AR_ITC);
#elif defined __ia64__ && defined __GNUC__
    long long time;
    asm volatile ("mov %0=ar.itc" : "=r" (time));
    total_time += time;
#elif defined __PPC__ && (defined __GNUC__ || defined __xlC__)
    int high, low, retry;

    asm
    (
	"0:\n\t"
	"mfspr %0,269\n\t"
	"mfspr %1,268\n\t"
	"mfspr %2,269\n\t"
	"cmpw %2,%0\n\t"
	"bne 0b\n\t"
	"addc %3,%3,%1\n\t"
	"adde %4,%4,%0"
    :
	"=r" (high), "=r" (low), "=r" (retry),
	"=r" (total_time_low), "=r" (total_time_high)
    :
	"3" (total_time_low), "4" (total_time_high)
    );
#endif

#if defined __x86_64__ && defined __INTEL_COMPILER && defined _OPENMP
    asm volatile ("lock;addq $1,%0" : "+m" (count));
#elif defined __i386__ && defined __INTEL_COMPILER && defined _OPENMP
    asm volatile
    (
	"lock;addl $1,%0\n\t"
	"lock;adcl $0,%1"
    :
	"+m" (count_low), "+m" (count_high)
    );
#else
    ++ count;
#endif
  }

}  // end namespace LOFAR


#endif
