/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002-2004 libOSC++ contributors. See AUTHORS		     */
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

#include "InetTCPSlave.h"
#include <sstream>

using namespace std;

InetTCPSlave::InetTCPSlave (InetTCPMaster *tcpMaster,
			    InetOSCAddrType addrType,
			    int socket,
			    struct sockaddr *remoteAddr) :
  _tcpMaster (tcpMaster),
  _remoteAddr (remoteAddr)
{
  _socket = socket;
  _addrType = addrType;
  _rstate = GETTING_SIZE; // the first we will read is a size count
  _rsize = 0;
  _gotbytes = 0;
  _writeblock = true;
  _connected = true;

} // InetTCPSlave


InetTCPSlave::~InetTCPSlave ()
{
  _tcpMaster->deregisterSlave(this);

  if (_connected)
    {
      disconnect();
    }

  free((void *)_remoteAddr);

} // ~InetTCPSlave


bool
InetTCPSlave::handle ()
{
  if (! _connected)
    {
      _error = OSC_IN_CLOSED_CONN;
      return false;
    }

  const uint32_t buflen = 256;
  char buffer[buflen]; // temporary read buffer
  int32_t read;
  int cycles = _maxHandleCycles;

  while (cycles--)
    {
      // reset cycles if necessary
      if (cycles < 0)
	{
	  cycles = -1;
	}

      // let's read first
      if ((read = recv(_socket, buffer, buflen, 0)) == -1)
	{
	  if (errno == EINTR) // interrupted by signal
	    {
#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPSlave::handle(): recv() returned EINTR (interrupted by signal): retrying" << endl;
#endif
	      continue;
	    }
	  else if (errno == EAGAIN) // no more data
	    {
#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPSlave::handle(): recv() returned EAGAIN (would block): waiting for next cycle" << endl;
#endif
	      _error = OSC_IN_SUCCESS;

	      return true;
	    }
	  else // other error
	    {
#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPSlave::handle(): recv(): " << strerror(errno) << endl;
#endif
	      _lasterrno = errno;
	      _error = OSC_IN_RECV_ERRNO;

	      return false;
	    }
	}
      else if (! read)  // nothing read = closed connection
	{
#ifdef LIBOSC_DEBUG
	  cerr << "InetTCPSlave::handle(): peer closed connection" << endl;
#endif

	  // close socket
 	  closeSocket(&_socket, false);
 	  _error = OSC_IN_CLOSED_CONN;
	  _connected = false;
 	  return false;
	}
      else // read something
	{
	  int32_t processed = 0; // num of processed bytes
	  int32_t restsize = 0;

	  // now let's process the received buffer
	  while (processed < read)
	    {
	      switch (_rstate)
		{
		case GETTING_SIZE :
		  restsize = 4 - _gotbytes;
		  if (restsize > read - processed)
		    { // not more than we've got
		     restsize = read - processed;
		    }

		  // copy missing bytes of size
		  for (int i = 0; i < restsize; ++i)
		    {
		      *(((char *)&_rsize) + i) = *(buffer + processed + i);
		    }

		  _gotbytes += restsize;
		  processed += restsize;

		  // got complete size?
		  if (_gotbytes == 4)
		    {
		      _gotbytes = 0;

		      // convert to host byte order
		      _rsize = ntohl(_rsize);

#ifdef LIBOSC_DEBUG
			  cerr << "InetTCPSlave::handle(): next packet will be " << _rsize << " bytes long" << endl;
#endif
		      if (_rsize <= OSC_STREAM_MAXLENGTH)
			{
			  _rstate = GETTING_PACKET;
			}
		      else
			{
#ifdef LIBOSC_DEBUG
			  cerr << "InetTCPSlave::handle(): next packet is too long. dropping" << endl;
#endif
			  _rstate = DROPPING_PACKET;
			}
		    }
		  break; // case GETTING_SIZE

		case GETTING_PACKET :

		  restsize = _rsize - _gotbytes; // rest of entity
		  if (restsize > read - processed)
		    { // not more than we've got
		      restsize = read - processed;
		    }

		  // save received part of message
		  _rbuf.append(buffer + processed, restsize);

		  _gotbytes += restsize;
		  processed += restsize;

		  if (_gotbytes == _rsize)
		    {
		      // process message
		      /* handing out our bare pointer
		       * is MEGA-UGLY.
		       * we should use smartpointers here
		       */
		      _tcpMaster->process(_rbuf, this);

		      // reset
		      _rbuf = "";
		      _gotbytes = 0;
		      _rstate = GETTING_SIZE;
		    }
		  break; // case GETTING_PACKET


		case DROPPING_PACKET :

		  restsize = _rsize - _gotbytes; // rest of entity
		  if (restsize > read - processed)
		    { // not more than we've got
		      restsize = read - processed;
		    }

		  _gotbytes += restsize;
		  processed += restsize;

		  if (_gotbytes == _rsize)
		    {
		      _gotbytes = 0;
		      _rstate = GETTING_SIZE;
		    }
		  break; // case DROPPING_PACKET

		default :
#ifdef LIBOSC_DEBUG
		  cerr << "InetTCPSlave::handle(): invalid value in _rstate" << endl;
#else
		  ; // food for the compiler
#endif
		} // switch ...
	    } // while (processed < read)
	} // else // read something
    } // while (1)

  // only reached if no EAGAIN before _maxHandleCycles
#ifdef LIBOSC_DEBUG
  cerr << "InetTCPSlave::handle(): _maxHandleCycles reached. leaving" << endl;
#endif
  _error = OSC_IN_MAXCYCLES;

  return true; // strictly speaking we had to return false here...
} //handle


bool
InetTCPSlave::transmit (const std::string& data)
{
  /* sending is blocking (by default)
   * if _writeblock is set, we return false on an EAGAIN
   * and set _error to OSC_IN_WOULDBLOCK.
   * handling of this error is delegated to the caller
   */

  if (! _connected)
    {
      _error = OSC_IN_CLOSED_CONN;
      return false;
    }

  int32_t size = data.length(); // num of bytes to send
  int32_t sent = 0; // num of bytes sent
  int32_t wrote;

  // prepend size to the data
#ifdef LIBOSC_DEBUG
  cerr << "InetTCPSlave::transmit(): transmitting " << size << " + 4 bytes" << endl;
#endif

  uint32_t nsize = htonl(size);
  // local copy to prepend size
  std::string sizeddata = string((char *)&nsize, 4);
  sizeddata.append(data);
  size += 4;

  while (sent < size)
    {
#ifdef __CYGWIN__
      if ((wrote = send(_socket,
                        (const char *)(sizeddata.c_str() + sent),
                        size - sent,
                        0)) == -1)
#else
      if ((wrote = send(_socket,
			(void *)(sizeddata.c_str() + sent),
			size - sent,
			0)) == -1)
#endif
	{
	  switch (errno)
	    {
	    case EINTR :
#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPSlave::transmit(): send(): got signal. retrying" << endl;
#endif
	      continue; // while loop

	    case EAGAIN :

	      if (1) // _writeblock)
		{
#ifdef LIBOSC_DEBUG
		  cerr << "InetTCPSlave::transmit(): send(): EAGAIN and we are set to writeblock. waiting for _socket" << endl;
#endif

		  // init poll struct
		  struct pollfd pollstruct;
		  pollstruct.fd = _socket;
		  pollstruct.events = POLLOUT;

		  // poll for _socket (infinitely for now)
		  while (1)
		    {
		      if (poll(&pollstruct, 1, -1) == -1)
			{
			  if (errno == EINTR)
			    {
			      continue;
			    }
			  else
			    {
			      _error = OSC_IN_POLL_ERRNO;
			      _lasterrno = errno;

			      return false;
			    }
			}
		      else
			{
			  break;
			}
		    } // while (1)

		  // check for event
		  switch (pollstruct.revents)
		    {
		    case POLLOUT :
#ifdef LIBOSC_DEBUG
		      cerr << "InetTCPSlave::transmit(): poll(): got POLLOUT event. continuing" << endl;
#endif
		      continue;

		    case POLLERR :
#ifdef LIBOSC_DEBUG
		      cerr << "InetTCPSlave::transmit(): poll(): got POLLERR event. leaving" << endl;
#endif
		      _error = OSC_IN_UNKNOWN;
		      return false;

		    case POLLHUP :
#ifdef LIBOSC_DEBUG
		      cerr << "InetTCPSlave::transmit(): poll(): got POLLHUP event. leaving" << endl;
#endif
		      // close socket
		      closeSocket(&_socket, false);
		      _error = OSC_IN_CLOSED_CONN;
		      _connected = false;
		      return false;

		    default :
#ifdef LIBOSC_DEBUG
		      cerr << "InetTCPSlave::transmit(): poll(): got unknown event. leaving" << endl;
#endif
		      _error = OSC_IN_UNKNOWN;
		      return false;

		    } // switch (pollstruct.revents)
		} // if (_writeblock)
	      else
		{
#ifdef LIBOSC_DEBUG
		  cerr << "InetTCPSlave::transmit(): send(): EAGAIN and we are set to not blocking on write. leaving" << endl;
#endif
		  _error = OSC_IN_WOULDBLOCK;

		  return false;
		}

	    default:
	      _error = OSC_IN_SEND_ERRNO;
	      _lasterrno = errno;
	      return false;

	    } // switch (errno)
	}
      else // wrote something
	{
	  sent += wrote;
	}
    } // while (1)

  return true;
} // transmit


const int
InetTCPSlave::getHandle () const
{
  return (const int)_socket;

} // getHandle


bool
InetTCPSlave::disconnect ()
{
  _tcpMaster->deregisterSlave(this);

  return justDisconnect();
 } // disconnect


const struct sockaddr *const
InetTCPSlave::getRemoteAddr () const
{
  return (const struct sockaddr *const)_remoteAddr;
} // getRemoteAddr


const string
InetTCPSlave::getRemoteHost () const
{
  string host;

  switch (_addrType)
    {
    case OSC_IN_IPv4 :
      {
	char hostbuf[INET_ADDRSTRLEN];
#ifdef __CYGWIN__
#warning "no inet_ntop!"
#else
	host.assign(inet_ntop(AF_INET,
			      (void *)&(((sockaddr_in *)_remoteAddr)->sin_addr),
			      hostbuf,
			      INET_ADDRSTRLEN));
#endif
      }
      break;

    case OSC_IN_IPv6 :
      {
	char hostbuf[INET6_ADDRSTRLEN];
#ifdef __CYGWIN__
#warning "no inet_ntop!"
#else
	host.assign(inet_ntop(AF_INET6,
			      (void *)&(((sockaddr_in6 *)_remoteAddr)->sin6_addr),
			      hostbuf,
			      INET6_ADDRSTRLEN));
#endif
      }
      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPSlave::getRemoteHost(): _addrType has invalid value" << endl;
#else
      ; // compiler food
#endif
    }

  return host;
} // getRemoteHost


const int
InetTCPSlave::getRemotePort () const
{
  short port = 0;

  switch (_addrType)
    {
    case OSC_IN_IPv4 :

      port = ((sockaddr_in *)_remoteAddr)->sin_port;
      break;

    case OSC_IN_IPv6 :

      port = ((sockaddr_in6 *)_remoteAddr)->sin6_port;
      break;

    default :
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPSlave::getRemotePort(): _addrType has invalid value" << endl;
#else
      ; // compiler food
#endif
    }

  return (const int)port;

} // getRemotePort


const bool
InetTCPSlave::isConnected () const
{
  return _connected;

} // isConnected


const bool
InetTCPSlave::isWriteBlocking () const
{
  return _writeblock;

} // isWriteBlocking


void
InetTCPSlave::setWriteBlocking (bool writeblock)
{
  _writeblock = writeblock;

} // setWriteBlocking


const std::string
InetTCPSlave::getStatusString (void)
{
  ostringstream status;

  status << "InetTCPSlave: ";

  if (_connected)
    {
      status << "connected to host: "
	     << getHostAddress(_addrType, _remoteAddr)
	     << ", port: "
	     << (unsigned short)getPortNumber(_addrType, _remoteAddr)
	     << ends;
    }
  else
    {
      status << "<not connected>" << endl;
    }

  return status.str();
} // getStatusString


bool
InetTCPSlave::justDisconnect ()
{
  if (shutdown(_socket, SHUT_RDWR))
    {
      _error = OSC_IN_SHUTDOWN_ERRNO;
      _lasterrno = errno;
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPSlave::justDisconnect():shutdown(): " << strerror(errno) << endl;
#endif

      return false;
    }

  if (! closeSocket(&_socket))
    {
      return false;
    }

  _connected = false;
  _error = OSC_IN_SUCCESS;

  return true;
} // justDisconnect


std::string
InetTCPSlave::getName ()
{
  ostringstream name;
  name << "InetTCPSlave@" << this;
  return name.str();
}

// EOF
