//#  filename.cc: one line description
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
//#  $Id: RunOnNode.cc 4847 2005-04-13 14:33:43Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/RunOnNode.h>

namespace LOFAR
{

int  RunOnNode::itsProcessID=0;
int  RunOnNode::itsApplID=0;
  
  RunOnNode::RunOnNode(int processid, int applid) {
    itsProcessID = processid;
    itsApplID    = applid;
  }


} // namespace LOFAR
