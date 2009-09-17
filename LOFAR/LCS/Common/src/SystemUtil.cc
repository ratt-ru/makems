//#  SystemUtil.cc: Utility functions
//#
//#  Copyright (C) 2002-2007
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
//#  $Id: SystemUtil.cc 11806 2008-09-28 21:07:32Z loose $
//#
#include <lofar_config.h>
#include <Common/LofarLogger.h>
#include <Common/StringUtil.h>
#include <Common/SystemUtil.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

#if !defined(USE_NOSOCKETS)
// netdb is not available on Cray XT machines with Catamount.
#if defined HAVE_BGL && !defined HAVE_ZOID
// netdb is not available on BGL; all code using netdb will be 
// conditionally included using the HAVE_BGL definition;
#define OPTIMAL_CIOD_CHUNK_SIZE (64 * 1024)
#else
#include <netdb.h>
#endif
#endif

namespace LOFAR {

//
// remoteCopy(localFile, remoteHost, remoteFile)
//
int remoteCopy (const string& localFile, 
			    const string& remoteHost, 
			    const string& remoteFile)
{
	string		tmpResultFile(getTempFileName());
  
	// -B: batch mode; -q: no progress bar
	string command(formatString("scp -Bq %s %s:%s 1>&2 2>%s", localFile.c_str(),
									remoteHost.c_str(), remoteFile.c_str(),
									tmpResultFile.c_str()));
	// execute the command.
	int error = system(command.c_str());
	LOG_DEBUG(formatString("copy command: %s",command.c_str()));

	if(error == 0) {			
		LOG_INFO(formatString("Successfully copied %s to %s:%s",
						localFile.c_str(),remoteHost.c_str(),remoteFile.c_str()));
	}
	else {
		// an error occured, try to reconstruct the message
		char 	outputLine[200];
		string 	outputString;
		FILE* 	f = fopen(tmpResultFile.c_str(),"rt");	// open file with errormsg
		if (f == NULL) {						// oops, problems opening the file
			LOG_ERROR(
				formatString("Unable to remote copy %s to %s:%s: reason unknown",
				localFile.c_str(),remoteHost.c_str(),remoteFile.c_str()));
		}
		else {
			// construct the error message
			while(!feof(f)) {
				fgets(outputLine,200,f);
				if(!feof(f)) {
					outputString+=string(outputLine);
				}
			}
			fclose(f);
			LOG_ERROR(formatString("Unable to remote copy %s to %s:%s: %s",
						localFile.c_str(),remoteHost.c_str(),remoteFile.c_str(),
						outputString.c_str()));
		}
	}

	// remove the temporarely file.
	remove(tmpResultFile.c_str());

	return (error);
}

//
// copyFromRemote(remoteHost, remoteFile, localFile)
//
int copyFromRemote(const string& remoteHost, 
				   const string& remoteFile,
				   const string& localFile)
{
	string		tmpResultFile(getTempFileName());
  
	// -B: batch mode; -q: no progress bar
	string command(formatString("scp -Bq %s:%s %s 1>&2 2>%s", 
									remoteHost.c_str(), remoteFile.c_str(), localFile.c_str(),
									tmpResultFile.c_str()));
	// execute the command.
	int error = system(command.c_str());
	LOG_DEBUG(formatString("copy command: %s",command.c_str()));

	if(error == 0) {			
		LOG_INFO(formatString("Successfully copied %s:%s to %s",
							  remoteHost.c_str(),remoteFile.c_str(),localFile.c_str()));
	}
	else {
		// an error occured, try to reconstruct the message
		char 	outputLine[200];
		string 	outputString;
		FILE* 	f = fopen(tmpResultFile.c_str(),"rt");	// open file with errormsg
		if (f == NULL) {						// oops, problems opening the file
			LOG_ERROR(
				formatString("Unable to remote copy %s:%s to %s: reason unknown",
							 remoteHost.c_str(),remoteFile.c_str(),localFile.c_str()));
		}
		else {
			// construct the error message
			while(!feof(f)) {
				fgets(outputLine,200,f);
				if(!feof(f)) {
					outputString+=string(outputLine);
				}
			}
			fclose(f);
			LOG_ERROR(formatString("Unable to remote copy %s:%s to %s: %s",
								   remoteHost.c_str(),remoteFile.c_str(),localFile.c_str(),
								   outputString.c_str()));
		}
	}

	// remove the temporarely file.
	remove(tmpResultFile.c_str());

	return (error);
}

//
// getTempFileName([format])
//
string getTempFileName(const string&	format)
{
	char tempFileName [256];

	if (format.find("XXXXXX", 0) != string::npos) {	// did user specify mask?
		strcpy (tempFileName, format.c_str());		// use user-mask
	}
	else {
		strcpy (tempFileName, "/tmp/LOFAR_XXXXXX");// use default mask
	}

	mkstemp(tempFileName);							// let OS invent the name

	return string(tempFileName);
}

//
// myHostname(giveFullname)
//
string myHostname(bool	giveFullName)
{
	char	fullhostname[300];
	if (gethostname(fullhostname, 300) != 0) {
		return ("localhost");
	}

	if (!giveFullName) {
		char*	dot = strchr(fullhostname, '.');
		if (dot) {
			*dot='\0';
		}
	}

	return (fullhostname);
}

//
// myIPaddress
//
uint32 myIPV4Address()
{
#if defined HAVE_BGL && !defined HAVE_ZOID
	LOG_ERROR ("Function myIPV4Address not available for Blue Gene.");
	return (0);
#elif defined USE_NOSOCKETS
	LOG_ERROR ("Function myIPV4Address not available.");
	return (0);
#else
	struct hostent*		hostEnt;
	if (!(hostEnt = gethostbyname(myHostname(false).c_str()))) {
		return (0);
	}

	if (hostEnt->h_length != 4) {
		LOG_ERROR_STR ("Length of my IPaddress is " << hostEnt->h_length << " i.s.o. 4, ignoring result.");
		return (0);
	}

	// copy info to save place.
	uint32	address;
	bcopy (hostEnt->h_addr_list[0], &address, hostEnt->h_length);

	return (address);
#endif
}

} // namespace LOFAR
