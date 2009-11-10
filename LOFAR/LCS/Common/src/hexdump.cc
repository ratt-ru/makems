//# Hexdump.cc: Produces hexdump of data block
//#
//# Copyright (C) 2002-2004
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
//# $Id: hexdump.cc 4847 2005-04-13 14:33:43Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/hexdump.h>
#include <Common/StringUtil.h>
#include <stdio.h>

using namespace LOFAR;
using namespace std;

namespace LOFAR
{

// Define hexdump modes
enum { HdmString, HdmFile, HdmCppString };

//
// internalHexdump(buf, nrBytes, mode)
//
void internalHexdump (void*			device, 
					  const void*	buf, 
					  int32 		nrBytes, 
					  int32			mode)
{
#define	BTS_P_LINE		16
#define HALF_LINE		(BTS_P_LINE/2)

	char  hex[(3*BTS_P_LINE)+4], chars[BTS_P_LINE+1];

	const unsigned char *ch = (const unsigned char*)buf;

	for (int32 left = 0; left < nrBytes; left += BTS_P_LINE) {
		for (int32 i = 0; i < BTS_P_LINE; i++, ch++) {
			if (left + i < nrBytes) {
				// add bytes code to hex and char string
				sprintf(hex+(i*3)+(i/HALF_LINE),"%02x  ",(int)*ch);
				chars[i] = (*ch>=32 && *ch<=127) ? *ch : '.';
			}
			else {
				sprintf(hex+(i*3)+(i/HALF_LINE), "    ");
				chars[i] = ' ';
			}
		}
		hex   [3*BTS_P_LINE+2] = '\0';
		chars [BTS_P_LINE] 	   = '\0';
		// show result
		switch (mode) {
		case HdmFile:
			fprintf (static_cast<FILE*>(device), "%04X: %s%s\n", left, hex, chars);
			break;
		case HdmString:
			sprintf (static_cast<char*>(device), "%04X: %s%s\n", left, hex, chars);
			device = static_cast<char*>(device) + (6+(3*BTS_P_LINE)+2+BTS_P_LINE+1);
			break;
		case HdmCppString:
			static_cast<string*>(device)->append(formatString("%04X: %s%s\n", left, hex, chars));
			break;
		}

		chars[0] = '\0';
	}
}

//
// hexdump(buf, nrBytes)		 to standard out
//
void hexdump (const void *buf, int32 	nrBytes)
{
	internalHexdump(stdout, buf, nrBytes, HdmFile);
}

//
// hexdump(file, buf, nrBytes)
//
void hexdump (FILE*	aFile, const void *buf, int32 	nrBytes)
{
	internalHexdump(aFile, buf, nrBytes, HdmFile);
}

//
// hexdump(string, buf, nrBytes)
//
void hexdump (char*	aString, const void *buf, int32 	nrBytes)
{
	internalHexdump(aString, buf, nrBytes, HdmString);
}

//
// hexdump(string&, buf, nrBytes)
//
void hexdump (string&	aString, const void *buf, int32 	nrBytes)
{
	internalHexdump(&aString, buf, nrBytes, HdmCppString);
}


} // namespace LOFAR
