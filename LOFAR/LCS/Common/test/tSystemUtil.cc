//#  tSystemUtil.cc: test program for the SystemUtil source
//#
//#  Copyright (C) 2006-2007
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
//#  $Id: tSystemUtil.cc 10552 2007-10-12 07:05:24Z overeem $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/StringUtil.h>
#include <Common/SystemUtil.h>
#include <Common/hexdump.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace LOFAR;
using namespace std;

int main()
{
	cout << "My short hostname is: " << myHostname(false) << endl;
	cout << "My long hostname is : " << myHostname(true) << endl;
	uint32	address = myIPV4Address();
	cout << formatString("My IPV4 address is  : %08lX\n", ntohl(address));
	hexdump ((char*) &address, sizeof(uint32));
	
	return (0);
}
