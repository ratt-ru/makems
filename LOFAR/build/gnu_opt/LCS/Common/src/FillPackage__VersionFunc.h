/* Generated by CMake for project Common */

namespace LOFAR {
  std::string CommonVersion::getBuildTime()
  { return std::string(__DATE__) + " " + std::string(__TIME__); }
  std::string CommonVersion::getBuildUser()
  { return "oms"; }
  std::string CommonVersion::getBuildMachine()
  { return "jakob"; }
  void CommonVersion::fillInfo
    (int level, std::vector<Version::PkgInfo>& vec)
  {
    // Add info of this package if needed.
    vec.push_back (Version::PkgInfo(level, &getInfo));
    // Add info of packages being used.
  }
}