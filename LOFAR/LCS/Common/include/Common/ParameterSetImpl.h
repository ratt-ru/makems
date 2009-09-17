//#  ParameterSetImpl.h: Implements a map of Key-Value pairs.
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
//#  Note: This source is read best with tabstop 4.
//#
//#  $Id: ParameterSetImpl.h 13829 2009-08-24 09:17:42Z romein $

#ifndef LOFAR_COMMON_PARAMETERSETIMPL_H
#define LOFAR_COMMON_PARAMETERSETIMPL_H

// \file
// Implements a map of Key-Value pairs.

//# Never #include <config.h> or #include <lofar_config.h> in a header file!
//# Includes
#include <Common/ParameterValue.h>
#include <Common/LofarTypes.h>
#include <Common/lofar_map.h>
#include <Common/lofar_string.h>
#include <Common/lofar_vector.h>
#include <Common/lofar_iostream.h>
#include <Common/lofar_sstream.h>
#include <Common/StringUtil.h>

namespace LOFAR {

// \addtogroup Common
// @{

const char PC_QUAL_STABLE[]   = { "stable"        };
const char PC_QUAL_TEST[]     = { "test"          };
const char PC_QUAL_DEVELOP[]  =	{ "development"   };
const char PC_KEY_VERSIONNR[] = { "versionnr"     };
const char PC_KEY_QUAL[]      =	{ "qualification" };

// The StringUtil::Compare functor will be used for key comparison.
typedef StringUtil::Compare			KeyCompare;

// A key/value map is defined as a map of strings. The third template
// parameter, \c KeyCompare, is the string comparison functor that will be
// used to compare keys.
typedef map <string, ParameterValue, KeyCompare>	KVMap;

//# Description of class.
// The ParameterSetImpl class is a key-value implementation of the type
// map<string, string, KeyCompare>. 
// This means that values are stored as a string which allows easy merging and
// splitting of ParameterSetImpls because no conversions have to be done.
// A couple of getXxx routines are provided to convert the strings to the 
// desired type.
//
class ParameterSetImpl : public KVMap
{
public:
	typedef KVMap::iterator		iterator;
	typedef KVMap::const_iterator	const_iterator;

	// \name Construction and Destruction
	// A ParameterSetImpl can be constructed as empty collection, can be
	// read from a file or copied from another collection. 
	// @{

	// Create an empty collection. The argument \a mode determines how
	// keys should be compared.
	 explicit ParameterSetImpl(KeyCompare::Mode	mode);

	// Destroy the contents.
	~ParameterSetImpl();

	// Construct a ParameterSet from the contents of \a theFilename. The
	// argument \a mode determines how keys should be compared.
	explicit ParameterSetImpl(const string&		theFilename,
				  KeyCompare::Mode	mode);
	// @}

	// Deal with the reference count.
	// @{
	ParameterSetImpl* incrCount()
	  { itsCount++; return this; }
	int decrCount()
	  { return --itsCount; }
	// @}

        // Get the ParameterValue.
        // @{ 
        const ParameterValue& get (const string& aKey) const;

	// Return the key comparison mode.
	KeyCompare::Mode keyCompareMode() const { return itsMode; }


	// \name Merging or appending collections
	// An existing collection can be extended/merged with another collection.
	// @{

	// Adds the Key-Values pair in the given file to the current
	// collection. Each key will be prefixed with the optional argument \a
	// thePrefix.
	void	adoptFile      (const string&               theFilename,
				const string&               thePrefix = "");

	// Adds the Key-Values pair in the given buffer to the current
	// collection. Each key will be prefixed with the optional argument \a
	// thePrefix.
	void	adoptBuffer    (const string&               theBuffer,
				const string&               thePrefix = "");

	// Adds the Key-Values pair in the given collection to the current 
	// collection. Each key will be prefixed with the optional argument \a
	// thePrefix.
	void	adoptCollection(const ParameterSetImpl&     theCollection,
				const string&               thePrefix = "");
	// @}


	// \name Saving the collection
	// The map of key-value pair can be saved in a file or a string.
	// @{

	// Writes the Key-Values pair from the current ParCollection to the file.
	void	writeFile   (const string& theFilename, bool append = false) const;

	// Writes the Key-Values pair from the current ParCollection to the 
	// string buffer.
	void	writeBuffer (      string& theBuffer) const;

	// Write the Key-Value pairs from the current ParCollection to the
	// output stream.
	void	writeStream (ostream&	os)	const;
	//@}

	// \name Handle subsets
	// A subset from the current collection can be made based on the prefix
	// of the keys in the collection.
	// @{

	// Creates a subset from the current ParameterSetImpl containing all the 
	// parameters that start with the given baseKey. The baseKey is cut off 
	// from the Keynames in the created subset, the optional prefix is put
	// before all keys in the subset.
	ParameterSetImpl*	makeSubset(const string& baseKey,
								   const string& prefix = "") const;

	// Subtract a subset from the current ParameterSet. Every parameter
	// whose key starts with the given name will be removed from the
	// ParameterSet.
	void	subtractSubset(const string& fullPrefix);
	// @}

	
	// \name Handling single key-value pairs
	// Single key-value pairs can ofcourse be added, replaced or removed from 
	// a collection.
	// @{

	// Add the given pair to the collection. When the \c aKey already exist 
	// in the collection an exception is thrown.
	void	add    (const string& aKey, const ParameterValue& aValue);

	// Replaces the given pair in the collection. If \c aKey does not exist in
	// the collection the pair is just added to the collection.
	void	replace(const string& aKey, const ParameterValue& aValue);

	// Removes the pair with the given key. Removing a non-existing key is ok.
	void	remove (const string& aKey);
	// @}

	// \name Searching and retrieving
	// The following functions support searching the collection for existance
	// of given keys an the retrieval of the corresponding value. In the getXxx
	// retrieve functions the stored string-value is converted to the wanted
	// type.
	// @{

	// Checks if the given Key is defined in the ParameterSetImpl.
	bool	isDefined (const string& searchKey) const
				{ return (find(searchKey) != end()); };

	// Searches for a module whose name end in the given modulename.
	// e.g: a.b.c.d.param=xxx ; locateModule('d') --> 'a.b.c.'
	string	locateModule(const string&	shortName) const;

	// Return the 'metadata' from the parameterCollection.
//	string  getName          () const;

	// Return the 'metadata' from the parameterCollection.
//	string  getVersionNr     () const;

	// Return scalar value.
	// @{
	bool	getBool  (const string& aKey) const;
        bool	getBool  (const string& aKey, bool aValue) const;
	int	getInt   (const string& aKey) const;
        int	getInt   (const string& aKey, int aValue) const;
	uint	getUint  (const string& aKey) const;
        uint	getUint  (const string& aKey, uint aValue) const;
	int16	getInt16 (const string& aKey) const;
        int16	getInt16 (const string& aKey, int16 aValue) const;
	uint16	getUint16(const string& aKey) const;
        uint16	getUint16(const string& aKey, uint16 aValue) const;
	int32	getInt32 (const string& aKey) const;
        int32	getInt32 (const string& aKey, int32 aValue) const;
	uint32	getUint32(const string& aKey) const;
        uint32	getUint32(const string& aKey, uint32 aValue) const;
#if HAVE_LONG_LONG
	int64	getInt64 (const string& aKey) const;
        int64	getInt64 (const string& aKey, int64 aValue) const;
	uint64	getUint64(const string& aKey) const;
        uint64	getUint64(const string& aKey, uint64 aValue) const;
#endif
	float	getFloat (const string& aKey) const;
        float	getFloat (const string& aKey, float aValue) const;
	double	getDouble(const string& aKey) const;
        double	getDouble(const string& aKey, double aValue) const;
	string	getString(const string& aKey) const;
        string	getString(const string& aKey, const string& aValue) const;
	// Returns the value as a time value (seconds since 1970).
	// @{
	time_t	getTime  (const string& aKey) const;
        time_t	getTime  (const string& aKey, const time_t& aValue) const;
	// @}
	// @}

	// Return vector of values.
	// @{
        vector<bool>	getBoolVector  (const string& aKey,
                                        bool expandable) const;
        vector<bool>	getBoolVector  (const string& aKey,
                                        const vector<bool>& aValue,
                                        bool expandable) const;
        vector<int>	getIntVector   (const string& aKey,
                                        bool expandable) const;
        vector<int>	getIntVector   (const string& aKey,
                                        const vector<int>& aValue,
                                        bool expandable) const;
        vector<uint>	getUintVector  (const string& aKey,
                                        bool expandable) const;
        vector<uint>	getUintVector  (const string& aKey,
                                        const vector<uint>& aValue,
                                        bool expandable) const;
	vector<int16>	getInt16Vector (const string& aKey,
                                        bool expandable) const;
        vector<int16>	getInt16Vector (const string& aKey,
                                        const vector<int16>& aValue,
                                        bool expandable) const;
	vector<uint16>	getUint16Vector(const string& aKey,
                                        bool expandable) const;
        vector<uint16>	getUint16Vector(const string& aKey,
                                        const vector<uint16>& aValue,
                                        bool expandable) const;
	vector<int32>	getInt32Vector (const string& aKey,
                                        bool expandable) const;
        vector<int32>	getInt32Vector (const string& aKey,
                                        const vector<int32>& aValue,
                                        bool expandable) const;
	vector<uint32>	getUint32Vector(const string& aKey,
                                        bool expandable) const;
        vector<uint32>	getUint32Vector(const string& aKey,
                                        const vector<uint32>& aValue,
                                        bool expandable) const;
#if HAVE_LONG_LONG
	vector<int64>	getInt64Vector (const string& aKey,
                                        bool expandable) const;
        vector<int64>	getInt64Vector (const string& aKey,
                                        const vector<int64>& aValue,
                                        bool expandable) const;
	vector<uint64>	getUint64Vector(const string& aKey,
                                        bool expandable) const;
        vector<uint64>	getUint64Vector(const string& aKey,
                                        const vector<uint64>& aValue,
                                        bool expandable) const;
#endif
	vector<float>	getFloatVector (const string& aKey,
                                        bool expandable) const;
        vector<float>	getFloatVector (const string& aKey,
                                        const vector<float>& aValue,
                                        bool expandable) const;
        vector<double>	getDoubleVector(const string& aKey,
                                        bool expandable) const;
        vector<double>	getDoubleVector(const string& aKey,
                                        const vector<double>& aValue,
                                        bool expandable) const;
        vector<string>	getStringVector(const string& aKey,
                                        bool expandable) const;
        vector<string>	getStringVector(const string& aKey,
                                        const vector<string>& aValue,
                                        bool expandable) const;
        vector<time_t>	getTimeVector  (const string& aKey,
                                        bool expandable) const;
        vector<time_t>	getTimeVector  (const string& aKey,
                                        const vector<time_t>& aValue,
                                        bool expandable) const;
	// @}

	// @}

	// \name Printing
	// Mostly for debug purposes the collection can be printed.
	// @{

	// Allow printing the whole parameter collection.
	friend std::ostream& operator<<(std::ostream& os, const ParameterSetImpl &thePS);
	// @}

private:
	// Copying is not needed, thus not allowed.
	// @{
	ParameterSetImpl(const ParameterSetImpl& that);
	ParameterSetImpl& 	operator=(const ParameterSetImpl& that);
	// @}

	// \name Implementation of the 'adopt' methods
	// The 'adopt' methods are implemented in the readStream method. The 'read'
	// methods do some preprocessing so the 'adopt' method can use the
	// \c readStream method.
	// @{
	void	readFile   (const string& theFile, const string& prefix, const bool merge);
	void	readBuffer (const string& theFile, const string& prefix, const bool merge);
	void	readStream (istream&	inputStream, const string& prefix, const bool merge);
	// @}

	// Find the key \a aKey. If \a doThrow == \c true (the default) an exception
	// is thrown when \a aKey is not found. Otherwise, end() is returned.
	const_iterator	findKV(const string& aKey, bool doThrow = true) const;

        // Merge in a key/value. A warning is logged if already existing
        // and merge=false.
        void addMerge (const string& key, const string& value, bool merge);


        //# Data members.
	// Reference count.
	int itsCount;
	// Key comparison mode.
	const KeyCompare::Mode itsMode;
};

//# -------------------- Global functions --------------------
// Checks if the given string is a valid versionnumber (x.y.z)
bool	isValidVersionNr   (const string& versionNr);

// Checks if the given string is a valid versionnumber reference. This may be
// of the form \c x.y.z or the words \c stable, \c test or \c development
// (defined as \c PC_QUAL_STABLE, \c PC_QUAL_TEST and \c PC_QUAL_DEVELOP).
bool	isValidVersionNrRef(const string& versionNr);

// Returns the value of the given string or 0 if it is not a valid seqnr
//uint32	seqNr(const string& aString);

// When a hierarchical keyname is passed to \c fullKeyName the methods returns
// the last part of the keyname. For example:
// \code
// moduleName("base.sub.leaf")
// \endcode
// returns \c "leaf". When a keyname without dots is passed the whole key
// is returned.<br>
// \c keyName is a kind of \c dirname function for keys.
string	keyName	   (const string& fullKeyName);

// When a hierarchical keyname is passed to \c moduleName the methods returns
// all but the last part of the keyname. For example:
// \code
// moduleName("base.sub.leaf")
// \endcode
// returns \c "base.sub". When a keyname without dots is passed and empty string
// is returned.<br>
// \c moduleName is a kind of \c basename function for keys.
string	moduleName (const string& fullKeyName);

// Returns the raw keypart of a parameterline that contains a key-value pair.
// The returned string is \e not trimmed for whitespace.
string	keyPart	   (const string& parameterLine);

// Returns the raw value-part of a parameterline that contains a key-value pair.
// This means that the string is \e not trimmed for whitespace and that comments
// at the end of the line are also returned.<br>
// It simply returns everything behind the first \c = sign.
string	valuePart  (const string& parameterLine);

// Returns the value of the index if the string contains an index otherwise
// 0 is returned. The \c indexMarker argument must be used to pass the two chars
// that are used to delimeter the index. The index must be a literal value
// not an expression. For example:
// \code
//  indexValue("label{25}andmore", "{}");
// \endcode
// returns the value 25. When more indexdelimiters are found in the string the
// last pair is used.
int32 	indexValue (const string&	label, char	indexMarker[2]);

// @} addgroup


//#	get(key)
inline const ParameterValue& ParameterSetImpl::get(const string& aKey) const
{
        return findKV(aKey)->second;
}
//#	getBool(key)
inline bool ParameterSetImpl::getBool(const string& aKey) const
{
        return findKV(aKey)->second.getBool();
}

//#	getBool(key, value)
inline bool ParameterSetImpl::getBool(const string& aKey, bool aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getBool();
}

//#	getInt(key)
inline int ParameterSetImpl::getInt(const string& aKey) const
{
        return findKV(aKey)->second.getInt();
}

//#	getInt(key, value)
inline int ParameterSetImpl::getInt(const string& aKey, int aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getInt();
}

//#	getUint(key)
inline uint ParameterSetImpl::getUint(const string& aKey) const
{
        return findKV(aKey)->second.getUint();
}

//#	getUint(key, value)
inline uint ParameterSetImpl::getUint(const string& aKey, uint aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getUint();
}

//#	getInt16(key)
inline int16 ParameterSetImpl::getInt16(const string& aKey) const
{
        return findKV(aKey)->second.getInt16();
}

//#	getInt16(key, value)
inline int16 ParameterSetImpl::getInt16(const string& aKey, int16 aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getInt16();
}

//#	getUint16(key)
inline uint16 ParameterSetImpl::getUint16(const string& aKey) const
{
        return findKV(aKey)->second.getUint16();
}

//#	getUint16(key, value)
inline uint16 ParameterSetImpl::getUint16(const string& aKey, uint16 aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getUint16();
}

//#	getInt32(key)
inline int32 ParameterSetImpl::getInt32(const string& aKey) const
{
        return findKV(aKey)->second.getInt32();
}

//#	getInt32(key, value)
inline int32 ParameterSetImpl::getInt32(const string& aKey, int32 aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getInt32();
}

//#	getUint32(key)
inline uint32 ParameterSetImpl::getUint32(const string& aKey) const
{
        return findKV(aKey)->second.getUint32();
}

//#	getUint32(key, value)
inline uint32 ParameterSetImpl::getUint32(const string& aKey, uint32 aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getUint32();
}

#if HAVE_LONG_LONG
//#	getInt64(key)
inline int64 ParameterSetImpl::getInt64(const string& aKey) const
{
        return findKV(aKey)->second.getInt64();
}

//#	getInt64(key, value)
inline int64 ParameterSetImpl::getInt64(const string& aKey, int64 aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getInt64();
}

//#	getUint64(key)
inline uint64 ParameterSetImpl::getUint64(const string& aKey) const
{
        return findKV(aKey)->second.getUint64();
}

//#	getUint64(key, value)
inline uint64 ParameterSetImpl::getUint64(const string& aKey, uint64 aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getUint64();
}
#endif

//#	getFloat(key)
inline float ParameterSetImpl::getFloat (const string& aKey) const
{
        return findKV(aKey)->second.getFloat();
}

//#	getFloat(key, value)
inline float ParameterSetImpl::getFloat (const string& aKey, float aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getFloat();
}

//#	getDouble(key)
inline double ParameterSetImpl::getDouble(const string& aKey) const
{
        return findKV(aKey)->second.getDouble();
}

//#	getDouble(key, value)
inline double ParameterSetImpl::getDouble(const string& aKey, double aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getDouble();
}

//#	getString(key)
inline string ParameterSetImpl::getString(const string& aKey) const
{
        return findKV(aKey)->second.getString();
}

//#	getString(key, value)
inline string ParameterSetImpl::getString(const string& aKey, const string& aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getString();
}

//#	getTime(key)
inline time_t ParameterSetImpl::getTime(const string& aKey) const
{
        return findKV(aKey)->second.getTime();
}

//#	getTime(key, value)
inline time_t ParameterSetImpl::getTime(const string& aKey, const time_t& aValue) const
{
        const_iterator it = findKV(aKey,false);
	if (it == end()) return aValue;
        return it->second.getTime();
}

} // namespace LOFAR

#endif
