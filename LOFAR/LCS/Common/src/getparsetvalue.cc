//#  getparsetvalue.cc: Get a value as a string from a parameter set
//#
//#  Copyright (C) 2008
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
//#  $Id: getparsetvalue.cc 12371 2008-12-23 13:18:31Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/ParameterSet.h>
#include <Common/LofarLogger.h>
#include <stdexcept>
#include <iostream>

int main (int argc, const char* argv[])
{
  try {
    if (argc < 3) {
      std::cerr << "Run as: getparsetvalue parsetfile parmname [index]"
		<< "    negative index counts from the end (a la python)"
		<< std::endl;
      return 1;
    }
    int inx = 0;
    bool useAll = true;
    if (argc > 3) {
      std::istringstream iss(argv[3]);
      iss >> inx;
      useAll = false;
    }
    LOFAR::ParameterSet parset(argv[1]);
    if (useAll) {
      std::string value = parset.getString (argv[2]);
      std::cout << value << std::endl;
    } else {
      std::vector<std::string> values = parset.getStringVector (argv[2]);
      // Negative index is 
      int i = inx;
      if (i < 0) {
	i += values.size();
      }
      ASSERTSTR (i >= 0  &&  i < int(values.size()),
		 "Index " << inx
		 << " exceeds value size " << values.size()
		 << " of parameter " << argv[2]);
      std::cout << values[i] << std::endl;
    }
  } catch (std::exception &x) {
    std::cerr << x.what() << std::endl;
    return 1;
  }
  return 0;
}
