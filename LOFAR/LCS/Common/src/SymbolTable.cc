//#  SymbolTable.cc: one line description
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
//#  $Id: SymbolTable.cc 11806 2008-09-28 21:07:32Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

//# Includes
#include <Common/SymbolTable.h>
#include <cstdlib>

#ifdef HAVE_BFD

namespace LOFAR
{

#if defined(__linux__)
  static const char* bfdFile = "/proc/self/exe";
#elif defined(__sun__)
  static const char* bfdFile = "/proc/self/object/a.out";
#else
# error "Alias for process's executable file, like /proc/self/exe \
on linux, must be present."
#endif


  SymbolTable::SymbolTable() :
    itsBfd(0),
    itsSymbols(0)
  {
    init() && read();
  }


  SymbolTable::~SymbolTable()
  {
    cleanup();
  }


  SymbolTable& SymbolTable::instance()
  {
    static SymbolTable symTab;
    return symTab;
  }


  bool SymbolTable::init()
  {
    bfd_init();
    if ((itsBfd = bfd_openr(bfdFile,0)) == 0) {
      bfd_perror(bfdFile);
      return false;
    }
    if (!bfd_check_format(itsBfd, bfd_object)) {
      bfd_perror(bfdFile);
      return false;
    }
    return true;
  }
 

  bool SymbolTable::read()
  {
    if ((bfd_get_file_flags(itsBfd) & HAS_SYMS) == 0) {
      bfd_perror(bfdFile);
      return true;
    }
    unsigned int size;
    long symcount;
    symcount = bfd_read_minisymbols(itsBfd, false, (void**) &itsSymbols, &size);
    if (symcount == 0) {
      symcount = bfd_read_minisymbols(itsBfd, true, (void**) &itsSymbols, &size);
    }
    if (symcount < 0) {
      bfd_perror(bfdFile);
      return false;
    }
    return true;
  }


  void SymbolTable::cleanup()
  {
    if (itsSymbols) {
      free(itsSymbols);
      itsSymbols = 0;
    }
    if (itsBfd) {
      bfd_close(itsBfd);
      itsBfd = 0;
    }
  }

} // namespace LOFAR

#endif /* HAVE_BFD */
