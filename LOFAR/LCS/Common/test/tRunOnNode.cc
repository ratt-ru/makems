//#  testRunOnNode.cc: Program to test the interface to the LofarLogger.
//#
//#  Copyright (C) 2004
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
//#  $Id: tRunOnNode.cc 7376 2006-01-03 14:50:34Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>


#include <Common/RunOnNode.h>

using namespace std;
using namespace LOFAR;

int main (int, const char**) {

  cout << "Program started" << endl;
  
  cout << "Initialise RunOnNode; tell this process to be (0,0)" << endl;
  SETNODE(0,0);
  
  cout << "Check for conditional execution" << endl;
  RUNINPROCESS(0,0)  cout << "Executing code for (0,0)" << endl;
  RUNINPROCESS(3,7)  cout << "ERROR: Executing code for (3,7)" << endl;

// todo:test reset of node (not implemented yet)
// 	cout << "Now re-init to (3,7)" << endl;
// 	SETNODE(3,7);
// 	cout << "Check for conditional execution" << endl;
// 	RUNINPROCESS(0,0)  cout << "ERROR: Executing code for (0,0)" << endl;
// 	RUNINPROCESS(3,7)  cout << "Executing code for (3,7)" << endl;

	
}

