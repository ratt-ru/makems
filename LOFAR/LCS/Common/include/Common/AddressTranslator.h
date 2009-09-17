//#  AddressTranslator.h: Translate return addresses to function, file, line.
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
//#  $Id: AddressTranslator.h 11806 2008-09-28 21:07:32Z loose $

#ifndef LOFAR_COMMON_ADDRESSTRANSLATOR_H
#define LOFAR_COMMON_ADDRESSTRANSLATOR_H

//# Never #include <config.h> or #include <lofar_config.h> in a header file!

// \file
// Translate return addresses to function, file, line.

//# Includes
#include <Common/Backtrace.h>
#include <vector>

#ifndef HAVE_BFD
# warning Binary file descriptor (bfd) package is missing, \
please install the GNU binutils.
#else
# include <bfd.h>
#endif

namespace LOFAR
{
  // \ingroup Common
  // @{

  // Functor class for translating return addresses to function name,
  // filename, and line number. 
  //
  // \note The code is based on the utility addr2line.c in the GNU binutils 2.16.
  class AddressTranslator {
  public:
    AddressTranslator();
    ~AddressTranslator();

    // Translate the \a size addresses specified in \a addr to \a size
    // trace lines, containing function name, filename, and line number, iff
    // that information can be retrieved from the program's symbol table.
    // \param[out] trace vector containing the trace lines
    // \param[in]  addr C-array of return addresses
    // \param[in]  size number of return addresses in \a addr
    void operator()(std::vector<Backtrace::TraceLine>& trace, 
                    void* const* addr, int size);

#ifdef HAVE_BFD
  private:
    // The workhorse that translates addresses 
    bool translate_addresses();

    // Helper function to "convert" the member function
    // do_find_addresss_in_section() to a void(*), which can be passed as
    // argument to bfd_map_over_sections().
    // \see BFD documentation for details (<tt>info bfd</tt>).
    static void find_address_in_section(bfd*, asection*, void*);

    // Find the source code line nearest to 
    void do_find_address_in_section(bfd*, asection*);

    // Local variables used by translate_addresses() and
    // do_find_address_in_section().
    // @{
    bfd_vma pc;
    const char* filename;
    const char* functionname;
    unsigned int line;
    bool found;
    // @}
#endif
  };

  // @}

} // namespace LOFAR

#endif
