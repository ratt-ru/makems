//#  SymbolTable.h: one line description
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
//#  $Id: SymbolTable.h 11806 2008-09-28 21:07:32Z loose $

#ifndef LOFAR_COMMON_SYMBOLTABLE_H
#define LOFAR_COMMON_SYMBOLTABLE_H

// \file
// one line description.

//# Includes
#ifndef HAVE_BFD
# warning Binary file descriptor (bfd) package is missing, \
please install the GNU binutils.
#else
# include <bfd.h>

namespace LOFAR
{

  // \addtogroup Common
  // @{

  // Class holding the symbol table of the current executable program. It is
  // implemented as a Singleton. So, any overhead will only be paid when this
  // class is instantiated, which will normally happen when the first
  // Backtrace is printed (\e not when the first exception is thrown).
  //
  // \note The code is based on the utility addr2line.c in GNU binutils 2.16.
  class SymbolTable {

  public:
    // Destructor.
    ~SymbolTable();

    // Return an instance of SymbolTable. When called for the first time
    // an instance of SymbolTable is created. Subsequent calls return the
    // previously created instance.
    static SymbolTable& instance();

    // Return a pointer to the symbol table
    asymbol** getSyms() const
    { return itsSymbols; }

    // Return a pointer to the bfd
    bfd* getBfd() const
    { return itsBfd; }

  private:
    // Default constructor is called from the static member function
    // SymbolTable::instance().
    SymbolTable();

    // Disallow copy constructor.
    SymbolTable(const SymbolTable&);

    // Disallow assignment operator.
    SymbolTable& operator=(const SymbolTable&);

    // Open the bfd-file and check its format.
    // @return True on success.
    bool init();

    // Read the symbol table from the bfd-file.
    // @return True when read was successful.
    bool read();

    // Free dynamically allocated memory; close bfd-file.
    void cleanup();

    // Pointer to the bfd
    bfd* itsBfd;

    // Pointer to the symbol table.
    asymbol** itsSymbols;
  };

  // @}

} // namespace LOFAR

#endif /* HAVE_BFD */

#endif
