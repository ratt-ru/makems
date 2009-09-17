//#  FdSet.h: C++ version of fd_set.
//#
//#  Copyright (C) 2002-2004
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
//#  $Id: FdSet.h 10728 2007-12-07 12:32:30Z diepen $

#ifndef LOFAR_COMMON_FDSET_H
#define LOFAR_COMMON_FDSET_H

// \file
// C++ version of fd_set, does some boring bookkeeping.

//# Never #include <config.h> or #include <lofar_config.h> in a header file!
//# Includes
#include <sys/select.h>
#include <Common/LofarTypes.h>

namespace LOFAR {
// \addtogroup Common
// @{

// C++ version of fd_set
class FdSet
{
public:
	FdSet();
	~FdSet();

	// set bit in fd_set.
	void add   (int32 fID);

	// clear bit in fd_set.
	void remove(int32 fID);

	// clear whole fd_set.
	void clear ();

	// Is bit fID set?
	//# On some OS (e.g. OS-X Leopard) fd_set must be non-const!!
	inline bool isSet (int32 fID) 	const
          { return (FD_ISSET(fID, const_cast<fd_set*>(&itsSet))); }

	// Return a pointer to the fd_set.
	inline fd_set* getSet()	 		 	{ return (&itsSet);    }

	// Return number if highest fID added to the set.
	inline int32   highest() const 		{ return (itsHighest); }

	// Return number if lowest fID added to the set.
	inline int32   lowest()	 const		{ return (itsLowest);  }

	// Return number if bits set in the set.
	inline int32   count()	 const		{ return (itsFdCount); }

	//# FdSet(const FdSet&	that); // let compiler generate it.

	// Copying is allowed
	FdSet& operator=(const FdSet& that);

private:
	//# Datamembers
	fd_set		itsSet;
	int32		itsHighest;
	int32		itsLowest;
	int32		itsFdCount;
};

// @} addtogroup
} // namespace LOFAR

#endif
