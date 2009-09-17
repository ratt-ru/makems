//#  Backtrace.h: Store stack frame return addresses for self-debugging.
//#
//#  Copyright (C) 2002-2008
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
//#  $Id: Backtrace.h 11806 2008-09-28 21:07:32Z loose $

#ifndef LOFAR_COMMON_BACKTRACE_H
#define LOFAR_COMMON_BACKTRACE_H

// \file
// Store stack frame return addresses for self-debugging.

#ifndef HAVE_BACKTRACE
# error Backtrace is not supported.
#endif

// \def BACKTRACE_MAX_RETURN_ADDRESSES Maximum number of stack frame return
// addresses that will be stored by the Backtrace class (default=50).
#ifndef BACKTRACE_MAX_RETURN_ADDRESSES
# define BACKTRACE_MAX_RETURN_ADDRESSES 50
#endif

//# Includes
#include <iosfwd>
#include <string>
#include <vector>

namespace LOFAR
{
  //# Forward declarations
  class SymbolTable;

  // \ingroup Common
  // @{

  // Store stack frame return addresses for self-debugging and provide a way
  // to print them in a human readable form.
  class Backtrace {
  public:
    // Layout of a trace line
    struct TraceLine {
      TraceLine() : function("??"), file("??"), line(0) {}
      std::string function;
      std::string file;
      unsigned line;
    };
    // Constructor. Calls backtrace() to fill \c itsAddr with the return
    // addresses of the current program state.
    Backtrace();

    // Print the current backtrace in human readable form into the output
    // stream \a os.
    void print(std::ostream& os) const;

    // Indicates whether we should stop printing a backtrace when we reach
    // main(), or not.
    static bool stopAtMain;

  private:
    // Maximum number of return addresses that we are willing to handle. Add
    // one to the compile time constant, because, later on, we will hide the
    // first return address (being the construction of %Backtrace itself).
    static const int maxNrAddr = BACKTRACE_MAX_RETURN_ADDRESSES + 1;

    // C-array of return addresses.
    void* itsAddr[maxNrAddr];

    // Actual number of return addresses returned by get_addresses().
    int itsNrAddr;

    // Traceback info containing function name, filename, and line number.
    // This vector will be filled by AddressTranslator.operator().
    mutable std::vector<TraceLine> itsTrace;
  };

  // Print the backtrace \a st to the output stream \a os.
  std::ostream& operator<<(std::ostream& os, const Backtrace& st);

  // @}

} // namespace LOFAR

#endif
