//# pkgdep.cc: Find dependencies of packages
//#
//# Copyright (C) 2004
//# ASTRON (Netherlands Foundation for Research in Astronomy)
//# P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//# This program is free software; you can redistribute it and/or modify
//# it under the terms of the GNU General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or
//# (at your option) any later version.
//#
//# This program is distributed in the hope that it will be useful,
//# but WITHOUT ANY WARRANTY; without even the implied warranty of
//# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//# GNU General Public License for more details.
//#
//# You should have received a copy of the GNU General Public License
//# along with this program; if not, write to the Free Software
//# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//# $Id: pkgdep.cc 11748 2008-09-16 11:03:51Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <vector>

using namespace std;

// Define the struct to hold the usage.
struct Used
{
  // Constructor.
  Used() : itsNused(0) {};
  // Number of times the package is used.
  int          itsNused;
  // Names of the packages being used.
  set<string> itsUses;
};

// Define the map to hold the usage.
typedef map<string,Used> UsedMap;

enum OutType {ASCII,XHTML,JS};

// Remove the basename part.
string baseName(const string& name, bool strip)
{
  if (!strip) {
    return name;
  }
  string::size_type pos = name.rfind('/');
  if (pos == string::npos) {
    return name;
  }
  return name.substr(pos+1);
}

// Replace slashes by underscores.
string replaceSlash (const string& str)
{
  string out = str;
  string::size_type idx = out.find('/');
  while (idx != string::npos) {
    out.replace (idx, 1, 1, '_');
    idx = out.find('/');
  }
  return out;
}

// Write the dependency tree in ASCII format.
void writeASCII (ostream& os, const string& pkg, UsedMap& dep,
		 const string& indent,
		 int depth, int maxdepth, bool strip)
{
  os << indent << baseName(pkg,strip) << endl;
  // write used packages if any and if maxdepth not reached.
  if (maxdepth < 0  ||  depth < maxdepth) {
    string newIndent = indent + ' ';
    set<string> uses = dep[pkg].itsUses;
    for (set<string>::const_iterator iter = uses.begin();
	 iter != uses.end();
	 ++iter) {
      writeASCII (os, *iter, dep, newIndent, depth+1, maxdepth, strip);
    }
  }
}

// Write JavaScript header.
void writeJSHeader (ostream& os, const string& hdrtxt)
{
  os << "<html>" << endl;
  os << "<head>" << endl;
  os << "<script src='lib/TreeMenu.js' language='JavaScript' type='text/javascript'></script>" << endl;
  os << "<title>" << hdrtxt << "</title>" << endl;
  os << "</head>" << endl;
  os << "<body>" << endl;
  os << "<center>" << endl;
  os << "<h1>" << hdrtxt << "</h1>" << endl;
  os << "</center>" << endl;
  os << "<script language='javascript' type='text/javascript'>" << endl;
  os << "  objTreeMenu_1 = new TreeMenu('images', 'objTreeMenu_1', '_self', 'treeMenuDefault', true, false);" << endl;
  os << "  newNode = objTreeMenu_1.addItem(new TreeNode('Lofar dependencies', 'folder.gif', null, false, true, ''));" << endl;
}

// Write JavaScript footer.
void writeJSFooter (ostream& os)
{
  os << "  objTreeMenu_1.drawMenu();" << endl;
  os << "  objTreeMenu_1.resetBranches();" << endl;
  os << "</script></body>" << endl;
  os << "</html>" << endl;
}

// Write the dependency tree in JavaScript format.
void writeJS (ostream& os, const string& pkg, UsedMap& dep,
	      const string& parent,
	      int depth, int maxdepth, int seqnr, bool strip,
	      const string& hreftxt)
{
  // Form the name for this node.
  ostringstream oss;
  oss << parent << '_' << seqnr+1;
  // Get children.
  set<string> uses = dep[pkg].itsUses;
  os << "newNode" << oss.str()
       << "= newNode" << parent << ".addItem(new TreeNode('"
       << baseName(pkg,strip) << "', '";
  if (uses.empty()) {
    os << "item";
  } else {
    os << "folder";
  }
  os << ".gif', null, false, true, ''));" << endl;
  // Write used packages if any and if maxdepth not reached.
  if (maxdepth < 0  ||  depth < maxdepth) {
    int newSeqnr = 0;
    for (set<string>::const_iterator iter = uses.begin();
	 iter != uses.end();
	 ++iter, ++newSeqnr) {
      writeJS (os, *iter, dep, oss.str(), depth+1, maxdepth, newSeqnr,
	       strip, hreftxt);
    }
  }
}

void writeXHTMLHeader (ostream& os, const string& hdrtxt, const string& pkg,
		       const string& hreftxt)
{
  os << "<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'>" << endl;
  os << " <head>" << endl;
  os << "    <meta http-equiv='Content-Type' content='text/xhtml;charset='iso-8859-1' />" << endl;
  os << "    <meta http-equiv='Content-Style-Type' content='text/css' />" << endl;
  os << "    <meta http-equiv='Content-Language' content='en' />" << endl;
  os << "    <link rel='stylesheet' href='doxygen.css'>" << endl;
  os << "    <title>TreeView</title>" << endl;
  os << "    <style type='text/css'>" << endl;
  os << "    <!--" << endl;
  os << "    .directory { font-size: 10pt; font-weight: bold; }" << endl;
  os << "    .directory h3 { margin: 0px; margin-top: 1em; font-size: 11pt; }" << endl;
  os << "    .directory p { margin: 0px; white-space: nowrap; }" << endl;
  os << "    .directory div { display: none; margin: 0px; }" << endl;
  os << "    .directory img { vertical-align: middle; }" << endl;
  os << "    -->" << endl;
  os << "    </style>" << endl;
  os << "    <script type='text/javascript'>" << endl;
  os << "    <!-- // Hide script from old browsers" << endl;
  os << "    " << endl;
  os << "    function findChildNode(node, name) " << endl;
  os << "    {" << endl;
  os << "      var temp;" << endl;
  os << "      if (node == null) " << endl;
  os << "      {" << endl;
  os << "        return null;" << endl;
  os << "      } " << endl;
  os << "      node = node.firstChild;" << endl;
  os << "      while (node != null) " << endl;
  os << "      {" << endl;
  os << "        if (node.nodeName == name) " << endl;
  os << "        {" << endl;
  os << "          return node;" << endl;
  os << "        }" << endl;
  os << "        temp = findChildNode(node, name);" << endl;
  os << "        if (temp != null) " << endl;
  os << "        {" << endl;
  os << "          return temp;" << endl;
  os << "        }" << endl;
  os << "        node = node.nextSibling;" << endl;
  os << "      }" << endl;
  os << "      return null;" << endl;
  os << "    }" << endl;
  os << "" << endl;
  os << "    function toggleFolder(id, imageNode) " << endl;
  os << "    {" << endl;
  os << "      var folder = document.getElementById(id);" << endl;
  os << "      var l = 0;" << endl;
  os << "      var vl = '/software/TreeBrowser/images/ftv2vertline.png';" << endl;
  os << "      if (imageNode != null && imageNode.nodeName != 'IMG') " << endl;
  os << "      {" << endl;
  os << "        imageNode = findChildNode(imageNode, 'IMG');" << endl;
  os << "        if (imageNode!=null) l = imageNode.src.length;" << endl;
  os << "      }" << endl;
  os << "      if (folder == null) " << endl;
  os << "      {" << endl;
  os << "      } " << endl;
  os << "      else if (folder.style.display == 'block') " << endl;
  os << "      {" << endl;
  os << "        while (imageNode != null && " << endl;
  os << "               imageNode.src.substring(l-vl.length,l) == vl)" << endl;
  os << "        {" << endl;
  os << "          imageNode = imageNode.nextSibling;" << endl;
  os << "          l = imageNode.src.length;" << endl;
  os << "        }" << endl;
  os << "        if (imageNode != null) " << endl;
  os << "        {" << endl;
  os << "          l = imageNode.src.length;" << endl;
  os << "          imageNode.nextSibling.src = '/software/TreeBrowser/images/ftv2folderclosed.png';" << endl;
  os << "          if (imageNode.src.substring(l-13,l) == '/software/TreeBrowser/images/ftv2mnode.png')" << endl;
  os << "          {" << endl;
  os << "            imageNode.src = '/software/TreeBrowser/images/ftv2pnode.png';" << endl;
  os << "          }" << endl;
  os << "          else if (imageNode.src.substring(l-17,l) == '/software/TreeBrowser/images/ftv2mlastnode.png')" << endl;
  os << "          {" << endl;
  os << "            imageNode.src = '/software/TreeBrowser/images/ftv2plastnode.png';" << endl;
  os << "          }" << endl;
  os << "        }" << endl;
  os << "        folder.style.display = 'none';" << endl;
  os << "      } " << endl;
  os << "      else " << endl;
  os << "      {" << endl;
  os << "        while (imageNode != null && " << endl;
  os << "               imageNode.src.substring(l-vl.length,l) == vl)" << endl;
  os << "        {" << endl;
  os << "          imageNode = imageNode.nextSibling;" << endl;
  os << "          l = imageNode.src.length;" << endl;
  os << "        }" << endl;
  os << "        if (imageNode != null)" << endl; 
  os << "        {" << endl;
  os << "          l = imageNode.src.length;" << endl;
  os << "          imageNode.nextSibling.src = '/software/TreeBrowser/images/ftv2folderopen.png';" << endl;
  os << "          if (imageNode.src.substring(l-13,l) == '/software/TreeBrowser/images/ftv2pnode.png')" << endl;
  os << "          {" << endl;
  os << "            imageNode.src = '/software/TreeBrowser/images/ftv2mnode.png';" << endl;
  os << "          }" << endl;
  os << "          else if (imageNode.src.substring(l-17,l) == '/software/TreeBrowser/images/ftv2plastnode.png')" << endl;
  os << "          {" << endl;
  os << "            imageNode.src = '/software/TreeBrowser/images/ftv2mlastnode.png';" << endl;
  os << "          }" << endl;
  os << "        }" << endl;
  os << "        folder.style.display = 'block';" << endl;
  os << "      }" << endl;
  os << "    }" << endl;
  os << "" << endl;
  os << "    // End script hiding --> " << endl;       
  os << "    </script>" << endl;
  os << " </head>" << endl;
  os << "" << endl;
  os << " <body>" << endl;
  os << "  <div class='directory'>" << endl;
  os << "   <h2>" << hdrtxt << "</h2>" << endl;
  if (! pkg.empty()) {
    os << "   <h3>";
    if (hreftxt.empty()) {
      os << pkg;
    } else {
      os << hreftxt;
      os << pkg << "</a>";
    }
    os << "</h3>" << endl;
  }
  os << "   <div style='display: block;'>" << endl;
}

void writeXHTMLFooter (ostream& os)
{
  os << "   </div>" << endl;
  os << "  </div>" << endl;
  os << " </body>" << endl;
  os << "</html>" << endl;
}

// Write the dependency tree in XHTML format.
bool writeXHTML (ostream& os, const string& pkg, UsedMap& dep,
		 const string& parent, const string& indent,
		 int depth, int maxdepth, int seqnr, bool strip,
		 const string& hreftxt, bool isLast,
		 const std::vector<bool>& parentIsLast)
{
  // Check if endless recursion.
  if (depth > 50) {
    cerr << "Endless recursion for package=" << pkg << endl;
    return false;
  }
  // Form the name for this node.
  ostringstream oss;
  oss << parent << '_' << seqnr+1;
  // Get children.
  set<string> uses = dep[pkg].itsUses;
  bool doChildren = !uses.empty() && (maxdepth < 0  ||  depth < maxdepth);
  os << indent << "<p>";
  for (int i=0; i<depth; ++i) {
    if (parentIsLast[i]) {
      os << "<img src='/software/TreeBrowser/images/ftv2blank.png' alt='&nbsp' width=16 height=22 />";
    } else {
      os << "<img src='/software/TreeBrowser/images/ftv2vertline.png' alt='|' width=16 height=22 />";
    }
  }
  if (!doChildren) {
    if (isLast) {
      os << "<img src='/software/TreeBrowser/images/ftv2lastnode.png' alt='/' width=16 height=22 />";
    } else {
      os << "<img src='/software/TreeBrowser/images/ftv2node.png' alt='o' width=16 height=22 />";
    }
    os << "<img src='/software/TreeBrowser/images/openordner.png' alt='*' width=24 height=22 />";
  } else {
    if (isLast) {
      os << "<img src='/software/TreeBrowser/images/ftv2plastnode.png' alt='\\' width=16 height=22 onclick='toggleFolder(";
    } else {
      os << "<img src='/software/TreeBrowser/images/ftv2pnode.png' alt='o' width=16 height=22 onclick='toggleFolder(";
    }
    os << '"' << "node" << oss.str() << '"';
    os << ", this)'/>";
    os << "<img src='/software/TreeBrowser/images/ftv2folderclosed.png' alt='+' width=24 height=22 onclick='toggleFolder(";
    os << '"' << "node" << oss.str() << '"';
    os << ", this)'/>";
  }
  if (hreftxt.empty()) {
    os << baseName(pkg,strip);
  } else {
    // Replace %pkg% in hreftxt by pkg.
    string::size_type idx = hreftxt.find("%pkg%");
    if (idx == string::npos) {
      os << hreftxt;
    } else {
      os << hreftxt.substr(0,idx) << replaceSlash(pkg)
	 << hreftxt.substr(idx+5);
    }
    os << baseName(pkg,strip) << "</a>";
  }
  os << "</p>" << endl;
  // Write used packages if any and if maxdepth not reached.
  if (doChildren) {
    os << "<div id='node" << oss.str() << "'>" << endl;
    string newIndent = indent + ' ';
    std::vector<bool> newParentIsLast(parentIsLast);
    newParentIsLast.push_back (isLast);
    unsigned int newSeqnr = 1;
    for (set<string>::const_iterator iter = uses.begin();
	 iter != uses.end();
	 ++iter, ++newSeqnr) {
      if (! writeXHTML (os, *iter, dep, oss.str(), newIndent,
			depth+1, maxdepth, newSeqnr, strip, hreftxt,
			newSeqnr == uses.size(), newParentIsLast)) {
	cerr << "                              " << pkg << endl;
	return false;
      }
    }
    os << "</div>" << endl;
  }
  return true;
}

void writeHeader (ostream& os, OutType outtype, const string& hdrtxt,
		  const string& pkg, const string& hreftxt)
{
  // Replace %pkg% in hdrtxt and hreftxt by pkg.
  string::size_type idx;
  string newhdrtxt = hdrtxt;
  while ((idx = newhdrtxt.find("%pkg%")) != string::npos) {
    newhdrtxt = newhdrtxt.substr(0,idx) + pkg + newhdrtxt.substr(idx+5);
  }
  string newhreftxt = hreftxt;
  while ((idx = newhreftxt.find("%pkg%")) != string::npos) {
    newhreftxt = newhreftxt.substr(0,idx) + pkg + newhreftxt.substr(idx+5);
  }
  switch (outtype) {
  case ASCII:
    break;
  case JS:
    writeJSHeader (os, newhdrtxt);
    break;
  case XHTML:
    writeXHTMLHeader (os, newhdrtxt, pkg, newhreftxt);
    break;
  }
}

void writeBody (ostream& os, OutType outtype, const string& pkg, UsedMap& dep,
		int maxdepth, int seqnr, bool strip,
		const string& hreftxt, bool isLast, bool skipSingle)
{
  // If it's a single package, only print its children if told so.
  set<string> uses = dep[pkg].itsUses;
  if (isLast && skipSingle && !uses.empty()) {
    unsigned int newSeqnr = 1;
    for (set<string>::const_iterator iter = uses.begin();
	 iter != uses.end();
	 ++iter, ++newSeqnr) {
      writeBody (os, outtype, *iter, dep, maxdepth-1, newSeqnr, strip,
		 hreftxt, newSeqnr==uses.size(), false);
    }
  } else {
    switch (outtype) {
    case ASCII:
      writeASCII (os, pkg, dep, "", 0, maxdepth, strip);
      break;
    case JS:
      writeJS (os, pkg, dep, "", 0, maxdepth, seqnr, strip, hreftxt);
      break;
    case XHTML:
      std::vector<bool> parentIsLast;
      if (! writeXHTML (os, pkg, dep, "", "    ", 0, maxdepth, seqnr, strip,
			hreftxt, isLast, parentIsLast)) {
	return;
      }
      break;
    }
  }
}

void writeFooter (ostream& os, OutType outtype)
{
  switch (outtype) {
  case ASCII:
    break;
  case JS:
    writeJSFooter (os);
    break;
  case XHTML:
    writeXHTMLFooter (os);
    break;
  }
}

// Determine all dependencies.
void findFlatDep (const string& pkg, UsedMap& dep, set<string>& flatUses)
{
  set<string> uses = dep[pkg].itsUses;
  for (set<string>::const_iterator iter = uses.begin();
       iter != uses.end();
       ++iter) {
    flatUses.insert (*iter);
    findFlatDep (*iter, dep, flatUses);
  }
}

int main(int argc, const char* argv[])
{
  if (argc < 2) {
    cerr << "Use as:   pkgdep inputfile [flat|maxdepth=-1] [ascii|xhtml|js]"
	 << "[strip] [top] [split=ext] [hdrtxt=string] [href=text]"
	 << endl;
    return 1;
  }
  bool flat=false;
  OutType outtype = JS;
  bool top=false;
  bool strip=false;
  bool split=false;
  string ext="";
  int maxdepth=-1;
  string hdrtxt;
  string hreftxt;
  for (int i=2; i<argc; ++i) {
    string arg = argv[i];
    string val;
    string::size_type idx = arg.find('=');
    if (idx != string::npos) {
      val = arg.substr(idx+1);
      arg = arg.substr(0, idx);
    }
    if (arg == "ascii") {
      outtype = ASCII;
    } else if (arg == "js") {
      outtype = JS;
    } else if (arg == "xhtml") {
      outtype = XHTML;
    } else if (arg == "top") {
      top = true;
    } else if (arg == "strip") {
      strip = true;
    } else if (arg == "split") {
      split = true;
      ext = val;
    } else if (arg == "flat") {
      flat = true;
      maxdepth = 1;
    } else if (arg == "hdrtxt") {
      hdrtxt = val;
    } else if (arg == "href") {
      hreftxt = val;
    } else {
      istringstream istr(argv[2]);
      istr >> maxdepth;
    }
  }

  // Hold the dependencies and the used in.
  UsedMap dep;

  // Read all packages and record which packages it uses and how often used.
  ifstream ifs(argv[1]);
  string pkg1, pkg2;
  while (ifs) {
    ifs >> pkg1;
    if (ifs) {
      ifs >> pkg2;
      dep[pkg1].itsUses.insert (pkg2);
      dep[pkg2].itsNused++;
    }
  }

  UsedMap* depPtr = &dep;
  // If flat dependencies have to be determined, do so for all packages.
  UsedMap flatdep;
  if (flat) {
    for (UsedMap::const_iterator iter = dep.begin();
	 iter != dep.end();
	 ++iter) {
      findFlatDep (iter->first, dep, flatdep[iter->first].itsUses);
    }
    depPtr = &flatdep;
  }

  // Get name of input file.
  string inname(argv[1]);
  // Default header text is "LOFAR Dependency Tree: " followed by file type.
  if (hdrtxt.empty()) {
    hdrtxt = "LOFAR Dependency Tree: " + inname.substr(inname.find('.') + 1);
  }
  if (!split) {
    writeHeader (cout, outtype, hdrtxt, "LOFAR", hreftxt);
  }
  // Write the dependencies starting at all root packages
  // (i.e. packages not used by others).
  // First count how many to write to find the last one.
  unsigned int nrout = 0;
  for (UsedMap::const_iterator iter = depPtr->begin();
       iter != depPtr->end();
       ++iter) {
    if (!top  ||  iter->second.itsNused == 0) {
      nrout++;
    }
  }
  unsigned int seqnr = 1;
  for (UsedMap::const_iterator iter = depPtr->begin();
       iter != depPtr->end();
       ++iter) {
    if (!top  ||  iter->second.itsNused == 0) {
      if (split) {
	string name = replaceSlash(iter->first) + ext;
	ofstream ofs(name.c_str());
	writeHeader (ofs, outtype, hdrtxt, iter->first, hreftxt);
	writeBody (ofs, outtype, iter->first, *depPtr,
		   maxdepth, seqnr, strip, hreftxt, true, true);
	writeFooter (ofs, outtype);
      } else {
	writeBody (cout, outtype, iter->first, *depPtr,
		   maxdepth, seqnr, strip, hreftxt, seqnr==nrout, true);
      }
      seqnr++;
    }
  }
  if (!split) {
    writeFooter (cout, outtype);
  }
}
