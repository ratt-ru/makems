//#  ParameterSet.cc: Implements a map of Key-Value pairs.
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
//#  $Id: ParameterSetImpl.cc 12371 2008-12-23 13:18:31Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/ParameterSetImpl.h>
#include <Common/Exceptions.h>
#include <Common/LofarLogger.h>
#include <Common/lofar_fstream.h>
#include <Common/StreamUtil.h>
#include <Common/StringUtil.h>
#include <string>
#include <cstdio>

using namespace std;

namespace LOFAR {

//-------------------------- creation and destroy ---------------------------
//
// Default constructor
//
ParameterSetImpl::ParameterSetImpl(KeyCompare::Mode	mode)
	: KVMap(mode),
	  itsCount (1),
	  itsMode(mode)
{}

//
// Construction by reading a parameter file.
//
ParameterSetImpl::ParameterSetImpl(const string&	theFilename,
				   KeyCompare::Mode	mode)
	: KVMap(mode), 
	  itsCount (1),
	  itsMode(mode)
{
	readFile(theFilename, "", false);
}

//
//	Destructor
//
ParameterSetImpl::~ParameterSetImpl()
{}

//
// operator<<
//
std::ostream&	operator<< (std::ostream& os, const ParameterSetImpl &thePS)
{
	thePS.writeStream(os);
	return os;
}

//-------------------------- merge and split --------------------------------
//
// makeSubset(baseKey [, prefix])
//
// Creates a Subset from the current ParameterSetImpl containing all the 
// parameters that start with the given baseKey. 
// The baseKey is cut off from the Keynames in the created subset, the 
// optional prefix is put before the keynames.
//
ParameterSetImpl* 
ParameterSetImpl::makeSubset(const string& baseKey, 
                             const string& prefix) const
{
  // Convert \a baseKey to lowercase, if we need to do case insensitve compare.
  string            base   = (itsMode == KeyCompare::NOCASE) ? toLower(baseKey) : baseKey;
  ParameterSetImpl* subSet = new ParameterSetImpl(itsMode);
  iterator          pos    = subSet->begin();

  LOG_TRACE_CALC_STR("makeSubSet(" << baseKey << "," << prefix << ")");

  // Start scanning at the point where \a base might first occur.
  for (const_iterator it = lower_bound(base); it != end(); ++it) {

    bool match = (itsMode == KeyCompare::NOCASE) ?
      toLower(it->first).compare(0, base.size(), base) == 0 :
      it->first.compare(0, base.size(), base) == 0;

    // We can stop scanning once \a match becomes false, since keys are sorted.
    if (!match) break;

    LOG_TRACE_VAR_STR(base << " matches with " << it->first);
    // cut off baseString and copy to subset
    pos = subSet->insert(pos, make_pair(prefix + it->first.substr(base.size()),
                                        it->second));
  }
  
  return (subSet);
}

//
// subtractSubset(fullPrefix)
//
// Removes all the keys the start with the given prefix from the parset.
//
void ParameterSetImpl::subtractSubset(const string& fullPrefix) 
{
	LOG_TRACE_CALC_STR("subtractSubSet(" << fullPrefix << ")");

	// Convert \a baseKey to lowercase, if we need to do case insensitve compare.
	string	prefix = (itsMode == KeyCompare::NOCASE) ? toLower(fullPrefix) : fullPrefix;
	int		length = prefix.size();

	// Loop over parset and delete the matching keys.
	iterator	endIter  = end();
	iterator	iter     = lower_bound(prefix);
	while (iter != endIter) {
		bool match = (itsMode == KeyCompare::NOCASE) ?
					toLower(iter->first).compare(0, length, prefix) == 0 :
					iter->first.compare(0, length, prefix) == 0;

		// We can stop scanning once a match becomes false, since keys are sorted.
		if (!match) {
			return;
		}

		// erase the matching element
		iterator	old_iter = iter;
		iter++;
		erase (old_iter);
	}
}

//
// adoptFile
//
// Adds the parameters from the file to the current ParameterSetImpl.
//
void ParameterSetImpl::adoptFile(const string&	theFilename,
				 const string&	thePrefix)
{
	readFile(theFilename, thePrefix, true);
}

//
// adoptBuffer
//
// Adds the parameters from the string to the current ParameterSetImpl.
//
void ParameterSetImpl::adoptBuffer(const string&	theBuffer,
				   const string&	thePrefix)
{
	readBuffer(theBuffer, thePrefix, true);
}

//
// adoptCollection
//
// Adds the parameters from the ParColl. to the current ParameterSetImpl.
//
void ParameterSetImpl::adoptCollection(const ParameterSetImpl& theCollection,
				       const string&	thePrefix)
{
    // Cannot adopt itself.
    if (&theCollection != this) {
	const_iterator		newItem = theCollection.begin();

	while (newItem != theCollection.end()) {
		replace(thePrefix+newItem->first, newItem->second);
		++newItem;
	}
    }
}

//
// readFile
// (private)
//
// Disentangles the file and adds the Key-Values pair to the current ParameterSetImpl.
//
void ParameterSetImpl::readFile(const	string&	theFilename, 
				const	string&	prefix,
				const	bool	merge)
{
	ifstream		paramFile;

	// Try to pen the file
	paramFile.open(theFilename.c_str(), ifstream::in);
	if (!paramFile) {
		THROW (APSException, 
		       formatString("Unable to open file %s", theFilename.c_str()));
	}

	if (paramFile.eof()) {
		THROW (APSException, 
		       formatString("file %s is empty", theFilename.c_str()));
	}

	readStream(paramFile, prefix, merge);

	paramFile.close();
}

//
// readBuffer
// (private)
//
// Disentangles the file and adds the Key-Values pair to the current ParameterSetImpl.
//
void ParameterSetImpl::readBuffer(const	string&	theBuffer, 
				  const	string&	prefix,
				  const	bool	merge)
{
	istringstream		iss(theBuffer, istringstream::in);
	readStream(iss, prefix, merge);
}

void ParameterSetImpl::readStream (istream& inputStream, 
                                   const string& prefix,
                                   bool	merge)
{
  // Define key and value.
  string value;
  string key;
  // Read first line.
  string line;
  getline (inputStream, line);
  while (inputStream) {
    // Skip leading and trailing whitespace.
    uint st  = lskipws (line, 0, line.size());
    if (line[st] != '#') {                         // skip if only comment
      uint end = rskipws (line, st, line.size());
      if (st < end) {                              // skip empty line
        bool squote = false;            // In a single quoted string?
        bool dquote = false;            // In a double quoted string?
        bool quote  = false;            // In a quoted string?
        uint nonbl  = st;               // Position of last non-blank character
        uint stval  = st;               // Start of value
        for (uint i=st; i<end; ++i) {
          if (!dquote) {
            if (line[i] == '\'') {
              squote = !squote;
              quote  = squote;
            }
          }
          if (!squote) {
            if (line[i] == '"') {
              dquote = !dquote;
              quote  = dquote;
            }
          }
          if (!quote) {
            if (line[i] == '#') {
              end = rskipws(line, st, i);         // A comment ends the line
            } else if (line[i] == '=') {
              if (! key.empty()) {
                addMerge (key, value, merge);   // Add previous key/value to map
                value.erase();
              }
              key = prefix + line.substr (st, nonbl-st+1);   // New key
              stval = i+1;
            } else if (line[i] != ' '  &&  line[i] != '\t') {
              nonbl = i;                        // Position of last non-blank
            }
          }
        }
        if (quote) {
          THROW (APSException, "Unbalanced quotes in " + line);
        }
        // Skip possible whitespace before the value.
        // A trailing backslash is processed for backward compatibility.
        if (line[end-1] == '\\') {
          end = rskipws(line, stval, end-1);
        }
        stval = lskipws(line, stval, end);
        if (stval < end) {
          // Append the line's non-empty value to the value.
          if (value.empty()) {
            value = line.substr(stval, end-stval);
          } else {
            // If both quoted, remove last and first quote.
            // This is just like C where continuated strings have to be
            // enclosed in quotes on all lines.
            // Give an error if one is quoted and the other not.
            if ((line[stval] == '"'          || line[stval] == '\'')  !=
                (value[value.size()-1] =='"' || value[value.size()-1] =='\'')) {
              THROW (APSException, "All value lines need to be quoted around "
                     "continuation line " + line);
            }
            if (line[stval] == '"'  ||  line[stval] == '\'') {
              value = value.substr (0, value.size()-1) +
                line.substr(stval+1, end-stval-1);
            } else {
              value += ' ';
              value += line.substr(stval, end-stval);
            }
          }
        }
      }
    }
    // Get next line.
    getline (inputStream, line);
  }
  // Add last key.
  if (! key.empty()) {
    addMerge (key, value, merge);
  }
}


void ParameterSetImpl::addMerge (const string& key,
                                 const string& value,
                                 bool merge)
{
  // remove any existed value and insert this value
  if ((erase(key) > 0)  &&  !merge) {
    LOG_WARN ("Key " + key + " is defined twice; ignoring first value");
  }
  add (key, ParameterValue(value));
}

//------------------------- single pair functions ----------------------------
//
// findKV(key) [private]
//
ParameterSetImpl::const_iterator
ParameterSetImpl::findKV(const string& aKey, bool doThrow) const
{
	LOG_TRACE_CALC_STR("find(" << aKey << ")");

	const_iterator	iter = find(aKey);
	if (iter == end() && doThrow) {
		THROW (APSException, formatString("Key %s unknown", aKey.c_str()));
	}

	return (iter);
}

//
// add (key, value)
//
void ParameterSetImpl::add(const string& aKey, const ParameterValue& aValue)
{
  if (!insert(make_pair(aKey, aValue)).second) {
    THROW (APSException, "add: Key " + aKey + " double defined?"); 
  }
}

//
// replace (key, value)
//
void ParameterSetImpl::replace(const string& aKey, const ParameterValue& aValue)
{
  (*this)[aKey] = aValue;
}

//
// remove (key)
//
void ParameterSetImpl::remove(const string& aKey)
{
  // remove any existed value
  erase(aKey);
}

//
//-------------------------- retrieve functions -----------------------------
// getName
//
//string	ParameterSetImpl::getName() const
//{
//	string fullKeyName = begin()->first;
//	char*	firstPoint = strchr(fullKeyName.c_str(), '.');
//
//	return(fullKeyName.substr(0, firstPoint - fullKeyName.c_str()));
//}

//
// getVersionNr
//
//string	ParameterSetImpl::getVersionNr() const
//{
//	const_iterator	iter = find(getName()+"."+PC_KEY_VERSIONNR);
//
//	if (iter != end()) {
//		return (iter->second);
//	}
//	
//	return("");
//}

#define PARAMETERSETIMPL_GETVECTOR(TPC,TPL) \
vector<TPL> ParameterSetImpl::get##TPC##Vector(const string& aKey, \
                                               bool expandable) const \
{ \
  ParameterValue value (findKV(aKey)->second); \
  if (expandable) value = value.expand(); \
  return value.get##TPC##Vector(); \
} \
 \
vector<TPL> ParameterSetImpl::get##TPC##Vector(const string& aKey, \
                                               const vector<TPL>& aValue, \
                                               bool expandable) const   \
{ \
  const_iterator it = findKV(aKey,false); \
  if (it == end()) return aValue; \
  ParameterValue value (it->second); \
  if (expandable) value = value.expand(); \
  return value.get##TPC##Vector(); \
}

PARAMETERSETIMPL_GETVECTOR (Bool, bool)
PARAMETERSETIMPL_GETVECTOR (Int, int)
PARAMETERSETIMPL_GETVECTOR (Uint, uint)
PARAMETERSETIMPL_GETVECTOR (Int16, int16)
PARAMETERSETIMPL_GETVECTOR (Uint16, uint16)
PARAMETERSETIMPL_GETVECTOR (Int32, int32)
PARAMETERSETIMPL_GETVECTOR (Uint32, uint32)
#ifdef HAVE_LONG_LONG
PARAMETERSETIMPL_GETVECTOR (Int64, int64)
PARAMETERSETIMPL_GETVECTOR (Uint64, uint64)
#endif
PARAMETERSETIMPL_GETVECTOR (Float, float)
PARAMETERSETIMPL_GETVECTOR (Double, double)
PARAMETERSETIMPL_GETVECTOR (String, string)
PARAMETERSETIMPL_GETVECTOR (Time, time_t)


//---------------------------- save functions -------------------------------
//
// writeFile
//
// Writes the Key-Values pair from the current ParameterSetImpl to the given file
// thereby overwritting any file contents.
//
void ParameterSetImpl::writeFile(const string&	theFilename,
								    bool			append) const
{
	ofstream		paramFile;

	// Try to open the file
	LOG_TRACE_STAT_STR("Writing parameter file `" << theFilename << "'");
	paramFile.open(theFilename.c_str(), 
				   ofstream::out | (append ? ofstream::app : ofstream::trunc));
	if (!paramFile) {
		THROW (APSException, formatString("Unable to open file %s", theFilename.c_str()));
	}

	// Write all the pairs to the file
	writeStream(paramFile);

	// Close the file
	paramFile.close();
}

//
// writeBuffer
//
// Writes the Key-Values pair from the current ParameterSetImpl to the given 
// string.
//
void ParameterSetImpl::writeBuffer(string&	aBuffer) const
{
	ostringstream oss;
	writeStream(oss);
	aBuffer = oss.str();
}

//
// writeStream
//
// Writes the Key-Value pairs from the current ParameterSetImpl to the given
// output stream.
//
void ParameterSetImpl::writeStream(ostream&	os) const
{
	// Write all the pairs to the file
	const_iterator		curPair = begin();
	while (curPair != end()) {
		// Key can always be written.
		os << curPair->first << "=";
                os << curPair->second.get() << endl;
		curPair++;
	}
}


//
// isValidVersionNr(versionNr)
//
// Check format of versionnumber.
//
bool isValidVersionNr   (const string& versionNr)
{
	int		release, update, patch;
	char	toomuch[20];

	return (sscanf(versionNr.c_str(), "%d.%d.%d%10s", 
					&release, &update, &patch, toomuch) == 3);
}

//
// isValidVersionNrRef(versionNr)
//
// Check format of versionnumber, used as a reference.
//
bool isValidVersionNrRef(const string& versionNr)
{
	return (isValidVersionNr(versionNr) || (versionNr == PC_QUAL_STABLE) || 
		    (versionNr == PC_QUAL_TEST) || (versionNr == PC_QUAL_DEVELOP));

}

#if 0
//
// seqNr(aString)
//
// Check is given string is a valid sequencenumber
//
uint32	seqNr(const string& aString)
{
	int32	theNumber;

	sscanf(aString.c_str(), "%d", &theNumber);

	if (theNumber <= 0) {
		return (0);
	}

	return (theNumber);
}
#endif

//
// keyName(fullKeyName)
//
// Returns the real name of the key (without the module hierachy)
//
string keyName(const string& fullKeyName)
{
	string::size_type lastPoint = fullKeyName.rfind('.');
	if (lastPoint == string::npos) 
		return fullKeyName;
	else 
		return fullKeyName.substr(lastPoint+1);
}


//
// moduleName(fullKeyName)
//
// Returns the module hierarchy of key
//
string moduleName(const string& fullKeyName)
{
	string::size_type lastPoint = fullKeyName.rfind('.');
	if (lastPoint == string::npos) 
		return "";
	else 
		return fullKeyName.substr(0, lastPoint);
}

//
// keyPart(parameterline)
//
// Returns the key part of a parameter line.
//
string	keyPart	  (const string& parameterLine)
{
	string::size_type firstEqual = parameterLine.find('=');
	if (firstEqual == string::npos)
		return parameterLine;
	else
		return parameterLine.substr(0, firstEqual);
}

// Returns the value of a parameterline
string	valuePart   (const string& parameterLine)
{
	string::size_type firstEqual = parameterLine.find('=');
	if (firstEqual == string::npos)
		return parameterLine;
	else
		return parameterLine.substr(firstEqual+1);
}

// Returns the value of the index if the string contains an index otherwise
// 0 is returned. The second string contains the opening and closing chars
// that are used to indicate the index. The index must be a literal value
// not an expression.
int32 	indexValue (const string&	label, char	indexMarker[2])
{
	string::size_type	start = label.find_last_of(indexMarker[0]);
	if (start == string::npos) {
		return (0);
	}

	string::size_type	end = label.find(indexMarker[1], start);
	if (end == string::npos) {
		return(0);
	}

	return (strtol(label.data()+start+1, 0 ,0));

}

//
// locateModule(shortKey)
//
// Searches for a key ending in the given 'shortkey' and returns it full name.
// e.g: a.b.c.d.param=xxxx --> locateKey(d)-->a.b.c.
string	ParameterSetImpl::locateModule(const string&	shortKey) const
{
	const_iterator		iter = begin();
	const_iterator		eom  = end();
	while ((iter != eom)) {
		if (keyName(moduleName(iter->first)) == shortKey) {
			string prefix = moduleName(moduleName((iter->first)));
			if (prefix.length() > 0) {
				prefix += ".";
			}
			return (prefix);
		}
		iter++;
	}

	return ("");
}

} // namespace LOFAR
