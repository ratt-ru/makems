//#  Version.cc: Helper class for version info
//#
//#  Copyright (C) 2007
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
//#  $Id: Version.cc 13414 2009-06-16 22:15:37Z loose $

#include <Common/Version.h>
#include <ostream>
#include <map>

namespace LOFAR {

  Version::Version (const std::string& packageName,
		    const std::string& version,
		    const std::string& confVersion,
		    const std::string& revision,
		    const std::string& packageRevision,
		    const std::string& nrChangedFiles,
		    const std::string& buildTime,
		    const std::string& buildUser,
		    const std::string& buildMachine)
    : itsPackageName     (packageName),
      itsVersion         (version),
      itsConfVersion     (confVersion),
      itsRevision        (revision),
      itsPackageRevision (packageRevision),
      itsNrChangedFiles  (nrChangedFiles),
      itsBuildTime       (buildTime),
      itsBuildUser       (buildUser),
      itsBuildMachine    (buildMachine)
  {}

  std::vector<std::string> Version::doUsedPackages
  (const std::vector<Version::PkgInfo>& vec)
  {
    std::vector<std::string> result;
    std::vector<PkgInfo> vflat = flatten (vec);
    result.reserve (vflat.size());
    for (std::vector<PkgInfo>::const_iterator viter = vflat.begin();
	 viter != vflat.end();
	 ++viter) {
      Version vers = viter->second();
      if (vers.version() != "trunk") {
	result.push_back (vers.packageName());
      } else {
	result.push_back (vers.packageName() + '-' + vers.version());
      }
    }
    return result;
  }

  void Version::doShow (std::ostream& os,
			const std::string& applName,
			const std::string& type,
			const std::vector<Version::PkgInfo>& vec)
  {
    if (vec.empty()) {
      return;
    } 
    // Get main info.
    // Set name to given name.
    Version versRoot = vec[0].second();
    if (! applName.empty()) {
      versRoot.setPackageName (applName);
    }
    // Print tree if required.
    if (type == "tree") {
      doShowTree (os, versRoot, vec);
      return;
    }
    bool full = (type == "full");
    // Only keep a single package in the vector at the highest level.
    std::vector<PkgInfo> vflat = flatten (vec);
    // Determine the maximum level and if revision is the same everywhere.
    int maxLevel = 0;
    bool sameRev = true;
    const std::string& mainRevision = versRoot.revision();
    for (std::vector<PkgInfo>::const_iterator viter = vflat.begin();
	 viter != vflat.end();
	 ++viter) {
      if (viter->first > maxLevel) {
	maxLevel = viter->first;
      }
      sameRev = sameRev && (viter->second().revision() == mainRevision);
    }
    versRoot.showAll (os, std::string(), sameRev);
    os << " packages used: " << std::endl;
    for (int level=1; level<=maxLevel; ++level) {
      for (std::vector<PkgInfo>::const_iterator viter = vflat.begin();
	   viter != vflat.end();
	   ++viter) {
	if (viter->first == level) {
	  Version vers = viter->second();
	  if (full) {
	    os << std::endl;
	    vers.showAll (os, "  ", true);
	  } else {
	    vers.showBrief (os, "  ");
	  }
	}
      }
    }
  }

  void Version::showAll (std::ostream& os,
			 const std::string& indent,
			 bool sameRev) const
  { 
    os << indent << packageName() << ": version = " << version();
    if (version() != confVersion()) {
      os << " (in CMakeLists.txt: " << confVersion() << ')';
    }
    os << std::endl;
    os << indent << " overall revision  = " << revision();
    if (!sameRev) os << " (note: used packages have different revision)";
    os << std::endl;
    os << indent << " package revision  = " << packageRevision()
       << " (last change in package)" << std::endl;
    os << indent << " built on " << buildMachine()
       << " by " << buildUser()
       << " at " << buildTime() << std::endl;
    if (nrChangedFiles() != "0") {
      os << indent << "  " << nrChangedFiles()
	 << " files were different from the repository" << std::endl;
    }
  }

  void Version::showBrief (std::ostream& os,
			   const std::string& indent) const
  {
    os << indent << packageName();
    if (version() != "trunk") {
      os << '-' << version();
    }
    os << " (rev." << revision() << ')';
    if (nrChangedFiles() != "0") {
      os << ' ' << nrChangedFiles() << " changed files";
    }
    os << std::endl;
  }

  void Version::doShowTree (std::ostream& os,
			    const Version& versRoot,
			    const std::vector<Version::PkgInfo>& vec)
  {
    versRoot.showAll (os, "", true);
    os << " tree of packages used: " << std::endl;
    std::vector<PkgInfo>::const_iterator viter = vec.begin();
    // Skip first one if name is the same as applName in versRoot.
    if (viter->second().packageName() == versRoot.packageName()) {
      ++viter;
    }
    for (; viter != vec.end(); ++viter) {
      // Blanks according to level.
      for (int i=0; i<viter->first; ++i) {
	os << ' ';
      }
      Version vers = viter->second();
      os << "  " << vers.packageName();
      if (vers.version() != "trunk") {
	os << '-' << vers.version();
      }
      os << " (rev." << vers.revision() << ')';
      os << std::endl;
    }
  }

  std::vector<Version::PkgInfo> Version::flatten
  (const std::vector<Version::PkgInfo>& vec)
  {
    std::vector<PkgInfo> result;
    // Only keep a single package in the vector at the highest level.
    // Map package to vector index.
    std::map<GetInfoFunc*, int> m;
    for (std::vector<PkgInfo>::const_iterator iter = vec.begin();
	 iter != vec.end();
	 ++iter) {
      int level = iter->first;
      GetInfoFunc* func = iter->second;
      std::map<GetInfoFunc*,int>::iterator mfnd = m.find(func);
      if (mfnd == m.end()) {
	m[func] = result.size();
	result.push_back (*iter);
      } else {
	int index = mfnd->second;
	if (level > result[index].first) {
	  result[index].first = level;
	}
      }
    }
    return result;
  }

}
