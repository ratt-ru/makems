//#  StringUtil.h: useful string manipulation methods.
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
//#  $Id: StringUtil.h 13875 2009-08-31 07:47:33Z diepen $

#ifndef LOFAR_COMMON_STRINGUTIL_H
#define LOFAR_COMMON_STRINGUTIL_H

// \file
// Useful string manipulation methods

//# Includes
#include <Common/LofarTypes.h>
#include <Common/lofar_string.h>
#include <Common/lofar_vector.h>
#include <Common/Exception.h>

namespace LOFAR
{

  // Useful string manipulation methods and classes.
  namespace StringUtil
  {
    // Split a string into substrings using \c c as the separator character.
    // The result may contain empty strings, e.g. when \c str contains two or
    // more consecutive occurrences of the separations character.
    //
    // For example:
    // \code
    //    res = StringUtil::split( "aa,bb,,dd,", ',' )
    // \endcode
    //
    // would yield the following array:
    // \verbatim
    //   res[0] = "aa"
    //   res[1] = "bb"
    //   res[2] = ""
    //   res[3] = "dd"
    //   res[4] = ""
    // \endverbatim
    vector<string> split(const string& str, char c);


    // Functor to compare two strings. Strings can be compared case sensitive
    // (\c NORMAL) and case insensitive (\c NOCASE).
    // \attention This class does not handle locales properly. It does string
    // comparison the way \c strcmp and \c strcasecmp (or \c stricmp for that
    // matter) do it.
    class Compare
    { 
    public:
      // String comparison mode.
      enum Mode {NORMAL, NOCASE};

      // Constructor. Initialize the comparison criterion. Default is "normal"
      // case sensitive comparison.
      Compare(Mode mode=NORMAL) : itsMode(mode) {}

      // The comparison operator
      bool operator()(const string& s1, const string& s2) const 
      {
	if (itsMode == NORMAL) return s1 < s2;
	else return lexicographical_compare(s1.begin(), s1.end(),
					    s2.begin(), s2.end(),
					    nocaseCompare);
      }

    private:
      // Helper function to do case insensitive comparison of two chars.
      static bool nocaseCompare(char c1, char c2)
      {
	return toupper(c1) < toupper(c2);
      }

      // The current comparison mode.
      Mode itsMode;
    };

  } // namespace StringUtil

//
// formatString(format, ...) --> string
//
// The function formatString accepts printf-like arguments and returns a
// formatted string. It can be used e.g. in cout constructions:
// cout << formatString("Opening connection with host %%s", hostName);
//# In real life this must be %s ofcourse but doxygen need a double %%.
const string formatString(const	char* format, ...);

//
// timeString(aTime [, gmt, format]) --> string
//
// The function timeString format the given timestamp into a human-readable
// format. The default format is yyyy-mm-dd hh:mm:ss
const string timeString(time_t     aTime, 
						bool       gmt = true,
						char*      format = "%F %T");

// Skip leading whitespace (blanks and horizontal tabs) starting at st.
// It returns the position of the first non-whitespace character.
// It returns end if all whitespace.
// It can be used in combination with rskipws.
uint lskipws (const string& value, uint st, uint end);

// Skip trailing whitespace (blanks and horizontal tabs) starting at end.
// It returns the position after the last non-whitespace character, thus
// value.substr(st, end-st) extracts the significant value.
// It returns st if all whitespace.
uint rskipws (const string& value, uint st, uint end);

// Skip past a quoted string.
// The quote character is the first character (at position st).
// Usually the quote character is ' or ", but it could be any other character.
// An exception is thrown if no ending quote character is found.
uint skipQuoted (const string& str, uint st);

// Skip past the end of a balanced pair of delimiters where nested pairs
// are also skipped. Delimiters in quoted parts are ignored.
// The starting delimiter is the first character in the string (at position st).
// The ending delimiter is given as an argument.
// The function works well if starting and ending delimiter are the same.
// An exception is thrown if the delimiters are not balanced.
// For example, it will skip something like '[[1,2,3],[4,5,6]]'.
uint skipBalanced (const string& str, uint st, uint end, char endChar);

//
// rtrim(char* CString [,len])
//
// Skip trailing spaces. If len of string is already known at invocation
// it may be given thus avoiding a relative expensive strlen call.
//
// Returns the length of the new string
//
// NOTE: original string is truncated!
//
int32 	rtrim(char*	aCstring, int32		len = 0, char* whiteSpace = " 	");

//
// char* ltrim(char*	Cstring)
//
// Skip leading spaces. A pointer to the first non-whitespace char is
// returned. (points into original string)
char*	ltrim(char*	aCstring, char* whiteSpace = " 	");

//
// rtrim(aString)
//
// Removes trailing whitespace from the string.
//
inline void rtrim(string&		aString, const string& whiteSpace = " 	")
{
	aString = aString.erase(aString.find_last_not_of(whiteSpace)+1);
}

//
// ltrim(aString)
//
// Removes leading whitespace from the string.
//
inline void ltrim(string&		aString, const string&	whiteSpace = " 	")
{
	aString = aString.erase(0, aString.find_first_not_of(whiteSpace));
}

// Return an uppercased string of \a str.
std::string toUpper(string str);

// Return a lowercased string of \a str.
std::string toLower(string str);

// @name Convert numeric value to string
// Convert the value of any of the fundamental arithmetic data types to a
// string representation. Most of the toString() methods provide the user with
// a means to override the default formatting behaviour by supplying his/her
// own formatting string, following the well-known <tt>printf</tt>-like
// conversions.
//
// \attention The user is responsible for the correctness of the optional
// conversion string. No checks are done by the toString() methods.

// @{

inline std::string toString(bool val)
{
  return val ? "true" : "false";
}

inline std::string toString(char val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  else return formatString("%hhi", val);
}

inline std::string toString(unsigned char val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%hhu", val);
}

inline std::string toString(short val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%hi", val);
}

inline std::string toString(unsigned short val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%hu", val);
}

inline std::string toString(int val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%i", val);
}

inline std::string toString(unsigned int val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%u", val);
}

inline std::string toString(long val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%li", val);
}

inline std::string toString(unsigned long val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%lu", val);
}

#if HAVE_LONG_LONG
inline std::string toString(long long val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%lli", val);
}

inline std::string toString(unsigned long long val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  return formatString("%llu", val);
}
#endif


inline std::string toString(float val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  else return formatString("%g", val);
}

inline std::string toString(double val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  else return formatString("%g", val);
}

#if HAVE_LONG_DOUBLE
inline std::string toString(long double val, const char* fmt = 0)
{
  if (fmt) return formatString(fmt, val);
  else return formatString("%Lg", val);
}
#endif

// @}

// @name Convert string to numeric value
// Convert a string to the value of any of the fundamental arithmetic data 
// types. Most of the StringTo...() methods provide the user with
// a means to override the default formatting behaviour by supplying his/her
// own formatting string, following the well-known <tt>scanf</tt>-like
// conversions.
// It uses sscanf, so a converting e.g. '2a' to int succeeds and results in 2.
//
// \attention The user is responsible for the correctness of the optional
// conversion string. No checks are done by the StringTo...() methods.
//
// \attention These functions will be deprecated in a next release.
// @{

bool   StringToBool  (const string& aString);      
int16  StringToInt16 (const string& aString,const char* fmt=0);
uint16 StringToUint16(const string& aString,const char* fmt=0);
int32  StringToInt32 (const string& aString,const char* fmt=0);
uint32 StringToUint32(const string& aString,const char* fmt=0);
#if HAVE_LONG_LONG
int64  StringToInt64 (const string& aString,const char* fmt=0);
uint64 StringToUint64(const string& aString,const char* fmt=0);
#endif
float  StringToFloat (const string& aString,const char* fmt=0);
double StringToDouble(const string& aString,const char* fmt=0);

// @}

// @name Strict conversion of string to numeric value
// Convert a string to the value of any of the fundamental arithmetic data 
// types. It checks if the entire string is used for the conversion.
// An integer value can also be given in hexadecimal format (e.g. 0x123).
// Leading and trailing whitespace is allowed.
// It checks if an integer value does not exceed the data type range.
// @{
long          strToLong   (const string& aString);
int           strToInt    (const string& aString);
int32         strToInt32  (const string& aString);
int16         strToInt16  (const string& aString);
unsigned long strToUlong  (const string& aString);
uint          strToUint   (const string& aString);
uint32        strToUint32 (const string& aString);
uint16        strToUint16 (const string& aString);
#if HAVE_LONG_LONG
int64         strToInt64  (const string& aString);
uint64        strToUint64 (const string& aString);
#endif
float         strToFloat  (const string& aString);
double        strToDouble (const string& aString);
inline bool   strToBool   (const string& aString)
  { return StringToBool (aString); }
// @}

// @name Manipulate strings containing a array specification
// Array specifications are often entered by the user with ranges like 3..32,55..58
// For converting such a string to a real vector the spec must be expanded so that
// it contains all elements instead of the ranges. 
// Likewise, when you present a array to the user you often want to show a spec with
// the ranges instead of all individual elements.
// See the ParameterSet document for a detailed description of the syntax.

// @{
// Given a string 'xx, xx, xx' this utility compacts the string
// by replacing series with range.
// Eg. [ lii001, lii002, lii003, lii005 ] --> [ lii001..lii003, lii005 ]
string compactedArrayString(const string&	orgStr);

// Given a string 'xx..xx, xx' this utility expands the string
// by replacing ranges with the fill series.
// Eg. [ lii001..003xx, lii005 ] --> [ lii001xx, lii002xx, lii003xx, lii005 ]
string expandRangeString(const string&);

// Given a string like '3*str' this utility expands the string
// by replacing the string 3 times.
// Eg. [3*0] --> [0,0,0]
string expandMultString(const string&);

// Apply expandMultString and expandRangeString (in that order) for an array
// string which must be enclosed in square brackets.
string expandArrayString(const string&);
// @} 

} // namespace LOFAR

#endif
