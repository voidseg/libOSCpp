/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002-2004 libOSC++ contributors. See AUTHORS                */
/*                                                                           */
/* This library is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU Lesser General Public                */
/* License as published by the Free Software Foundation; either              */
/* version 2.1 of the License, or (at your option) any later version.        */
/*                                                                           */
/* This library is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         */
/* Lesser General Public License for more details.                           */
/*                                                                           */
/* You should have received a copy of the GNU Lesser General Public          */
/* License along with this library; if not, write to the Free Software       */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                           */
/* For questions regarding this program contact                              */
/* Daniel Holth <dholth@fastmail.fm> or visit                                */
/* http://wiretap.stetson.edu/                                               */

/* Daniel Holth <dholth@stetson.edu> or visit                                */
/* http://www.stetson.edu/~ProctoLogic/                                      */

/*
 * This class was written by Martin Rumori (martin@rumori.de)
 */

#include "InetOSC.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "socklen_t.h"

#ifdef    __CYGWIN__

#else  // __CYGWIN__

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#else
#error "FIXME: portability code for no arpa/inet.h"
#endif

#endif // __CYGWIN__

using namespace std;

InetOSC::InetOSC ()
{
  _error = OSC_IN_SUCCESS;
  _lasterrno = 0;

  setMaxHandleCycles();

} // InetOSC


InetOSC::~InetOSC ()
{

} // ~InetOSC


const InetOSC::InetOSCError
InetOSC::getError () const
{
  return (const InetOSCError)_error;
} // getError


const int
InetOSC::getLastErrno () const
{
  return (const int)_lasterrno;
} // getLastErrno


const bool
InetOSC::getErrorString (const InetOSC::InetOSCError err, std::string& errString)
{
  switch (err)
    {
    case InetOSC::OSC_IN_SUCCESS :
      errString = "OSC_IN_SUCCESS: no error";
      break;

    case InetOSC::OSC_IN_UNKNOWN :
      errString = "OSC_IN_UNKNOWN: unspecified error";
      break;

    case InetOSC::OSC_IN_PROTO_UNKNOWN :
      errString = "OSC_IN_PROTO_UNKNOWN: invalid protocol type";
      break;

    case InetOSC::OSC_IN_ADDR_UNKNOWN :
      errString = "OSC_IN_ADDR_UNKNOWN: invalid address type";
      break;

    case InetOSC::OSC_IN_GETHOST_ERRNO :
      errString = "OSC_IN_GETHOST_ERRNO: error while gethostbyname()";
      break;

    case InetOSC::OSC_IN_GETPROTO_ERRNO :
      errString = "OSC_IN_GETPROTO_ERRNO: error while getprotobyname()";
      break;

    case InetOSC::OSC_IN_SOCKET_ERRNO :
      errString = "OSC_IN_SOCKET_ERRNO: error while socket()";
      break;

    case InetOSC::OSC_IN_SETSOCKOPT_ERRNO :
      errString = "OSC_IN_SETSOCKOPT_ERRNO: error while setsockopt()";
      break;

    case InetOSC::OSC_IN_FCNTL_ERRNO :
      errString = "OSC_IN_FCNTL_ERRNO: error while fcntl()";
      break;

    case InetOSC::OSC_IN_BIND_ERRNO :
      errString = "OSC_IN_BIND_ERRNO: error while bind()";
      break;

    case InetOSC::OSC_IN_LISTEN_ERRNO :
      errString = "OSC_IN_LISTEN_ERRNO: error while listen()";
      break;

    case InetOSC::OSC_IN_CONNECT_ERRNO :
      errString = "OSC_IN_CONNECT_ERRNO: error while connect()";
      break;

    case InetOSC::OSC_IN_SHUTDOWN_ERRNO :
      errString = "OSC_IN_SHUTDOWN_ERRNO: error while shutdown()";
      break;

    case InetOSC::OSC_IN_CLOSE_ERRNO :
      errString = "OSC_IN_CLOSE_ERRNO: error while close()";
      break;

    case InetOSC::OSC_IN_RECV_ERRNO :
      errString = "OSC_IN_RECV_ERRNO: error while recv() resp. recvfrom()";
      break;

    case InetOSC::OSC_IN_SEND_ERRNO :
      errString = "OSC_IN_SEND_ERRNO: error while send() resp. sendto()";
      break;

    case InetOSC::OSC_IN_POLL_ERRNO :
      errString = "OSC_IN_POLL_ERRNO: error while poll()";
      break;

    case InetOSC::OSC_IN_ACCEPT_ERRNO :
      errString = "OSC_IN_ACCEPT_ERRNO: error while accept()";
      break;

    case InetOSC::OSC_IN_MALLOC_ERRNO :
      errString = "OSC_IN_MALLOC_ERRNO: error while malloc()";
      break;

    case InetOSC::OSC_IN_NO_PROCESSOR :
      errString = "OSC_IN_NO_PROCESSOR: no processor set";
      break;

    case InetOSC::OSC_IN_NO_TRANSMGR :
      errString = "OSC_IN_NO_TRANSMGR: no transport manager set";
      break;

    case InetOSC::OSC_IN_NO_TCPMASTER :
      errString = "OSC_IN_NO_TCPMASTER: no tcp master set";
      break;

    case InetOSC::OSC_IN_NOT_IDLE :
      errString = "OSC_IN_NOT_IDLE: object is not idle";
      break;

    case InetOSC::OSC_IN_ALREADY_SLAVE :
      errString = "OSC_IN_ALREADY_SLAVE: slave already inserted";
      break;

    case InetOSC::OSC_IN_NOT_SLAVE :
      errString = "OSC_IN_NOT_SLAVE: requested slave not found";
      break;

    case InetOSC::OSC_IN_WOULDBLOCK :
      errString = "OSC_IN_WOULDBLOCK: operation would block";
      break;

    case InetOSC::OSC_IN_MAXCYCLES :
      errString = "OSC_IN_MAXCYCLES: maximum number of cycles reached";
      break;

    case InetOSC::OSC_IN_CLOSED_CONN :
      errString = "OSC_IN_CLOSEDCONN : object is not connected";
      break;

    default : // unknown error code
      errString = "<no description available>";
      return false;
    }

  return true;
} // getErrorString


const InetOSC::InetOSCAddrType
InetOSC::getAddrType () const
{
  return (const InetOSCAddrType)_addrType;
} // getAddrType


void
InetOSC::setMaxHandleCycles (const int cycles)
{
  _maxHandleCycles = cycles;

} // setMaxHandleCycles


const int
InetOSC::getMaxHandleCycles () const
{
  return _maxHandleCycles;

} // getMaxHandleCycles


bool
InetOSC::createSocket (int *const sd,
		       const InetOSCProto proto,
		       const InetOSCAddrType addrType)
{
  int socktype;
  struct protoent *protocol;

  // determine protocol
  switch (proto)
    {
    case OSC_IN_UDP :
      protocol = getprotobyname("udp");
      socktype = SOCK_DGRAM;
      break;

    case OSC_IN_TCP :
      protocol = getprotobyname("tcp");
      socktype = SOCK_STREAM;
      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::createSocket(): protocol type unknown" << endl;
#endif
      _error = OSC_IN_PROTO_UNKNOWN;

      return false;
    }

  if (! protocol)
    {
      _lasterrno = errno;
      _error = OSC_IN_GETPROTO_ERRNO;

      return false;
    }
  int protonum = protocol->p_proto;

  // create socket
  int domain = -1;

  switch (addrType)
    {
    case OSC_IN_IPv4 :
      domain = PF_INET;
      break;

    case OSC_IN_IPv6 :
      domain = PF_INET6;
      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::createSocket(): address type unknown" << endl;
#endif
      _error = OSC_IN_ADDR_UNKNOWN;

      return false;
    }

  if ((*sd = socket(domain, socktype, protonum)) == -1)
    {
      _lasterrno = errno;
      _error = OSC_IN_SOCKET_ERRNO;

      return false;
    }

  return true;
} // createSocket


bool
InetOSC::bindSocketToAny (int *const sd,
			  const short port,
			  const InetOSCAddrType addrType)
{
  // bind socket
  switch (addrType)
    {
    case OSC_IN_IPv4 :
      {
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(addr.sin_zero), '\0', sizeof(addr.sin_zero));

	if (bind(*sd, (struct sockaddr *)&addr, sizeof(addr)))
	  {
	    _lasterrno = errno;
	    _error = OSC_IN_BIND_ERRNO;

#ifdef LIBOSC_DEBUG
	    cerr << "InetOSC::bindSocketToAny():bind(): " << strerror(errno) << endl << endl;
#endif
	    return false;
	  }
      }
      break;

    case OSC_IN_IPv6 :
      {
	struct sockaddr_in6 addr;
	addr.sin6_family = AF_INET6;
	addr.sin6_port = port;
	addr.sin6_addr = in6addr_any;

	if (bind(*sd, (struct sockaddr *)&addr, sizeof(addr)))
	  {
	    _lasterrno = errno;
	    _error = OSC_IN_BIND_ERRNO;

#ifdef LIBOSC_DEBUG
	    cerr << "InetOSC::bindSocketToAny():bind(): " << strerror(errno) << endl << endl;
#endif
	    return false;
	  }
      }
      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::bindSocketToAny(): address type unknown" << endl;
#endif

      _error = OSC_IN_ADDR_UNKNOWN;

      return false;
    }

  return true;
} // bindSocketToAny


bool
InetOSC::closeSocket (int *const sd, const bool setError)
{
  while (1)
    {
      if (close(*sd))
	{
	  if (errno == EINTR)
	    {
#ifdef LIBOSC_DEBUG
	      cerr << "InetOSC::closeSocket():close(): got EINTR. retrying" << endl;
#endif
	      continue;
	    }
	  else
	    {
	      if (setError)
		{
		  _error = OSC_IN_CLOSE_ERRNO;
		  _lasterrno = errno;
		}
#ifdef LIBOSC_DEBUG
	      cerr << "InetOSC::closeSocket():close(): " << strerror(errno) << endl;
#endif
	      return false;
	    }
	}
      else
	{
	  break;
	}
    }

  return true;
} // closeSocket


bool
InetOSC::allocSockAddr (const InetOSCAddrType addrType,
			struct sockaddr * *const addr,
			socklen_t *const addrlen)
{
  switch (addrType)
    {
    case OSC_IN_IPv4 :
      *addrlen = sizeof(struct sockaddr_in);
      break;

    case OSC_IN_IPv6 :
      *addrlen = sizeof(struct sockaddr_in6);
      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::allocSockAddr(): address type unknown" << endl;
#endif
      _error = OSC_IN_ADDR_UNKNOWN;

      return false;
    } // switch (_addrtype)

  if (! (*addr = (struct sockaddr *)calloc(1, *addrlen)))
    {
      _lasterrno = errno;
      _error = OSC_IN_MALLOC_ERRNO;

#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::allocSockAddr():malloc(): returned error" << endl;
#endif

      return false;
    }

  return true;
} // allocSockAddr


bool
InetOSC::setRemoteAddr (const std::string& host,
			const short port,
			const InetOSCAddrType addrType,
			struct sockaddr *const addr)
{
  // resolve hostname
  struct hostent *hostaddr;

  /* what we finally want is a configure check for GNU gethostbyname2()
   */
  if (! (hostaddr = gethostbyname(host.c_str())))
    {
      _lasterrno = h_errno;
      _error = OSC_IN_GETHOST_ERRNO;

#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::setRemoteAddr():gethostbyname(): " << strerror(errno) << endl;
#endif

      return false;
    }

  switch (addrType)
    {
    case OSC_IN_IPv4 :
      {
	struct sockaddr_in *realaddr = (sockaddr_in *)addr;

	realaddr->sin_family = AF_INET;
	realaddr->sin_port = port;
	realaddr->sin_addr = *(struct in_addr*)(hostaddr->h_addr_list[0]);
      }
      break;

    case OSC_IN_IPv6 :
      {
	struct sockaddr_in6 *realaddr = (sockaddr_in6 *)addr;

	realaddr->sin6_family = AF_INET6;
	realaddr->sin6_port = port;
	realaddr->sin6_addr = *(struct in6_addr*)(hostaddr->h_addr_list[0]);
      }
      break;

    default :
      // this should have been caught earlier by allocSockAddr()
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::setRemoteAddr(): address type unknown" << endl;
#endif
      _error = OSC_IN_ADDR_UNKNOWN;

      return false;
    } // switch (addrType)

  return true;
} // setRemoteAddr


bool
InetOSC::setRemotePort(const short port,
			const InetOSCAddrType addrType,
			struct sockaddr *const addr)
{
  switch (addrType)
    {
    case OSC_IN_IPv4 :
      {
	struct sockaddr_in *realaddr = (sockaddr_in *)addr;

	realaddr->sin_family = AF_INET;
	realaddr->sin_port = port;
      }
      break;

    case OSC_IN_IPv6 :
      {
	struct sockaddr_in6 *realaddr = (sockaddr_in6 *)addr;

	realaddr->sin6_family = AF_INET6;
	realaddr->sin6_port = port;
      }
      break;

    default :
      // this should have been caught earlier by allocSockAddr()
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::setRemoteAddr(): address type unknown" << endl;
#endif
      _error = OSC_IN_ADDR_UNKNOWN;

      return false;
    } // switch (addrType)

  return true;
} // setRemotePort




std::string
InetOSC::getHostAddress (const InetOSCAddrType addrType,
			 const struct sockaddr *const addr)
{
  std::string rhost;

  switch (addrType)
    {
    case OSC_IN_IPv4 :
      {
	struct sockaddr_in *realaddr = (struct sockaddr_in *)addr;
	char remoteHost[INET_ADDRSTRLEN] = "";

#ifdef __CYGWIN__
#warning "no inet_ntop in cygwin; probable crash imminent"
#else
	inet_ntop(AF_INET,
		  &realaddr->sin_addr,
		  remoteHost,
		  INET_ADDRSTRLEN);
#endif
	rhost.assign(remoteHost);
      }
      break;

    case OSC_IN_IPv6 :
      {
	struct sockaddr_in6 *realaddr = (struct sockaddr_in6 *)addr;
	char remoteHost[INET6_ADDRSTRLEN] = "";

#ifdef __CYGWIN__
#warning "no inet_ntop in cygwin; probable crash imminent"
        cerr << "Cygwin seems to lack IPv6 support. Exiting now for your own good." << endl;
        exit(255);
#else
	inet_ntop(AF_INET6,
		  &realaddr->sin6_addr,
		  remoteHost,
		  INET6_ADDRSTRLEN);
#endif
	rhost.assign(remoteHost);
      }
      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::getHostAddress(): unknown address type" << endl;
#endif
      rhost.assign("");
      _error = OSC_IN_ADDR_UNKNOWN;
    }

  return rhost;
} // getHostAddress


std::string
InetOSC::getHostName (const InetOSCAddrType addrType,
		      const struct sockaddr *const addr)
{
  if (addrType != OSC_IN_IPv4)
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::getPortNumber(): unknown address type" << endl;
#endif
      _error = OSC_IN_ADDR_UNKNOWN;

      return std::string("");
    }

struct hostent *hostaddr;

 if (! (hostaddr = gethostbyaddr((char *)&((struct sockaddr_in *)addr)->sin_addr,
				  sizeof(struct in_addr),
				  AF_INET)))
   {
     _lasterrno = h_errno;
     _error = OSC_IN_GETHOST_ERRNO;

#ifdef LIBOSC_DEBUG
     cerr << "InetOSC::getHostName():gethostbyaddr(): " << strerror(errno) << endl;
#endif

     return std::string("");
   }

 return std::string(hostaddr->h_name);
} // getHostName


short
InetOSC::getPortNumber (const InetOSCAddrType addrType,
			const struct sockaddr *const addr)
{
  short port = 0;
  _error = OSC_IN_SUCCESS;

  switch (addrType)
    {
    case OSC_IN_IPv4 :
      port = ((struct sockaddr_in *)addr)->sin_port;

      break;

    case OSC_IN_IPv6 :
      port = ((struct sockaddr_in6 *)addr)->sin6_port;

      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::getPortNumber(): unknown address type" << endl;
#endif
      _error = OSC_IN_ADDR_UNKNOWN;
    }

  return ntohs(port);
} // getPortNumber


bool
InetOSC::setSocketNonBlock (int *const sd)
{
  if (fcntl(*sd, F_SETFL, O_NONBLOCK))
    {
      _lasterrno = errno;
      _error = OSC_IN_FCNTL_ERRNO;

#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::setSocketNonBlocking():fcntl(): " << strerror(errno) << endl;
#endif

      return false;
    }

  return true;
} // setSocketNonBlocking


bool
InetOSC::setSocketReuseAddr (int *const sd, bool reuse)
{
  int optval = reuse?1:0;

// cygwin prototype:  setsockopt(unsigned int, int, int, const char*, int)
#ifdef __CYGWIN__
#warning "cygwin build: setsockopt unfixed! lack of functionality imminent"
  if (0) 
#else
  if (setsockopt(*sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
#endif
    {
      _lasterrno = errno;
      _error = OSC_IN_SETSOCKOPT_ERRNO;

#ifdef LIBOSC_DEBUG
      cerr << "InetOSC::setSocketReuseAddr():setsockopt(): " << strerror(errno) << endl;
#endif

      return false;
    }

  return true;
} // setSocketReuseAddr


//EOF
