//#  Process.h: class wrapping the OS fork method.
//#
//#  Copyright (C) 2005
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
//#  $Id: Process.h 7711 2006-02-28 09:20:04Z loose $

#ifndef LOFAR_COMMON_PROCESS_H
#define LOFAR_COMMON_PROCESS_H

// \file
// Class wrapping the OS fork method.

//# Never #include <config.h> or #include <lofar_config.h> in a header file!

//# Includes
#include <sys/types.h>    // for pid_t

namespace LOFAR 
{
  // \addtogroup Common
  // @{

  // This class provides a wrapper around the OS fork() method. Classes that
  // support spawning can inherit (privately) from this class. Two virtual
  // methods, parent() and child(), can be overridden by the derived class
  // to implement class specific behaviour in the parent and child process
  // respectively.
  class Process
  {
  public:
    // We need a virtual destructor, because this is an abstract base class.
    virtual ~Process();

    // Spawn the current process, using the sytem's fork() method. After
    // spawning the process, spawn() will either call parent() or child(),
    // depending on whether the current process is the parent or the child
    // process. The parent() and child() method can be overridden by the
    // user.
    //
    // If the parent process does not want to fetch the child's termination
    // status you can specfiy \a avoidZombies = \c true. In that case, the
    // spawned process will be made child of the \c init process, which will
    // fetch the child's termination status.
    //
    // \return \c true if spawn succeeded, else \c false.
    bool spawn(bool avoidZombies = false);

    // Return true if this is the parent process.
    bool isParent() { return itsPid > 0; }

    // Return true if this is the child process.
    bool isChild() { return itsPid == 0; }

  protected:
    // Constructor
    Process();

    // This method will be called in the parent process, immediately after
    // spawning.
    virtual void parent();

    // This method will be called in the child process, immediately after
    // spawning.
    virtual void child();

  private:
    // Copying is not allowed
    Process(const Process& that);
    Process& operator=(const Process& that);

    // Do the actual spawn. 
    // \return \c true on succes; \c false on failure.
    bool doSpawn(bool avoidZombies);

    //# Datamembers

    // The process id that was returned by the invocation of ::fork(). If
    // itsPid == 0, then this is the child process; if itsPid > 0, then
    // this is the parent process; if itsPid < 0, it indicates an error.
    pid_t itsPid;

  };

  // @}

} // namespace LOFAR

#endif
