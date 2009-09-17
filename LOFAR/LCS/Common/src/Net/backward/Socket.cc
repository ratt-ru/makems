//# Socket.cc: Class for socket conections
//#
//# Copyright (C) 2002
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
//# $Id: Socket.cc 5156 2005-05-23 07:57:17Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

// Socket
#include <Common/Net/Socket.h>
#include <Common/lofar_algorithm.h>

namespace LOFAR
{

  InitDebugContext(Socket,"Socket");

  //##ModelId=3DB936CF02C9
  int Socket::default_sigpipe_counter;

  //##ModelId=3DB936D300BC
  int Socket::setBlocking (bool block)
  {
    if( fcntl(sid,F_SETFL,block?0:FNDELAY)<0 )
      return set_errcode(SOCKOPT);
    return 0;
  }

  //##ModelId=3C91BA4300F6


  // Class Socket 

  Socket::Socket (const string &sname)
    : name(sname),errcode_(Socket::NOINIT),sid(-1),do_intr(false),bound(false)
  {
    sigpipe_counter = &default_sigpipe_counter;
  }

  //##ModelId=9FD2BC39FEED
  Socket::Socket (const string &sname, const string &serv, int proto, int backlog)
    : name(sname),port_(serv),do_intr(false),bound(false)
  {
    sigpipe_counter = &default_sigpipe_counter;
    initServer(serv,proto,backlog);
  }

  //##ModelId=C15CE2A5FEED
  Socket::Socket (const string &sname, const string &host, const string &serv, int proto, int wait_ms)
    : name(sname),host_(host),port_(serv),do_intr(false),bound(false)
  {
    sigpipe_counter = &default_sigpipe_counter;
    initClient(host,serv,proto,wait_ms);
  }

  //##ModelId=4760B82BFEED
  Socket::Socket (int id, struct sockaddr_in &sa)
    : name("client"),sid(id),type(TCP),host_(">tcp"),port_("0"),do_intr(false),bound(false)
  {
    connected = false;
    sigpipe_counter = &default_sigpipe_counter;
    dprintf(1)("creating connected socket\n");
    // constructs a generic socket (used by accept(), below)
    rmt_addr = sa;
    if( setDefaults()<0 )
    {
      close(sid);
      sid = -1;
      return;
    }
    // Set non-blocking mode
    if( fcntl(sid,F_SETFL,FNDELAY)<0 )
      set_errcode(SOCKOPT);
    // successfully created connected socket
    connected = true;
  }

  //##ModelId=3CC95D6E032A
  Socket::Socket (int id, struct sockaddr_un &sa)
    : name("client"),sid(id),type(UNIX),host_(">unix"),port_("0"),do_intr(false),bound(false)
  {
    connected = false;
    sigpipe_counter = &default_sigpipe_counter;
    dprintf(1)("creating connected socket\n");
    unix_addr = sa;
    if( setDefaults()<0 )
    {
      close(sid);
      sid = -1;
      return;
    }
    // Set non-blocking mode
    if( fcntl(sid,F_SETFL,FNDELAY)<0 )
      set_errcode(SOCKOPT);
    // successfully created connected socket
    connected = true;
  }


  //##ModelId=3DB936D00067
  Socket::~Socket()
  {
    if( sid >=0 )
    {
      dprintf(1)("close(fd)\n");
      close(sid);
    }
    if( bound && type == Socket::UNIX && unix_addr.sun_path[0] )
    {
      int res = unlink(unix_addr.sun_path);
      dprintf(1)("unlink(%s) = %d (%s)\n",unix_addr.sun_path,res<0?errno:res,res<0?strerror(errno):"OK");
    }
    dprintf(1)("destroying socket\n");
  }



  //##ModelId=3C91B9FC0130
  int Socket::initServer (const string &serv, int proto, int backlog)
  {
    // open a server socket
    errcode_ = errno_sys_ = 0;
    sid = -1;
    server = true;
    connected = bound = false;
    type = proto;
  
    if( type == UNIX )
    {
      host_ = "unix";
      port_ = serv;
      // setup socket address
      unix_addr.sun_family = AF_UNIX;
      FailWhen( serv.length() >= sizeof(unix_addr.sun_path),"socket name too long");
      if( serv[0] == '=' ) // abstract socket name
      {
        memset(unix_addr.sun_path,0,sizeof(unix_addr.sun_path));
        serv.substr(1).copy(unix_addr.sun_path+1,sizeof(unix_addr.sun_path)-1);
      }
      else // socket in filesystem
        serv.copy(unix_addr.sun_path,sizeof(unix_addr.sun_path));
      // create socket
      sid = socket(PF_UNIX,SOCK_STREAM,0);
      if( sid < 0 )
        return set_errcode(SOCKET);
      dprintf(1)("creating unix socket %s\n",serv.c_str());
      if( setDefaults()<0 )
      {
        close(sid);
        sid = -1;
        return errcode_;
      }
      // bind the socket
      int res = bind(sid,(struct sockaddr*)&unix_addr,sizeof(unix_addr));
      dprintf(1)("bind()=%d (%s)\n",res<0?errno:res,res<0?strerror(errno):"OK");
      if( res<0 )
      {
        dprintf(1)("close(fd)\n");
        close(sid);
        sid = -1;
        return set_errcode(BIND);
      }
      bound = true;
      // start listening for connections
      res = listen(sid,backlog);
      dprintf(1)("listen()=%d (%s)\n",res<0?errno:res,res<0?strerror(errno):"OK");
      if( res<0 )
      {
        dprintf(1)("close(fd)\n");
        close(sid);
        sid=-1;
        return set_errcode(LISTEN);
      }
    } // endif type UNIX
    else // networked socket (type TCP or UDP)
    {
      host_ = "localhost";
      port_ = serv;
    
      struct servent     *pse;    // service info entry
      struct protoent    *ppe;    // protocol entry

      const char *prot = ( type == UDP ? "udp" : "tcp" );

      memset(&rmt_addr,0,sizeof(rmt_addr));
      rmt_addr.sin_family = AF_INET;
      rmt_addr.sin_addr.s_addr = htonl(INADDR_ANY);

      // try to get service by name or port
      if( (pse = getservbyname(serv.c_str(),prot)) != 0 )
        rmt_addr.sin_port = pse->s_port;
      else if( !(rmt_addr.sin_port = htons((u_short)atoi(serv.c_str()))) )
        return errcode_ = PORT;
      // Map protocol name to protocol number
      if( !(ppe = getprotobyname(prot)) )
        return errcode_ = PROTOCOL;
      // open the socket fd
      int soktype = type == TCP ? SOCK_STREAM : SOCK_DGRAM;
      sid = socket(PF_INET, soktype, ppe->p_proto);
      if( sid < 0 )
        return set_errcode(SOCKET);
      dprintf(1)("created server socket, port %d, protocol %d\n",
                 ntohs((ushort)rmt_addr.sin_port),(int)ppe->p_proto);
      // set default options
      if( setDefaults()<0 )
      {
        close(sid);
        sid = -1;
        return errcode_;
      }
      // bind to the socket
      int res = bind(sid,(struct sockaddr*)&rmt_addr,sizeof(rmt_addr));
      dprintf(1)("bind()=%d (%s)\n",res<0?errno:res,res<0?strerror(errno):"OK");
      if( res<0 )
      {
        dprintf(1)("close(fd)\n");
        close(sid);
        sid = -1;
        return set_errcode(BIND);
      }
      bound = true;
      // start listening on the socket
      if( type == TCP )
      {
        res = listen(sid,backlog);
        dprintf(1)("listen()=%d (%s)\n",res<0?errno:res,res<0?strerror(errno):"OK");
        if( res<0 )
        {
          dprintf(1)("close(fd)\n");
          close(sid);
          sid=-1;
          return set_errcode(LISTEN);
        }
      }
      else
      {
        memset(&rmt_addr,0,sizeof(rmt_addr));
      }
    } // end else TCP/UDP
  
    // set non-blocking mode
    if( fcntl(sid,F_SETFL,FNDELAY)<0 )
      return set_errcode(SOCKOPT);
  
    return 0;
  }

  //##ModelId=3C91BA16008E
  int Socket::initClient (const string &host, const string &serv, int proto, int wait_ms)
  {
    errcode_ = errno_sys_ = 0;
    sid = -1;
    server = false;
    connected = false;
    type = proto;
    host_ =  host;
    port_ = serv;
    if( type == UNIX )
    {
      // setup socket address
      string path = host +":" +serv;
      unix_addr.sun_family = AF_UNIX;
      FailWhen(path.length() >= sizeof(unix_addr.sun_path),"socket name too long");
      if( path[0] == '=' ) // abstract socket name
      {
        memset(unix_addr.sun_path,0,sizeof(unix_addr.sun_path));
        path.substr(1).copy(unix_addr.sun_path+1,sizeof(unix_addr.sun_path)-1);
      }
      else // socket in filesystem
        path.copy(unix_addr.sun_path,sizeof(unix_addr.sun_path));
      // create socket
      sid = socket(PF_UNIX,SOCK_STREAM,0);
      if( sid < 0 )
        return set_errcode(SOCKET);
      dprintf(1)("connecting unix socket to %s\n",path.c_str());
      if( setDefaults()<0 )
      {
        close(sid);
        sid = -1;
        return errcode_;
      }
    } // endif type UNIX
    else // networked socket (type TCP or UDP)
    {
      // opens a client socket
      struct servent     *pse;    // service info entry
      struct hostent     *phe;    // server host entry
      struct protoent    *ppe;    // protocol entry
      string host_addr;           // address of server host

      memset(&rmt_addr,0,sizeof(rmt_addr));
      rmt_addr.sin_family = AF_INET;

      const char *prot = ( proto == UDP ? "udp" : "tcp" );

      // try to get service by name or port
      if( (pse = getservbyname(serv.c_str(),prot)) != 0 )
        rmt_addr.sin_port = pse->s_port;
      else if( !(rmt_addr.sin_port = htons((u_short)atoi(serv.c_str()))) )
        return errcode_ = PORT;
      // try to get host by name
      if( (phe = gethostbyname(host.c_str())) != 0 )
      {
        if( phe->h_addrtype != AF_INET )
          return errcode_ = BADADDRTYPE; 
        host_addr = inet_ntoa(*((struct in_addr*)*(phe->h_addr_list)));
      }
      else
        host_addr = host;
      // try using dot notation
      if( (rmt_addr.sin_addr.s_addr = inet_addr(host_addr.c_str())) == INADDR_NONE )
        return errcode_ = BADHOST;
      // Map protocol name to protocol number
      if( !(ppe = getprotobyname(prot)) )
        return errcode_ = PROTOCOL;
      dprintf(1)("connecting client socket to %s:%s, protocol %d\n",
                 host_addr.c_str(),port_.c_str(),ppe->p_proto);
      // open the socket fd
      int soktype = type == TCP ? SOCK_STREAM : SOCK_DGRAM;
      sid = socket(PF_INET,soktype,ppe->p_proto);
      if( sid < 0 )
        return set_errcode(SOCKET);
      // set default options
      if( setDefaults()<0 )
      {
        close(sid);
        sid = -1;
        return errcode_;
      }
    }
  
    // set non-blocking mode
    if( fcntl(sid,F_SETFL,FNDELAY)<0 )
      return set_errcode(SOCKOPT);
    // try to connect
    if( wait_ms >= 0 )
      return connect(wait_ms);
    return 0;
  }

  //##ModelId=F1A741D4FEED
  string Socket::errstr () const
  {
    static char const *s_errstr[] = {
      "OK",
      "Can't create socket (%d: %s)",
      "Can't bind local address (%d: %s)",
      "Can't connect to server (%d: %s)",
      "Can't accept client socket (%d: %s)",
      "Bad server host name given",
      "Bad address type",
      "Read error (%d: %s)",
      "Write error (%d: %s)",
      "Remote client closed connection (%d: %s)",
      "Couldn't read/write whole message (%d: %s)",
      "Invalid operation",
      "setsockopt() or getsockopt() failure (%d: %s)",
      "wrong port/service specified (%d: %s)",
      "invalid protocol (%d: %s)",
      "listen() error (%d: %s)",
      "timeout (%d: %s)",
      "connect in progress (%d: %s)",
      "No more clients (%d: %s)",
      "General failure",
      "Uninitialized socket" 
    };  
    if( errcode_ < NOINIT || errcode_ > 0 )
      return "";
    return Debug::ssprintf(s_errstr[-errcode_],errno,strerror(errno));
  }

  //##ModelId=6AE5AA36FEED
  int Socket::connect (int wait_ms)
  {
    if( isServer() )
      return errcode_=INVOP;
    for(;;)
    {
      int res;
      if( type == UNIX )
        res = ::connect(sid,(struct sockaddr*)&unix_addr,sizeof(unix_addr));
      else
        res = ::connect(sid,(struct sockaddr*)&rmt_addr,sizeof(rmt_addr));
      if( !res )
        break; // connected? break out
      else 
      {
        dprintf(2)("connect() failed: errno=%d (%s)\n",errno,strerror(errno));
        if( errno == EINPROGRESS || errno == EALREADY )
        {
          {
            errcode_ = INPROGRESS;
            return 0;
          }
        }
        close(sid);
        sid = -1;
        return set_errcode(CONNECT);
      }
    }
    dprintf(1)("connect() successful\n");
    connected = true;
    errcode_ = 0;
    return 1;
  }

  //##ModelId=1357FC75FEED
  Socket* Socket::accept ()
  {
    if( !isServer() )   
    { errcode_=INVOP; return 0; }
    if( sid<0 ) 
    { errcode_=NOINIT; return 0; }
    if( type == UDP ) 
      return this;
    int id; 
    if( type == UNIX )
    {
      size_t len = sizeof(unix_addr);
      id = ::accept(sid,(struct sockaddr*)&unix_addr,&len);
    }
    else
    {
      size_t len = sizeof(rmt_addr);
      id = ::accept(sid,(struct sockaddr*)&rmt_addr,&len);
    }
    if( id < 0 )
    {
      dprintf(1)("accept() failed, errno=%d (%s)\n",errno,strerror(errno));
      set_errcode(ACCEPT);
      return 0;
    }
    else
    {
      dprintf(1)("accept() successful\n");
      errcode_=0;
      return type == UNIX 
        ? new Socket(id,unix_addr) 
        : new Socket(id,rmt_addr);
    }
  }

  //##ModelId=5264A6A9FEED
  int Socket::read (void *buf, int maxn)
  {
    if( sid<0 ) 
      return errcode_=NOINIT; 
    FailWhen(!buf,"null buffer");
    errcode_ = 0;
    if( !maxn ) 
      return 0;
    bool sigpipe = false;
  
    int nread,nleft=maxn;
    if( type != UDP )
    {
      while( nleft>0 && !errcode_ && !sigpipe )
      {
        errno = 0;
        int old_counter = *sigpipe_counter;
        nread = ::read( sid,buf,nleft ); // try to read something
        sigpipe = old_counter != *sigpipe_counter; // check for SIGPIPE
        dprintf(3)("read(%d)=%d%s, errno=%d (%s)\n",nleft,nread,
                   sigpipe?" SIGPIPE":"",errno,strerror(errno));
        if( Debug(10) && nread>0 )
          printData(buf,min(nread,200));
        if( nread<0 ) // error?
        {
          if( errno == EWOULDBLOCK || errno == EAGAIN ) 
          { // if refuses to block, that's OK, return 0
            errcode_ = TIMEOUT;
            return maxn - nleft;
          }
          else // else a real error
            return set_errcode(READERR);
        }
        else if( nread == 0 )
          return errcode_ = PEERCLOSED;
        else
        {
          buf = nread + (char*)buf;
          nleft -= nread;
        }
      }
    }
    else // UDP socket
    {
      errno = 0;
      //    if ((wres=Wait(rtimeout,SWAIT_READ))==0)
      //      errcode_=SK_TIMEOUT;
      //    else 
      socklen_t alen = sizeof(rmt_addr);
      if( (nread=recvfrom(sid,(char*)buf,maxn,0,
                          (struct sockaddr*)&rmt_addr,&alen))<=0 ||
          errno )
        return set_errcode(READERR);
      else
      {
        nleft = 0;
        connected = true;
      }
    }
  
    if( sigpipe )
      return errcode_ = PEERCLOSED;

    return maxn-nleft;
  }

  //##ModelId=139EF112FEED
  int Socket::write (const void *buf, int n)
  {
    if( sid<0 ) 
      return errcode_=NOINIT; 
    FailWhen(!buf,"null buffer");
    errcode_ = 0;
    if( !n ) 
      return 0;
    bool sigpipe = false;

    int nleft=n,nwr;
    if( type != UDP ) // TCP or UNIX: write to stream
    {
      while( nleft>0 && !errcode_ && !sigpipe)
      {
        errno = 0;
        int old_counter = *sigpipe_counter;
        nwr = ::write(sid,buf,nleft);
        sigpipe = old_counter != *sigpipe_counter; // check for SIGPIPE
        dprintf(3)("write(%d)=%d%s, errno=%d (%s)\n",nleft,nwr,
                   sigpipe?" SIGPIPE":"",errno,strerror(errno));
        if( Debug(10) && nwr>0 )
          printData(buf,min(nwr,200));
        if( nwr<0 )
        {
          if( errno == EWOULDBLOCK || errno == EAGAIN ) 
          { // if refuses to block, that's OK, return 0
            errcode_ = TIMEOUT;
            return n - nleft;
          }
          else // else a real error
            return set_errcode(WRITERR);
        }
        else if( nwr==0 )
          return errcode_ = PEERCLOSED;
        else
        {
          buf = nwr + (char*)buf;
          nleft -= nwr;
        }
      }
    }
    else // UDP
    {
      errno = 0;
      if( !connected )
        return errcode_ = WRITERR;
      if( (nwr = sendto(sid,(char*)buf,n,0,(struct sockaddr*)&rmt_addr,
                        sizeof(rmt_addr)))<=0 || errno )
        return set_errcode(WRITERR);
      else
        nleft=0;
    }
  
    if( sigpipe )
      return errcode_ = PEERCLOSED;
  
    return n - nleft;
  }

  //##ModelId=890ACD77FEED
  int Socket::shutdown (bool receive, bool send)
  {
    FailWhen(!receive && !send,"neither receive nor send specified");
    if( sid<0 ) 
      return errcode_ = NOINIT; 
    errcode_ = 0;
    int how = receive ? (send ? SHUT_RDWR : SHUT_RD) : SHUT_WR;
    int res = ::shutdown(sid,how);
    dprintf(1)("shutdown(%d)=%d",how,res);
    if( res<0 )
      return set_errcode(SHUTDOWN);
    return 0;
  }

  //##ModelId=3EE80597FEED
  int Socket::setDefaults ()
  {
    if( sid<0 ) 
      return errcode_ = NOINIT;
  
    uint val=1;
    struct linger lin = { 1,1 };

  
    if( setsockopt(sid,SOL_SOCKET,SO_REUSEADDR,(char*)&val,sizeof(val))<0 )
      return set_errcode(SOCKOPT);
  
    // no more defaults for UNIX sockets
    if( type == UNIX )
      return 0;

    if( setsockopt(sid,SOL_SOCKET,SO_KEEPALIVE,(char*)&val,sizeof(val) )<0)
      return set_errcode(SOCKOPT);
  
    if( setsockopt(sid,SOL_SOCKET,SO_LINGER,(const char*)&lin,sizeof(lin))<0 )
      return set_errcode(SOCKOPT);
  
    if( getsockopt(sid,SOL_SOCKET,SO_SNDBUF,(char*)&sbuflen,&val)<0 )
      return set_errcode(SOCKOPT);
  
    if( getsockopt(sid,SOL_SOCKET,SO_RCVBUF,(char*)&rbuflen,&val)<0 )
      return set_errcode(SOCKOPT);
  
    return 0;
  }

  // Additional Declarations
  //##ModelId=3DB936D50067
  int Socket::readblock (void *buf, int maxn)
  {
    if( sid<0 ) 
      return errcode_=NOINIT; 
    if( !maxn ) 
      return 0;
    FailWhen(!buf,"null buffer");
    errcode_ = 0;
    int nread,nleft=maxn;
    if( type != UDP )
    {
      while( nleft>0 )
      {
        errno = 0;
        nread = ::read( sid,buf,nleft ); // try to read something
        dprintf(3)("read(%d)=%d, errno=%d (%s)\n",nleft,nread,errno,strerror(errno));
        if( do_intr )
          return set_errcode(INCOMPLETE);
        if( Debug(10) && nread>0 )
          printData(buf,min(nread,200));
        if( nread<0 ) // error?
        {
          // return error (except in a few special cases)
          if( errno != EWOULDBLOCK && errno != EAGAIN && errno != EINTR ) 
            return set_errcode(READERR);
        }
        else if( nread == 0 )  // peer closed connection -- return
          return errcode_ = PEERCLOSED;
        else
        {
          buf = nread + (char*)buf;
          nleft -= nread;
        }
      }
    }
    else // UDP socket
    {
      errno = 0;
      //    if ((wres=Wait(rtimeout,SWAIT_READ))==0)
      //      errcode_=SK_TIMEOUT;
      //    else 
      socklen_t alen = sizeof(rmt_addr);
      if( (nread=recvfrom(sid,(char*)buf,maxn,0,
                          (struct sockaddr*)&rmt_addr,&alen))<=0 ||
          errno )
        return set_errcode(READERR);
      else
      {
        nleft = 0;
        connected = true;
      }
    }
  
    return maxn;
  }

  //##ModelId=3DB936D6007C
  int Socket::writeblock (const void *buf, int n)
  {
    if( sid<0 ) 
      return errcode_=NOINIT; 
    if( !n ) 
      return 0;
    FailWhen(!buf,"null buffer");
    errcode_ = 0;
  
    int nleft=n,nwr;
    if( type != UDP ) // TCP or UNIX: write to stream
    {
      while( nleft>0 )
      {
        errno = 0;
        nwr = ::write(sid,buf,nleft);
        dprintf(3)("write(%d)=%d, errno=%d (%s)\n",nleft,nwr,errno,strerror(errno));
        if( do_intr )
          return set_errcode(INCOMPLETE);
        if( Debug(10) && nwr>0 )
          printData(buf,min(nwr,200));
        if( nwr<0 )
        {
          if( errno != EWOULDBLOCK && errno != EAGAIN && errno != EINTR ) 
            return set_errcode(WRITERR);
        }
        else if( nwr==0 )
          return errcode_ = PEERCLOSED;
        else
        {
          buf = nwr + (char*)buf;
          nleft -= nwr;
        }
      }
    }
    else // UDP
    {
      errno = 0;
      if( !connected )
        return errcode_ = WRITERR;
      if( (nwr = sendto(sid,(char*)buf,n,0,(struct sockaddr*)&rmt_addr,
                        sizeof(rmt_addr)))<=0 || errno )
        return set_errcode(WRITERR);
      else
        nleft=0;
    }
  
    return n;
  }


  //##ModelId=3DB936D700EC
  void Socket::printData (const void *buf,int n)
  {
    char hex[64],chars[32];
    chars[20] = 0;
    const unsigned char *ch=(const unsigned char*)buf;
    for( int i=0; i<n; i++,ch++ )
    {
      int pos = i%20;
      if( i && !pos )
      {
        printf("%-60s%-20s\n",hex,chars);
        hex[0] = chars[0] = 0;
      }
      sprintf(hex+pos*3,"%02x ",(int)*ch);
      chars[pos] = (*ch>=32 && *ch<=127) ? *ch : '.';
    }
    if( strlen(hex) )
    {
      if( n%20 )
        chars[n%20] = 0;
      printf("%-60s%-20s\n",hex,chars);
    }
  }

  //##ModelId=3DB936D3037B
  void Socket::interrupt (bool intr)
  {
    do_intr = intr;
  }

  string Socket::sdebug ( int detail,const string &,const char *name ) const
  {
    string out;
    if( detail>=0 ) // basic detail
    {
      out = Debug::ssprintf("%s/%d",name?name:"Socket",sid);
      if( server )
        Debug::append(out,"S");
      if( connected )
        Debug::append(out,"c");
    }
    if( detail >= 1 || detail == -1 )   // normal detail
    {
      Debug::appendf(out,"err:%d",errcode_);
      if( errcode_ )
        Debug::append(out,errstr());
    }
    return out;
  }

} // namespace LOFAR
