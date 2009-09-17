//#  ReadLine.cc: read a line from stdin using readline or cin
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
//#  $Id: ReadLine.cc 12536 2009-01-28 14:49:05Z diepen $

#include <lofar_config.h>
#include <Common/ReadLine.h>
#include <Common/StringUtil.h>
#include <Common/lofar_iostream.h>
#ifdef HAVE_READLINE
# include <readline/readline.h>
# include <readline/history.h>
#endif

namespace LOFAR
{
#ifdef HAVE_READLINE
  bool readLine (string& line, const string& prompt, bool addToHistory)
  {
    char* str = readline(prompt.c_str());
    if (!str) return false;
    line = string(str);
    free(str);
    if (addToHistory) add_history (line.c_str());
    return true;
  }
#else
  bool readLine (string& line, const string& prompt, bool)
  {
    if (!prompt.empty()) cerr << prompt;
    getline (cin, line);
    return cin;
  }
#endif

  bool readLineSkip (string& line, const string& prompt,
                     const string& commentChars)
  {
    bool fnd = false;
    while (!fnd  &&  readLine (line, prompt, false)) {
      // Skip leading and trailing whitespace.
      uint st  = lskipws (line, 0, line.size());
      uint end = rskipws (line, st, line.size());
      uint sz = end-st;
      if (sz > 0) {                            
        // non-empty line.
        if (commentChars.empty()  ||  commentChars.size() > sz) {
          // Do not test for comment
          fnd = true;
        } else {
          for (uint i=0; i<commentChars.size(); ++i) {
            if (commentChars[i] != line[st+i]) {
              // non-comment
              line = line.substr(st, sz);
              fnd = true;
              break;
            }
          }
        }
      }
    }
#ifdef HAVE_READLINE
    add_history (line.c_str());
#endif
    return fnd;
  }

} // namespace LOFAR
