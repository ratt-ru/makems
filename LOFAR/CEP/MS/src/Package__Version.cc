//# Get revision info; generated by CMake/makeversion

//## pkg=MS
//## incpath=MS
//## incdir=include/MS

#include <MS/Package__Version.h>
#include <Common/Version.h>
#include "FillPackage__VersionInc.h"

namespace LOFAR {
  Version MSVersion::getInfo()
  {
    return Version ("MS",
		    getVersion(), getConfVersion(),
		    getRevision(), getPackageRevision(),
		    getNrChangedFiles(),
		    getBuildTime(), getBuildUser(), getBuildMachine());
  }

  std::string MSVersion::getVersion()
    { return "trunk"; }
  std::string MSVersion::getConfVersion()
    { return "0.1"; }
  std::string MSVersion::getRevision()
    { return "14031"; }
  std::string MSVersion::getPackageRevision()
    { return "13993"; }
  std::string MSVersion::getNrChangedFiles()
    { return "1"; }
}

//# FillPackage__VersionFunc.h is generated in the build area by
//# LofarPackageVersion.cmake. It contains function fillInfo.
#include "FillPackage__VersionFunc.h"
