//#  AddressTranslator.cc: one line description
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
//#  $Id: AddressTranslator.cc 11896 2008-10-14 19:40:33Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

//# Includes
#include <Common/AddressTranslator.h>
#include <Common/Backtrace.h>
#include <Common/SymbolTable.h>
#include <cstdlib>
#include <cstring>

#ifdef HAVE_CPLUS_DEMANGLE
# include <demangle.h>
#endif

namespace LOFAR
{

  AddressTranslator::AddressTranslator()
  {
  }

  AddressTranslator::~AddressTranslator()
  {
  }

  void AddressTranslator::operator()(std::vector<Backtrace::TraceLine>& trace,
                                     void* const* addr, int size) 
  {
    // Initialize \a size elements of vector \a trace to avoid reallocations.
    trace.resize(size);
  
#ifdef HAVE_BFD
    bfd* abfd = SymbolTable::instance().getBfd();
    if (!abfd) return;

    for (int i = 0; i < size; i++) {
      pc = reinterpret_cast<bfd_vma>(addr[i]);
      found = false;

      bfd_map_over_sections(abfd, find_address_in_section, this);
    
      if (found) {
        if (functionname && *functionname) {
#ifdef HAVE_CPLUS_DEMANGLE
          char* res = cplus_demangle(functionname, DMGL_ANSI | DMGL_PARAMS);
          if (res == 0) {
            trace[i].function = functionname;
          }
          else {
            trace[i].function = res;
            free(res);
          }
#else
          trace[i].function = functionname;
#endif
        }
        if (filename) {
          const char* h = strrchr(filename,'/');
          if (h) {
            filename = h+1;
          }
          trace[i].file = filename;
        }
        trace[i].line = line;
      }
      //     if (trace[i].function == "main")
      //       break;
    }
#else
    addr = addr;  // suppress `unused parameter' warning
#endif
    return;
  }


  //##----  P r i v a t e   f u n c t i o n s  ----##//

#ifdef HAVE_BFD

  void AddressTranslator::find_address_in_section(bfd*      abfd,
						  asection* section,
						  void*     data)
  {
    AddressTranslator* obj = static_cast<AddressTranslator*>(data);
    obj->do_find_address_in_section(abfd, section);
  }

  void AddressTranslator::do_find_address_in_section(bfd*       abfd, 
						     asection*  section)
  {
    if (found)
      return;
      
    asymbol** syms = SymbolTable::instance().getSyms();
    if (!syms)
      return;

    if ((bfd_get_section_flags (abfd, section) & SEC_ALLOC) == 0)
      return;
      
    bfd_vma vma = bfd_get_section_vma (abfd, section);
    if (pc < vma)
      return;
      
    bfd_size_type size = bfd_get_section_size (section);
    if (pc >= vma + size)
      return;

    found = bfd_find_nearest_line (abfd, section, syms, pc - vma, 
				   &filename, &functionname, &line);
  }
    
#endif

} // namespace LOFAR
