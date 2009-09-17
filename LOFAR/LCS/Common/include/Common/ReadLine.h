//#  ReadLine.h: read a line from stdin using readline or cin
//#
//#  Copyright (C) 2009
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
//#  $Id: ReadLine.h 12536 2009-01-28 14:49:05Z diepen $

#ifndef LOFAR_COMMON_READLINE_H
#define LOFAR_COMMON_READLINE_H

// \file

// The functions in here read a line from stdin using readline.
// If readline is not found during configure, it reverts to reading from cin.
//
// By default file name completion can be used and command recall and editing.
// If context-sensitive completion is needed, the user should initialize
// readline explicitly.

//# Includes
#include <Common/lofar_string.h>

namespace LOFAR
{
  // Print a prompt, read a line, and add it to the history.
  // False is returned if EOF is given.
  // The resulting line is put in the argument \a line.
  bool readLine (string& line, const string& prompt=string(),
                 bool addToHistory=true);

  // Same as readLine, but remove leading whitespace, skip empty lines,
  // and optionally skip comment lines.
  // The default comment character string is empty meaning that no test
  // for comment lines is done.
  bool readLineSkip (string& line, const string& prompt=string(),
                     const string& commentChars=string());

} // namespace LOFAR

#endif
