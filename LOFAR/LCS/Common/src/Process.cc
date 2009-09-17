//#  Process.cc: class wrapping the OS fork method.
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
//#  $Id: Process.cc 11588 2008-08-15 12:19:06Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

//# Includes
#include <Common/Process.h>
#include <Common/LofarLogger.h>
#include <unistd.h>    // for fork(), and getpid()
#include <sys/wait.h>  // for waitpid()
#include <sys/types.h>
#include <cerrno>      // for errno
#include <cstring>     // for strerror()
#include <cstdlib>     // for exit()

using namespace std;

namespace LOFAR
{

  Process::Process() : 
    itsPid(getpid())
  {
  }
    

  Process::~Process()
  {
  }


  bool Process::spawn(bool avoidZombies)
  {
    LOG_TRACE_FLOW(AUTO_FUNCTION_NAME);
    if (doSpawn(avoidZombies)) {
      if (isChild()) child();
      else if (isParent()) parent();
      return true;
    } 
    else {
      LOG_ERROR_STR("Failed to fork child process - " << strerror(errno));
      return false;
    }
  }


  void Process::parent()
  {
    LOG_TRACE_FLOW(AUTO_FUNCTION_NAME);
  }


  void Process::child()
  {
    LOG_TRACE_FLOW(AUTO_FUNCTION_NAME);
  }


  bool Process::doSpawn(bool avoidZombies)
  {
    if (!avoidZombies) {
      itsPid = ::fork();
      return (itsPid >= 0);
    }
    else {
      // This algorithm is adapted from an example in the Stevens book
      // "Advanced Programming in the Unix Environment". It creates an
      // orphan process that's inherited by the init process; init cleans up
      // when the orphan process terminates.
      pid_t pid = ::fork();

      if (pid == 0) {
        // The child process forks again to create a grandchild.
        itsPid = ::fork();
        switch(itsPid) {
        case -1:    // fork failed
          return false;
        case 0:     // grandchild returns 0
          return true;
        default:    // child terminates, orphaning grandchild
          exit(0);
        }
      }
      // Parent process waits for child to terminate
      return (pid >= 0 && waitpid(pid, 0, 0) == pid);
    }
  }


} // namespace LOFAR
