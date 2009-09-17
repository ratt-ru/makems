//#  ParameterSet.cc: Implements a map of Key-Value pairs.
//#
//#  Copyright (C) 2002-2003
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
//#  $Id: ParameterSet.cc 12371 2008-12-23 13:18:31Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>


#include <Common/ParameterSet.h>
#include <Common/LofarLogger.h>
#include <Common/lofar_fstream.h>


namespace LOFAR {

//-------------------------- creation and destroy ---------------------------
static ParameterSet* globalParameterSetInstance = 0;

ParameterSet* globalParameterSet()
{
  if (globalParameterSetInstance == 0) {
    globalParameterSetInstance = new ParameterSet();
  }
  return (globalParameterSetInstance);
}

//-------------------------- creation and destroy ---------------------------
//
// Default constructor
//
ParameterSet::ParameterSet(KeyCompare::Mode	mode)
  : itsSet (new ParameterSetImpl(mode))
{}

ParameterSet::ParameterSet(bool caseInsensitive)
  : itsSet (new ParameterSetImpl(caseInsensitive ?
                                 KeyCompare::NOCASE : KeyCompare::NORMAL))
{}

//
// Construction by reading a parameter file.
//
ParameterSet::ParameterSet(const string& theFilename, bool caseInsensitive)
  : itsSet (new ParameterSetImpl(theFilename, caseInsensitive ?
                                 KeyCompare::NOCASE : KeyCompare::NORMAL))
{}

//
// Construction by reading a parameter file.
//
ParameterSet::ParameterSet(const string&	theFilename,
			   KeyCompare::Mode	mode)
  : itsSet (new ParameterSetImpl(theFilename, mode))
{}

ParameterSet::ParameterSet(const char*	theFilename,
			   KeyCompare::Mode	mode)
  : itsSet (new ParameterSetImpl(string(theFilename), mode))
{}

//
// Copying is allowed.
//
ParameterSet::ParameterSet(const ParameterSet& that)
  : itsSet (that.itsSet->incrCount())
{}

//
// operator= copying
//
ParameterSet& 
ParameterSet::operator=(const ParameterSet& that)
{
	if (this != &that) {
	  unlink();
	  itsSet = that.itsSet->incrCount();
	}
	return (*this);
}

//
//	Destructor
//
ParameterSet::~ParameterSet()
{
  unlink();
}

void ParameterSet::unlink()
{
  if (itsSet->decrCount() == 0) {
    delete itsSet;
  }
}

//
// operator<<
//
std::ostream&	operator<< (std::ostream& os, const ParameterSet &thePS)
{
	os << *thePS.itsSet;
	return os;
}

} // namespace LOFAR
