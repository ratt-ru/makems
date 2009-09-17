//# Socket.h: Class for connections over TCP/IP or UDP
//#
//# Copyright(C) 2002-2004
//# ASTRON(Netherlands Foundation for Research in Astronomy)
//# P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//# This program is free software; you can redistribute it and/or modify
//# it under the terms of the GNU General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or
//#(at your option) any later version.
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
//# $Id: Socket.h 10722 2007-12-05 13:45:35Z diepen $

#ifndef LOFAR_COMMON_SOCKET_H
#define LOFAR_COMMON_SOCKET_H

// \file
// Class for connections over TCP/IP or UDP

#include <Common/lofar_string.h>
#include <Common/LofarTypes.h>

#ifndef USE_NOSOCKETS

#if defined(__sun)
#include <arpa/nameser.h>
#include <netinet/in.h>
// Note: should be uint32(=size_t), but in all TCP calls it is impl. as int32
typedef	int	socklen_t;		
#define    INADDR_NONE             ((in_addr_t) 0xffffffff)
#endif

#if defined(__APPLE__)
#include <netinet/in.h> 
#endif

#include <resolv.h>
#include <sys/un.h>
#endif

#include <errno.h>


namespace LOFAR
{
// \ingroup Common
// \addtogroup Socket
// @{

// The Socket class gives you a service access point for communication on
// TCP/IP or UDP sockets.
class Socket {
public:
	//# ---------- Construction and destruction ----------
	// Create a trivial Socket object, not yet connected to anything.
	explicit Socket(const string&	socketName = "");

	// Create server socket and start listener on given port.
	Socket(const string&	socketname, 
		   const string&	service,
		   int32			proto = Socket::TCP, 
		   int32			backlog = 5);

	// Create client socket and try to connect to given host and port.
	Socket(const string&	socketname, 
		   const string&	hostname,
		   const string&	service,
		   int32			proto = Socket::TCP);

	~Socket();

	// \name Bind and connect
	// Routines for binding and connecting the socket object to the TCP/UDP
	// stack.
	// <group>
	int32	initServer(const string&	service,
					   int32			proto = Socket::TCP, 
					   int32			backlog = 5);
	int32	initClient(const string&	hostname,
					   const string&	service,
					   int32			proto = Socket::TCP);
	// </group>

	// Attempts to (re)connect to a server socket.
	// waitMs: <0		blocking
	//		   >=0		wait waitMs milliseconds for completion
	//
	// Returvalues:
	//	SK_OK		Successful connected
	//	INPROGRESS	Not connected within the given timelimit, still in progress
	//	CONNECT		Some error occured, errnoSys() gives more info
	//
	int32 connect(int32 waitMs = -1);

	// Tries to (re)accept an incoming connection on a server socket.
	// waitMs: <0		blocking
	//		   >=0		wait waitMs milliseconds for completion
	//
	// Returvalues:
	//			!=0		Pointer to new created socket.
	//			0		Some problem occured, use errcode() to get info:
	//	SK_OK		Successful connected
	//	INV_OP		(Listener)socket must be a server socket
	//	NOINIT		(Listener)socket not initialized yet.
	//	INPROGRESS	Not connected within the given timelimit, still in progress
	//	ACCEPT		Some error occured, errnoSys() gives more info
	//
	Socket* accept(int32 waitMs = -1);

  // Close the socket. 
  int32 close();

	// Shuts down the socket for receive and/or send
	int32 shutdown(bool receive = true, bool send = true);

	// Sets the socket in blocking or non-blocking mode. All consequent reads
	// and writes in the socket are performed in this mode.
	// Default sockets are blocking.
	int32 setBlocking(bool blocking = true);
      
	// Interrupts readblocking/writeblocking calls(for multithreaded sockets)
	inline void interrupt(bool allowInterrupt = true);
      
	// Reads up to maxbytes from socket. When the socket is in blocking mode
	// the read waits until nrBytes are received. In non-blocking mode only
	// the available bytes are read.
	// Returns the number of bytes read or a (negative) errornumber
	// Returnvalue: < 0					error occured
	// 				>=0 != nrBytes		intr. occured or socket is nonblocking
	//				>=0 == nrBytes		everthing went OK.
	//
	// Errors:	SK_OK		no error
	//			NOINIT		socket not initialized yet.
	//			INCOMPLETE	not all bytes are received yet.
	//			READERR		Some generic error, errorSys() given more info
	//			PEERCLOSED	Other sid has closed the connection.
	int32 read(void*	buf,  int32	nrBytes);

	// Writes nrBytes bytes to the socket. 
	// Returns the number of bytes written or a (negative) errornumber
	// Returnvalue: < 0					error occured
	// 				>=0 != maxBytes		intr. occured or socket is nonblocking
	//				>=0 == maxBytes		everthing went OK.
	//
	// Errors:	SK_OK		no error
	//			NOINIT		socket not initialized yet.
	//			INCOMPLETE	not all bytes are written yet.
	//			WRITEERR	Some generic error, errorSys() given more info
	//			PEERCLOSED	Other sid has closed the connection.
	int32 write(const void*	buf, int32	nrBytes);

	// Does a blocking read \e independent of the current mode.
	// Returnvalues and errors are identical to read().
	int32 readBlocking(void*	buf, int32	nrBytes);

	// Does a blocking write \e independent of the current mode.
	// Returnvalues and errors are identical to write().
	//# Is this uberhaupt possible in TCP/IP ????
	int32 writeBlocking(const void*	buf, int32	nrBytes);

	//# --------------- Miscellaneous functions ---------------
	// Points socket at SIGPIPE counter.
	// When a read or write is done on the Socket the value of the sigpipe-
	// Counter is checked for value-changes. When the value was changed
	// during the read or write, the function returns with SIGPIPE and
	// will not process the data.
	// Such it is possible to implement a sigpipe-handler as long as this 
	// handler increments the sigpipeCounter everytime it handles some data.
	inline void setSigpipeCounter(const volatile int32*	counter);
      
	//# \name Datamember access functions
	//# <group>  

	// Get name of socket
	inline const string& getName() const;
	// Set name of socket
	inline void setName(const string& value);

	// Get human readable reason for failure.
	string errstr() const;

        // Socket errorcode
        inline int32                    errcode()               const;
        // System errorcode
        inline int32                    errnoSys()              const;
        // Socket ID (filedesc.)
        inline int32                    getSid()                const;
        // Protocoltype
        inline int16                    getType()               const;
        // Socket role
        inline bool                             isServer()              const;
        // Connected to other side
        inline bool                             isConnected()   const;
        // in blocking mode or not
        inline bool                             isBlocking()    const;
        // Host connected to
        inline const string&    host()                  const;
        // Port connected to
        inline const string&    port()                  const;
        // Connected and no errors
        inline bool                             ok()                    const;
        //# </group>

	//# Additional Public Declarations
	// Socket types. 
        typedef enum { 
                UDP,                    ///< UDP datagram socket
                TCP,                    ///< TCP(stream) socket over network
                UNIX,                   ///< unix socket(local)
                LOCAL=UNIX 
        } SocketTypes;

	// Error codes
        typedef enum {
                SK_OK         =  0,     ///< Ok
                SOCKET        = -1,     ///< Can't create socket
                BIND          = -2,     ///< Can't bind local address
                CONNECT       = -3,     ///< Can't connect to server
                ACCEPT        = -4,     ///< Can't accept client socket
                BADHOST       = -5,     ///< Bad server host name given
                BADADDRTYPE   = -6,     ///< Bad address type
                READERR       = -7,     ///< Read error
                WRITERR       = -8,     ///< Write error
                PEERCLOSED    = -9,     ///< Remote client closed connection
                INCOMPLETE    = -10,    ///< Couldn't read/write whole message
                INVOP         = -11,    ///< Invalid operation
                SOCKOPT       = -12,    ///< sockopt() failure
                PORT          = -13,    ///< wrong port/service specified
                PROTOCOL      = -14,    ///< invalid protocol
                LISTEN        = -15,    ///< listen() error
                TIMEOUT       = -16,    ///< timeout
                INPROGRESS    = -17,    ///< connect() in progress
                NOMORECLI     = -18,    ///< No more clients
                SHUTDOWN      = -19,    ///< shutdown() failure
                CLOSE         = -20,	///< close() failure
                NOINIT        = -21     ///< uninitialized socket
        } ErrorCodes;

protected:
#ifndef USE_NOSOCKETS
	// Constructs a generic socket for an incoming connection on a server
	// socket.
	// @{
	Socket(int32 id, struct sockaddr_in &sa);
	Socket(int32 id, struct sockaddr_un &sa);
	// @}

	// Sets default socket options like reuse address, linger, etc.
	int32 setDefaults();

	// Tries to init the socket by resolving all parameters and allocating
	// the real socket.
	// @{
	int32 initUnixSocket(bool	asServer);
	int32 initTCPSocket (bool	asServer);
	int32 openTCPSocket (bool	asServer);
	// @}
#endif
	// Saves given errorcode and system errorcode
	inline int32 setErrno(int32 ErrorNr);

private:
	// Copying is not allowed
	// @{
	Socket(const Socket&	that);
	Socket&	operator=(const Socket&	that);
	// @}

	//# ---------- Data Members ----------

        // Name of socket given by user
        string                          itsSocketname;
        // Own error number
        int32                           itsErrno;
        // System error number
        int32                           itsSysErrno;
        // File descriptor of the socket
        int32                           itsSocketID;
        // Socket type
        int16                           itsType;
        // Server or Client Socket
        bool                            itsIsServer;
        // Connected or not
        bool                            itsIsConnected;
        // Name of host at other side
        string                          itsHost;
        // Portnr of server
        string                          itsPort;
        // Interrupt read/write block call
        bool                            itsAllowIntr;
        // Socket is initialized
        bool                            itsIsInitialized;
        // Blocking mode or not
        bool                            itsIsBlocking;

#ifndef USE_NOSOCKETS
        // Connected client address(TCP)
        struct sockaddr_in              itsTCPAddr;
        // Connected client address(UNIX)
        struct sockaddr_un              itsUnixAddr;
#endif
        int32                           itsProtocolType;

	//# Support for sigpipes
	const volatile int32*	sigpipeCounter;
	static int32			defaultSigpipeCounter;

};

// @}

//# --------------- Inline implementations ---------------

inline bool Socket::ok() const
{
	return ((itsSocketID >= 0) && !itsErrno);
}

inline const string& Socket::getName() const
{
	return (itsSocketname);
}

inline void Socket::setName(const string& value)
{
	itsSocketname = value;
}

inline int32 Socket::errcode() const
{
	return (itsErrno);
}

inline int32 Socket::errnoSys() const
{
	return (itsSysErrno);
}

inline int32 Socket::getSid() const
{
	return (itsSocketID);
}

inline int16 Socket::getType() const
{
	return (itsType);
}

inline bool Socket::isServer() const
{
	return (itsIsServer);
}

inline bool Socket::isConnected() const
{
	return (itsIsConnected);
}

inline bool Socket::isBlocking() const
{
	return (itsIsBlocking);
}

inline const string& Socket::host() const
{
	return (itsHost);
}

inline const string& Socket::port() const
{
	return (itsPort);
}

inline int32 Socket::setErrno(int32 errorNr)
{
	itsSysErrno = errno;				// save system errno
	return (itsErrno = errorNr);		// save and return given error
}

inline void Socket::interrupt (bool intr)
{
	itsAllowIntr = intr;
}

inline void Socket::setSigpipeCounter(const volatile int32 *counter)
{
	sigpipeCounter = counter;
}

} // namespace LOFAR

#endif

