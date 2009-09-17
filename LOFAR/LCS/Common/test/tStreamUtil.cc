//#  tStreamUtil.cc: test program for the stream utilities
//#
//#  Copyright (C) 2002-2003
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
//#  $Id: tStreamUtil.cc 11748 2008-09-16 11:03:51Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/StreamUtil.h>
#include <Common/LofarLogger.h>
#include <Common/lofar_vector.h>
#include <Common/lofar_string.h>
#include <cstring>

using namespace LOFAR;

int main()
{
  INIT_LOGGER("tStreamUtil");
  cout << endl << "Usage of indent:" << endl;
  cout << indent << "Zeroth level, no indentation" << endl;
  {
    Indent id;
    cout << indent << "First level indentation" << endl;
    {
      Indent id;
      cout << indent << "Second level indentation" << endl;
    }
    cout << indent << "Back to first level" << endl;
  }
  cout << indent << "Back to zeroth level" << endl;
  cout << endl;


  vector<int> vi;
  for (int i = 0; i < 20; i++) vi.push_back(i+1);
  cout << "A vector of integers:" << endl
       << vi << endl << endl;
  
  vector<string> vs;
  vs.push_back("aap");   vs.push_back("noot");    vs.push_back("mies");
  vs.push_back("wim");   vs.push_back("zus");     vs.push_back("jet");
  vs.push_back("teun");  vs.push_back("vuur");    vs.push_back("gijs");
  vs.push_back("lam");   vs.push_back("kees");    vs.push_back("bok");
  vs.push_back("weide"); vs.push_back("does");    vs.push_back("hok");
  vs.push_back("duif");  vs.push_back("schapen");
  cout << "A vector of strings:" << endl
       << vs << endl << endl;

  return 0;
}
