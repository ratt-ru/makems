//# LofarConstants.h
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
//#  $Id: LofarConstants.h 13170 2009-04-24 09:39:40Z overeem $

#ifndef LOFAR_COMMON_LOFARCONSTANTS_H
#define LOFAR_COMMON_LOFARCONSTANTS_H

// \file
//

//# Never #include <config.h> or #include <lofar_config.h> in a header file!

namespace LOFAR {
	static const int N_POL					  = 2;				// Number of polarizations
	static const int NR_SUBRACKS_PER_CABINET  = 2;
	static const int NR_RSPBOARDS_PER_SUBRACK = 4;
	static const int NR_TBBOARDS_PER_RSPBOARD = 2;
	static const int NR_RCUS_PER_RSPBOARD     = 8;
	static const int NR_ANTENNAS_PER_RSPBOARD = (NR_RCUS_PER_RSPBOARD / N_POL);
	static const int NR_RCUS_PER_TBBOARD      = (NR_TBBOARDS_PER_RSPBOARD * NR_RCUS_PER_RSPBOARD);
	static const int NR_RCUS_PER_SUBRACK      = (NR_RCUS_PER_RSPBOARD * NR_RSPBOARDS_PER_SUBRACK);
	static const int NR_RCUS_PER_CABINET      = (NR_RCUS_PER_SUBRACK  * NR_SUBRACKS_PER_CABINET);
	static const int N_HBA_ELEM_PER_TILE	  = 16;						// Number of High Band antenna per tile

	static const int MAX_ANTENNAS			  = 96;						// Max number antenna's of each type.
	static const int MAX_RCUS				  = MAX_ANTENNAS * N_POL;	// Max number of RCU's in one station
	static const int MAX_SUBBANDS			  = 512;					// Number of subbands that are created
	static const int MAX_BEAMLETS_PER_RSP	  = 62;						// Max number of beamlets comming from one RSPboard
	static const int MAX_BEAMLETS			  = 4 * MAX_BEAMLETS_PER_RSP;	// Max number of beamlets comming from one station
	static const int MAX_RSPBOARDS			  = (MAX_RCUS / NR_RCUS_PER_RSPBOARD);

	static const int NR_RCU_MODES			  = 7;
}

#endif
