//#  Backtrace.cc: one line description
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
//#  $Id: Backtrace.cc 11896 2008-10-14 19:40:33Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

//# Includes
#include <Common/Backtrace.h>
#include <Common/AddressTranslator.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <execinfo.h>

namespace LOFAR
{
  using namespace std;

  // Initialize to true, so that backtrace printing stop at main() function.
  bool Backtrace::stopAtMain = true;

  Backtrace::Backtrace() :
    itsNrAddr(0)
  {
    memset(itsAddr, 0, maxNrAddr*sizeof(void*));
    itsNrAddr = backtrace(itsAddr, maxNrAddr);
  }

  void Backtrace::print(ostream& os) const
  {
    if (itsTrace.empty()) {
      try {
        AddressTranslator()(itsTrace, itsAddr, itsNrAddr);
      } catch (std::bad_alloc&) {}
    }
      
    // Save the current fmtflags
    ios::fmtflags flags(os.flags());

    os.setf(ios::left);
    for(int i = 1; i < itsNrAddr; ++i) {
      if (i > 1) os << endl;
      os << "#" << setw(2) << i-1
         << " " << itsAddr[i];
      if (i < itsTrace.size()) {
        os << " in " << itsTrace[i].function
           << " at " << itsTrace[i].file
           << ":"    << itsTrace[i].line;
        if (stopAtMain && itsTrace[i].function == "main") break;
      }
    }

    // Restore the fmtflags
    os.flags(flags);
  }


  ostream& operator<<(ostream& os, const Backtrace& st)
  {
    st.print(os);
    return os;
  }

} // namespace LOFAR
