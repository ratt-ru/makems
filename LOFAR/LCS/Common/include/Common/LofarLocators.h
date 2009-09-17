//#  LofarLocators.h: Tries to locate a file in an earlier defined path.
//#
//#  Copyright (C) 2006
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
//#  $Id: LofarLocators.h 10717 2007-12-04 13:53:32Z loose $

#ifndef LOFAR_COMMON_LOFARLOCATORS_H
#define LOFAR_COMMON_LOFARLOCATORS_H

// \file 
// Tries to locate a file in an earlier defined path.

//# Never #include <config.h> or #include <lofar_config.h> in a header file!
//# Includes
#include <Common/FileLocator.h>

namespace LOFAR {

//# --- Forward Declarations ---
//# classes mentioned as parameter or returntype without virtual functions.


// class_description
// FileLocator is a class that helps you to find a file that must be somewhere
// in an earlier defined path.
class ConfigLocator : public FileLocator
{
public:
	#define		CONFIG_SUB_DIR		"/etc"

	// Create a FileLocator without a predefined path.
	ConfigLocator() : FileLocator()
		{	setSubdir(CONFIG_SUB_DIR); }
	// Create a FileLocator with a predefined path.
	explicit ConfigLocator (const string&	aPath) : FileLocator(aPath) 
		{ 	setSubdir(CONFIG_SUB_DIR); }

private:
	ConfigLocator(const ConfigLocator&	that);
	ConfigLocator& operator=(const ConfigLocator& that);

	//# --- Datamembers ---
};


class ProgramLocator : public FileLocator
{
public:
	#define		PROGRAM_SUB_DIR		"/bin"

	// Create a FileLocator without a predefined path.
	ProgramLocator() : FileLocator()
		{	setSubdir(PROGRAM_SUB_DIR); }
	// Create a FileLocator with a predefined path.
	explicit ProgramLocator (const string&	aPath) : FileLocator(aPath) 
		{ 	setSubdir(PROGRAM_SUB_DIR); }

private:
	ProgramLocator(const ProgramLocator&	that);
	ProgramLocator& operator=(const ProgramLocator& that);

	//# --- Datamembers ---
};


} // namespace LOFAR

#endif
