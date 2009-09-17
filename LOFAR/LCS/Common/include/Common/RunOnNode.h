//#  filename.h: one line description
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
//#  $Id: RunOnNode.h 10717 2007-12-04 13:53:32Z loose $

#ifndef LOFAR_COMMON_RUNONNODE_H
#define LOFAR_COMMON_RUNONNODE_H

// \file

//# Never #include <config.h> or #include <lofar_config.h> in a header file!

//# Includes
#include <Common/LofarLogger.h>

namespace LOFAR
{
  //# Forward Declarations

  // \addtogroup Common
  // @{
  
  class RunOnNode
  {
  public:
    RunOnNode(int procesid, int apllid=0);
    void setProcessID(int processid);
    void setApplID(int applid);

    bool mustExecute(int processid, int applid=0);
    bool mustExecuteAppl(int processid);

  private:
    static int itsProcessID;
    static int itsApplID;
  };
  
  inline void RunOnNode::setProcessID(int processid) {
    DBGASSERTSTR(processid>=0  , "processid value invallid");
    itsProcessID=processid;
  }

  inline void RunOnNode::setApplID(int applid) {
    DBGASSERTSTR(applid>=0  , "applid value invallid");
    itsApplID=applid;
  }

  inline bool RunOnNode::mustExecute(int processid, int applid) {
    bool result = ((itsProcessID == processid) && (itsApplID == applid));
    LOG_TRACE_COND_STR("mustExecute(" << processid << "," << applid << ")  :  " << result);
    return result;
  }
  inline bool RunOnNode::mustExecuteAppl(int applid) {
    bool result = (applid == itsApplID);
    LOG_TRACE_COND_STR("mustExecuteAppl(" << applid << ")  :  " << result);
    return result;
  }

#define SETNODE(processid, applid)       RunOnNode itsRON(processid,applid) 
//# todo: #define SETNODEPARM              RunOnNode itsRON(param(...),param(...))

#define RUNINPROCESS(processid, applid)  if(itsRON.mustExecute(processid,applid))
#define RIP(processid,applid)            if(itsRON.mustExecute(processid,applid))
#define RUNINAPPL(applid)                if(itsRON.mustExecuteAppl(applid))
#define RIA(applid)                      if(itsRON.mustExecuteAppl(applid))

  // @}

} // namespace LOFAR

#endif
