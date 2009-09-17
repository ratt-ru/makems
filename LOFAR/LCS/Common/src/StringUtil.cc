//#  StringUtil.cc: implementation of the string utilities class.
//#
//#  Copyright (C) 2002-2005
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
//#  $Id: StringUtil.cc 13938 2009-09-04 12:08:41Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/LofarLogger.h>
#include <Common/LofarTypes.h>
#include <Common/StringUtil.h>
#include <Common/lofar_algorithm.h>
#include <Common/lofar_iostream.h>
#include <Common/lofar_iomanip.h>
#include <cstring>
#include <cstdarg>
#include <ctime>
#include <errno.h>

namespace LOFAR
{

enum rangeElementEnum
{
  PARENTHESIS,
  ASTERISK,
  DOTDOT
};

typedef std::map< std::string, rangeElementEnum > rangeElementTable;
typedef rangeElementTable::const_iterator rangeElementLookup;
  
vector<string> StringUtil::split(const string& s, char c)
{
	vector<string> 		v;
	string::size_type 	i, j;
	string				substring;
	i = j = 0;
	while (j != string::npos) {
		j = s.find(c,i);
		if (j == string::npos) {
			substring = s.substr(i);
		} 
		else {
			substring = s.substr(i,j-i);
		}
		ltrim(substring);
		rtrim(substring);
		v.push_back(substring);
		i = j + 1;
    }
    return (v);
  }

//
// formatString(format, ...) --> string
//
// Define a global function the accepts printf like arguments and returns 
// a string.
//
const string formatString(const	char* format, ...) {
	char		tmp_cstring[10240];
	va_list		ap;

	va_start (ap, format);
	vsnprintf(tmp_cstring, sizeof(tmp_cstring), format, ap);
	va_end   (ap);

	return   string(tmp_cstring);
}

//
// timeString(aTime [,format]) --> string
//
// Define a global function that convert a timestamp into a humanreadable 
// format.
//
const string timeString(time_t		aTime, 
							 bool		gmt,
							 char* 		format)
{
	char	theTimeString [256];
	strftime(theTimeString, 256, format, gmt ? gmtime(&aTime) 
														: localtime(&aTime));

	return (theTimeString);
}


uint lskipws (const string& value, uint st, uint end)
{
  for (; st<end; ++st) {
    if (value[st] != ' '  &&  value[st] != '\t') {
      break;
    }
  }
  return st;
}
  
uint rskipws (const string& value, uint st, uint end)
{
  for (; end>st; --end) {
    if (value[end-1] != ' '  &&  value[end-1] != '\t') {
      break;
    }
  }
  return end;
}
  

//
// rtrim(char* CString [,len=0])
//
// Skip trailing spaces. If len of string is already known at invocation
// it may be given thus avoiding a relative expensive strlen call.
//
// NOTE: original string is truncated!
//
int32 rtrim(char*	aCstring, int32		len, char*	whiteSpace)
{
	if (!aCstring || !(*aCstring)) {		// aCstring must be valid
		return (0);
	}

	if (!len || aCstring[len] != '\0') {	// len unknown or wrong?
		len = strlen(aCstring);				// recalculate it.
	}
	--len;									// set on last char

	while ((len >= 0) && (strchr(whiteSpace, aCstring[len]))) {
		aCstring[len--] = '\0';
	}

	return (len+1);
}

//
// char* ltrim(char*	Cstring)
//
// skip leading spaces. A pointer to the first non-whitespace char is
// returned.
char*	ltrim(char*	aCstring, char*	whiteSpace)
{
	aCstring += strspn(aCstring, whiteSpace);

	return (aCstring);
}

string	toUpper(string str)
{
  transform(str.begin(), str.end(), str.begin(), toupper);
  return str;
}

string	toLower(string str)
{
  transform(str.begin(), str.end(), str.begin(), tolower);
  return str;
}

bool	StringToBool(const string& aString) throw(Exception)
{
	char	firstChar = aString.c_str()[0];
	if ((firstChar == 't') || (firstChar == 'T') || (firstChar == '1') || (firstChar == 'Y') || (firstChar == 'y'))
		return (true);

	if ((firstChar == 'f') || (firstChar == 'F') || (firstChar == '0') || (firstChar == 'N') || (firstChar == 'n'))
		return (false);

	THROW (Exception, aString + " is not a boolean value");
}	

int16	StringToInt16(const string& aString, const char* fmt) throw(Exception)
{
	int16		theShort;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theShort) : 
			   sscanf(aString.c_str(), "%hd", &theShort)) != 1) {
		THROW (Exception, aString + " is not an short value");
	}

	return (theShort);
}	

uint16	StringToUint16(const string& aString, const char* fmt) throw(Exception)
{
	uint16		theUshort;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theUshort) : 
			   sscanf(aString.c_str(), "%hu", &theUshort)) != 1) {
		THROW (Exception, aString + " is not an unsigned short value");
	}

	return (theUshort);
}	

int32	StringToInt32(const string& aString, const char* fmt) throw(Exception)
{
	int32		theInt;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theInt) : 
			   sscanf(aString.c_str(), "%d", &theInt)) != 1) {
		THROW (Exception, aString + " is not an integer value");
	}
	return (theInt);
}	

uint32	StringToUint32(const string& aString, const char* fmt) throw(Exception)
{
	uint32		theUint;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theUint) : 
			   sscanf(aString.c_str(), "%u", &theUint)) != 1) {
		THROW (Exception, aString + " is not an unsigned int value");
	}

	return (theUint);
}	

#if HAVE_LONG_LONG
int64	StringToInt64(const string& aString, const char* fmt) throw(Exception)
{
	int64		theLong;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theLong) : 
			   sscanf(aString.c_str(), "%lld", &theLong)) != 1) {
		THROW (Exception, aString + " is not a long integer value");
	}

	return (theLong);
}	

uint64	StringToUint64(const string& aString, const char* fmt) throw(Exception)
{
	uint64		theUlong;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theUlong) : 
			   sscanf(aString.c_str(), "%llu", &theUlong)) != 1) {
		THROW (Exception, aString + " is not an unsigned long integer value");
	}

	return (theUlong);
}	
#endif

float	StringToFloat(const string& aString, const char* fmt) throw(Exception)
{
	float		theFloat;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theFloat) : 
			   sscanf(aString.c_str(), "%g", &theFloat)) != 1) {
		THROW (Exception, aString + " is not a float value");
	}

	return (theFloat);
}	


double	StringToDouble(const string& aString, const char* fmt) throw(Exception)
{
	double		theDouble;
	if ((fmt ? sscanf(aString.c_str(), fmt, &theDouble) : 
			   sscanf(aString.c_str(), "%lg", &theDouble)) != 1) {
		THROW (Exception, aString + " is not a double value");
	}

	return (theDouble);
}	


long strToLong (const string& aString) throw(Exception)
{
  const char* str = aString.c_str();
  int st  = lskipws(aString, 0, aString.size());
  int end = rskipws(aString, st, aString.size());
  char* endPtr;
  long val;
  // Clear errno since strtol does not do it.
  errno = 0;
  // We don't want octal values, so use base 10 unless a hex value is given.
  if (end > st+2  &&  str[st] == '0'  &&  (str[st+1]=='x' || str[st+1]=='X')) {
    val = strtol (str+st, &endPtr, 0);
  } else {
    val = strtol (str+st, &endPtr, 10);
  }
  ASSERTSTR (endPtr == str+end, aString << " is not an integer value");
  ASSERTSTR (errno != ERANGE  &&  errno != EINVAL,
             aString << " is invalid or outside long int range");
  return val;
}

int strToInt (const string& aString) throw(Exception)
{
  long val = strToLong (aString);
  if (sizeof(int) != sizeof(long)) {
    if (sizeof(int) == 4) {
      ASSERTSTR (val >= long(-32768)*65536  &&  val <= 2147483647L,
                 val << " is outside 4-byte int range");
    }
  }
  return val;
}

int32 strToInt32 (const string& aString) throw(Exception)
{
  long val = strToLong (aString);
  if (sizeof(int32) != sizeof(long)) {
    ASSERTSTR (val >= long(-32768)*65536  &&  val <= 2147483647L,
               val << " is outside int32 range");
  }
  return val;
}

int16 strToInt16 (const string& aString) throw(Exception)
{
  long val = strToLong (aString);
  ASSERTSTR (val >= -32768L  &&  val <= 32767L,
             val << " is outside int16 range");
  return val;
}

unsigned long strToUlong (const string& aString) throw(Exception)
{
  const char* str = aString.c_str();
  int st  = lskipws(aString, 0, aString.size());
  int end = rskipws(aString, st, aString.size());
  char* endPtr;
  unsigned long val;
  // Clear errno since strtoul does not do it.
  errno = 0;
  // We don't want octal values, so use base 10 unless a hex value is given.
  if (end > st+2  &&  str[st] == '0'  &&  (str[st+1]=='x' || str[st+1]=='X')) {
    val = strtoul (str+st, &endPtr, 0);
  } else {
    val = strtoul (str+st, &endPtr, 10);
  }
  ASSERTSTR (endPtr == str+end, aString << " is not an integer value");
  ASSERTSTR (errno != ERANGE  &&  errno != EINVAL,
             aString << " is invalid or outside long uint range");
  return val;
}

uint strToUint (const string& aString) throw(Exception)
{
  unsigned long val = strToUlong (aString);
  if (sizeof(uint) != sizeof(unsigned long)) {
    if (sizeof(uint) == 4) {
      ASSERTSTR (val <= 4294967295UL,
                 val << " is outside 4-byte uint range");
    }
  }
  return val;
}

uint32 strToUint32 (const string& aString) throw(Exception)
{
  unsigned long val = strToUlong (aString);
  if (sizeof(uint32) != sizeof(unsigned long)) {
    ASSERTSTR (val <= 4294967295UL,
               val << " is outside uint32 range");
  }
  return val;
}

uint16 strToUint16 (const string& aString) throw(Exception)
{
  unsigned long val = strToUlong (aString);
  ASSERTSTR (val <= 65535UL,
             val << " is outside uint16 range");
  return val;
}

float strToFloat (const string& aString) throw(Exception)
{
  const char* str = aString.c_str();
  int st  = lskipws(aString, 0, aString.size());
  int end = rskipws(aString, st, aString.size());
  char* endPtr;
  // Clear errno since strtof does not do it.
  errno = 0;
  double val = strtof (str+st, &endPtr);
  ASSERTSTR (endPtr == str+end, aString << " is not an integer value");
  ASSERTSTR (errno != ERANGE  &&  errno != EINVAL,
             aString << " is invalid or outside float range");
  return val;
}

double strToDouble (const string& aString) throw(Exception)
{
  const char* str = aString.c_str();
  int st  = lskipws(aString, 0, aString.size());
  int end = rskipws(aString, st, aString.size());
  char* endPtr;
  // Clear errno since strtod does not do it.
  errno = 0;
  double val = strtod (str+st, &endPtr);
  ASSERTSTR (endPtr == str+end, aString << " is not an integer value");
  ASSERTSTR (errno != ERANGE  &&  errno != EINVAL,
             aString << " is invalid or outside double range");
  return val;
}

#if HAVE_LONG_LONG
int64 strToInt64  (const string& aString) throw(Exception)
{
  if (sizeof(int64) == sizeof(long)) return strToLong(aString);
  ASSERTSTR (sizeof(int64)==sizeof(long long),
             "strToInt64: sizeof(int64) cannot be handled");
  const char* str = aString.c_str();
  int st  = lskipws(aString, 0, aString.size());
  int end = rskipws(aString, st, aString.size());
  char* endPtr;
  long long val;
  // Clear errno since strtoll does not do it.
  errno = 0;
  // We don't want octal values, so use base 10 unless a hex value is given.
  if (end > st+2  &&  str[st] == '0'  &&  (str[st+1]=='x' || str[st+1]=='X')) {
    val = strtoll (str+st, &endPtr, 0);
  } else {
    val = strtoll (str+st, &endPtr, 10);
  }
  ASSERTSTR (endPtr == str+end, aString << " is not an integer value");
  ASSERTSTR (errno != ERANGE  &&  errno != EINVAL,
             aString << " is invalid or outside long long int range");
  return val;
}

uint64 strToUint64 (const string& aString) throw(Exception)
{
  if (sizeof(uint64) == sizeof(unsigned long)) return strToUlong(aString);
  ASSERTSTR (sizeof(uint64)==sizeof(unsigned long long),
             "strToUint64: sizeof(uint64) cannot be handled");
  const char* str = aString.c_str();
  int st  = lskipws(aString, 0, aString.size());
  int end = rskipws(aString, st, aString.size());
  char* endPtr;
  unsigned long long val;
  // Clear errno since strtoull does not do it.
  errno = 0;
  // We don't want octal values, so use base 10 unless a hex value is given.
  if (end > st+2  &&  str[st] == '0'  &&  (str[st+1]=='x' || str[st+1]=='X')) {
    val = strtoull (str+st, &endPtr, 0);
  } else {
    val = strtoull (str+st, &endPtr, 10);
  }
  ASSERTSTR (endPtr == str+end, aString << " is not an integer value");
  ASSERTSTR (errno != ERANGE  &&  errno != EINVAL,
             aString << " is invalid or outside long long uint range");
  return val;
}
#endif

 
//
// compactedArrayString(string)
//
// Given een array string ( '[ xx, xx, xx ]' ) this utility compacts the string
// by replacing series with range.
// Eg. [ lii001, lii002, lii003, lii005 ] --> [ lii001..lii003, lii005 ]
//
// ----------------------- ATTENTION !!!----------------------------------
// This routine has been copied to the Navigator software
// (MAC/Navigator/scripts/libs/nav_usr/CS1/CS1_Common.ctl)
// if you change anything structural change the Navigator part also please
// -----------------------------------------------------------------------

string compactedArrayString(const string&	orgStr)
{
	string	baseString(orgStr);			// destroyable copy
	ltrim(baseString, " 	[");		// strip of brackets
	rtrim(baseString, " 	]");

	// and split into a vector
	vector<string>	strVector = StringUtil::split(baseString, ',');
	if (strVector.size() < 2) {
		return (orgStr);
	}

	// note: we assume that the format of each element is [xxxx]9999
	string::size_type	firstDigit(strVector[0].find_first_of("0123456789"));
	if (firstDigit == string::npos) {	// no digits? then return org string
		return (orgStr);
	}
	// construct masks for scanning and formatting.
	string	elemName	 (strVector[0].substr(0,firstDigit));
	string	scanMask	 (elemName+"%ld");
	string	rangeScanMask(scanMask+".."+scanMask);
	bool	rangeElem 	 (strVector[0].find("..",0) != string::npos);
	int		numberLength (strVector[0].length() - elemName.length());
	if (rangeElem) {
		numberLength = (numberLength-2)/2;
	}
	string	outMask 	 (formatString("%s%%0%dld", elemName.c_str(), numberLength));

	// process all elements in the vector.
	string 	result("[");
	long 	prevValue(-2);
	bool	firstElem(true);
	bool	endOfArray(false);
	int		elemsInRange(0);
	int		nrElems(strVector.size());
	for (int idx = 0; idx < nrElems; idx++) {
		long	value;
		long	lastValue;
		if (sscanf(strVector[idx].c_str(), scanMask.c_str(), &value) != 1) {
			LOG_DEBUG_STR("Element " << strVector[idx] << " does not match mask " 
						<< scanMask << ". Returning orignal string.");
			return (orgStr);
		}

		// check for already compacted elements.
		rangeElem = (strVector[idx].find("..",0) != string::npos);
		if (rangeElem && (sscanf(strVector[idx].c_str(), rangeScanMask.c_str(), &value, &lastValue) != 2)) {
			LOG_DEBUG_STR("RangeElement " << strVector[idx] << " does not match mask " 
						<< rangeScanMask << ". Returning orignal string.");
			return (orgStr);
		}
			
		// contiquous numbering?
		if (value == prevValue+1) {
			elemsInRange += rangeElem ? (lastValue-value+1) : 1;
			prevValue    =  rangeElem ? lastValue : value;
			if (idx < nrElems-1) {		// when elements left
				continue;
			}
			endOfArray = true;
		}

		// list not contiguous anymore, write results we collected
		if (firstElem) {				// don't start with a comma.
			result += formatString(outMask.c_str(), value);
		}

		if (elemsInRange == 1) {
			result += "," + formatString(outMask.c_str(), value);
		}
		else {
			if (elemsInRange == 2) {		// don't compact xx03,xx04 to xx03..xx04
				result += "," + formatString(outMask.c_str(), prevValue);
			}
			else if (elemsInRange > 2) {
				result += ".." + formatString(outMask.c_str(), prevValue);
			}
			if (!firstElem && !endOfArray) {
				result += "," + formatString(outMask.c_str(), value);
				if (rangeElem) {
					result += ".." + formatString(outMask.c_str(), lastValue);
				}
			}
		}
		elemsInRange = rangeElem ? (lastValue-value+1) : 1;
		prevValue    = rangeElem ? lastValue : value;
		firstElem 	 = false;
	}

	return (result+"]");
}

string stripBraces(const string& orgStr)
{
  string baseString(orgStr); // destroyable copy
  ltrim(baseString, " 	["); // strip of brackets
  rtrim(baseString, " 	]");
  
  return baseString;
}

rangeElementTable buildRangeElementTable()
{
  rangeElementTable table;
  table.insert(std::make_pair("(", PARENTHESIS));
  table.insert(std::make_pair("*", ASTERISK));
  table.insert(std::make_pair("..", DOTDOT));
	
  return table;
}

string findRangeElement(const string& orgStr)
{
  if (orgStr.find("(",0) != string::npos)
    return "(";
  if (orgStr.find("*",0) != string::npos)
    return "*";
  if (orgStr.find("..",0) != string::npos)
    return "..";
  
  return orgStr;
}

//
// expandedArrayString(string)
//
// Given een array string ( '[ xx..xx, xx ]' ) this utility expands the string
// by replacing ranges with the fill series.
// Eg. [ lii001..lii003, lii005 ] --> [ lii001, lii002, lii003, lii005 ]
//     [ 10*0         ] --> [ 0,0,0,0,0,0,0,0,0,0 ]
//     [ 3*(0;1;2;3)  ] --> [ 0,1,2,3,0,1,2,3,0,1,2,3 ]
//     [ 3*(300..303) ] --> [ 300,301,302,303,300,301,302,303,300,301,302,303 ]
//     [ 2*(5*0)      ] --> [ 0,0,0,0,0,0,0,0,0,0 ]       

// ----------------------- ATTENTION !!!----------------------------------
// This routine has been copied to the Navigator software
// (MAC/Navigator/scripts/libs/nav_usr/CS1/CS1_Common.ctl)
// if you change anything structural change the Navigator part also please
// -----------------------------------------------------------------------
string expandRangeString (const string& strng)
{
  string str(strng);
  uint i=0;
  uint last = str.size();
  while (i < last-1) {
    if (str[i] == '\''  ||  str[i] == '"') {
      // Ignore a quoted part.
      string::size_type pos = str.find (str[i], i+1);
      ASSERTSTR (pos != string::npos, "Unbalanced quoted string at position "
                 << i << " in " << str);
      i = pos;
    } else if (str[i] == '.'  && str[i+1] == '.') {
      // Found ..; look back for number and prefix.
      // First find number.
      int endnum = rskipws (str, 0, i);
      int stnum = endnum - 1;
      while (stnum >= 0  &&  isdigit(str[stnum])) {
        --stnum;
      }
      int lennum = endnum - stnum - 1;
      if (lennum > 0) {
        int num = strToInt (str.substr (stnum+1, lennum));
        // Found number, now find possible prefix.
        // We could say that the prefix has to be alphanumeric, but more
        // general is to accept all characters except ([,*; blank and tab.
        int stalp = stnum;
        while (stalp>=0 &&
               str[stalp]!='(' && str[stalp]!='[' &&
               str[stalp]!=',' && str[stalp]!=';' && str[stalp]!='*' &&
               str[stalp]!=' ' && str[stalp]!='\t') {
          --stalp;
        }
        stalp++;
        string prefix = str.substr (stalp, stnum-stalp+1);
        string suffix;
        // Now find part after the .. which can contain the same prefix.
        // if no parenthesis was used.
        i = lskipws (str, i+2, last);
        if (prefix.size() > 0  &&  last-i > prefix.size()  &&
            str.substr(i, prefix.size()) == prefix) {
          i += prefix.size();
        }
        // Test if a digit.
        if (isdigit(str[i])) {
          stnum = i;
          // Skip to end of number.
          while (i < last  &&  isdigit(str[i])) {
            ++i;
          }
          endnum = strToInt (str.substr(stnum, i-stnum));
          // We really have something like xxx000..004
          // Find a possible suffix.
          uint stsuf = i;
          while (i < last  &&
               str[i]!=')' && str[i]!=']' &&
               str[i]!=',' && str[i]!=';' && str[i]!='*' &&
               str[i]!=' ' && str[i]!='\t') {
            ++i;
          }
          if (i > stsuf) {
            suffix = str.substr (stsuf, i-stsuf);
          }
          // Removes braces if the prefix ends and suffix starts with it.
          int lpre = prefix.size();
          int lsuf = suffix.size();
          if (lpre>0 && lsuf>0 && prefix[lpre-1]=='{' && suffix[0]=='}') {
            prefix = prefix.substr(0, lpre-1);
            suffix = suffix.substr(1, lsuf-1);
          }
          // Fill it in in ascending or descending order.
          ostringstream ostr;
          if (num < endnum) {
            for (; num<=endnum; ++num) {
              ostr << prefix << setfill('0') << setw(lennum) << num << suffix;
              if (num != endnum) ostr << ',';
            }
          } else {
            for (; num>=endnum; --num) {
              ostr << prefix << setfill('0') << setw(lennum) << num << suffix;
              if (num != endnum) ostr << ',';
            }
          }
          str.replace (stalp, i-stalp, ostr.str());
          int diff = ostr.str().size() - (i-stalp);
          last += diff;
          // Start again, because something like aa00..01bcd00..03ef might be used.
          i = stalp-1;
        }
      }
    }
    ++i;
  }
  return str;
}

string expandMultString (const string& strng)
{
  string str(strng);
  uint i=0;
  uint last = str.size();
  while (i < last-1) {
    if (str[i] == '\''  ||  str[i] == '"') {
      // Ignore a quoted part.
      i = skipQuoted (str, i);
    } else {
      if (str[i] != '*') {
        ++i;
      } else {
        // Found *; look back for digits.
        int endnum = rskipws (str, 0, i);
        int stnum = endnum - 1;
        while (stnum >= 0  &&  isdigit(str[stnum])) {
          --stnum;
        }
        stnum++;
        // Only use it if the number is at begin or preceeded by a delimiter.
        uint j = rskipws(str, 0, stnum);
        int lennum = 0;
        if (j==0  ||  str[j-1]==','  ||  str[j-1]=='['  ||  str[j-1]=='(') {
          lennum = endnum - stnum;
        }
        if (lennum == 0) {
          // No number found, so ignore the *.
          ++i;
        } else {
          bool continueAtReplace = true;
          int num = strToInt (str.substr (stnum, lennum));
          // Now find the string that has to be multiplied.
          // This can be a single instance or a set indicated by () or [].
          // First skip possible whitespace after *.
          i = lskipws (str, i+1, last);
          uint stval = i;
          string val;
          if (str[i] == '[') {
            // A set in [].
            i = skipBalanced (str, i, last, ']');
            val = str.substr(stval, i-stval);
          } else if (str[i] == '(') {
            // A set in (). Remove the parentheses.
            i = skipBalanced (str, i, last, ')');
            val = str.substr(stval+1, i-stval-2);
            // Replace ; by , (for backward compatibility).
            uint stv = 0;
            while (stv < val.size()) {
              if (val[stv] == '"'  ||  val[stv] == '\'') {
                stv = skipQuoted (val, stv);
              } else {
                if (val[stv] == ';') {
                  val[stv] = ',';
                }
                stv++;
              }
            }
          } else {
            // Any other value is ended by ,]).
            while (i < last) {
              if (str[i] == '"'  ||  str[i] == '\'') {
                // A quoted string.
                i = skipQuoted (str, i);
              } else if (str[i] == '[') {
                // Another string in brackets.
                i = skipBalanced (str, i, last, ']');
              } else if (str[i] == '(') {
                // Another string in parentheses.
                i = skipBalanced (str, i, last, ')');
              } else {
                if (str[i]==',' || str[i]==']' || str[i]==')') {
                  // The end of the value.
                  break;
                }
                // Continue with next character.
                ++i;
              }
            }
            val = str.substr(stval, i-stval);
            ///            continueAtReplace = false;
          }
          // Insert the values num times separated by a comma.
          string res;
          res.reserve (num * (val.size() + 1));
          for (int j=0; j<num; ++j) {
            if (j > 0) res += ',';
            res += val;
          }
          // Replace the value by the new result.
          str.replace (stnum, i-stnum, res);
          last += res.size() - (i-stnum);
          // Continue scanning at start of replace (for possible recursion).
          if (continueAtReplace) {
            i = stnum;
          } else {
            i = stnum + res.size();
          }
        }
      }
    }
  }
  return str;
}

uint skipQuoted (const string& str, uint st)
{
  string::size_type pos = str.find (str[st], st+1);
  ASSERTSTR (pos != string::npos, "Unbalanced quoted string at position "
             << st << " in " << str);
  return pos+1;
}

uint skipBalanced (const string& str, uint st, uint end, char endChar)
{
  char ch = str[st++];
  int nrp = 1;
  while (st < end) {
    // First test on end character. In this way it also works well
    // if start and end character are the same.
    if (str[st] == endChar) {
      st++;
      if (--nrp == 0) return st;
    } else if (str[st] == ch) {
      st++;
      nrp++;
    } else if (str[st] == '"'  ||  str[st] == '\'') {
      st = skipQuoted (str, st);
    } else {
      st++;
    }
  }
  THROW (Exception, "Unbalanced " << ch << endChar << " in " << str);
}

string expandArrayString (const string& str)
{
  // Only do it if enclosed in brackets.
  uint st  = lskipws (str, 0, str.size());
  uint end = rskipws (str, st, str.size());
  if (st >= end  ||  str[st] != '['  ||  str[end-1] != ']') {
    return str;
  }
  // Do expandMult first, otherwise something like 3*lifs001..003 is not
  // handled properly.
  return expandRangeString (expandMultString (str));
}

} // namespace LOFAR
