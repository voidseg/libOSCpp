/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002-2004 libOSC++ contributors. See AUTHORS		         */
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

/*
 * This class was written by Martin Rumori (martin@rumori.de)
 */

#include "InetUDPMaster.h"
#include <sstream>

#ifdef LIBOSC_REPLY
#include "InetUDPReplier.h"
#endif

using namespace std;

InetUDPMaster::InetUDPMaster (InetOSCAddrType addrType)
{
  _addrType = addrType;
  _processor = 0;
  _transMan = 0;
  _listening = false;
  _remoteSendAddr = NULL;
  _remoteRecvAddr = NULL;

  /* try to create socket
   * we can't return (false) on error,
   * so success should be tested with getError()
   * immediately after construction of this object
   */
  createSocket(&_socket, OSC_IN_UDP, addrType);

  // see above
  setSocketNonBlock(&_socket);

  // see above
  allocSockAddr(addrType, &_remoteSendAddr, &_addrlen);
  allocSockAddr(addrType, &_remoteRecvAddr, &_addrlen);

} // InetUDPMaster


InetUDPMaster::~InetUDPMaster ()
{
  if (_listening)
    {
      stoplisten();
    }

  free(_remoteSendAddr);
  free(_remoteRecvAddr);

  closeSocket(&_socket);

} // ~InetUDPMaster


InetUDPMaster::InetUDPMaster (InetUDPMaster &from)
{
  _addrType = from._addrType;
  _processor = from._processor;
  _transMan = from._transMan;
  _listening = false;
  _remoteSendAddr = NULL;
  _remoteRecvAddr = NULL;

  /* try to create socket
   * we can't return (false) on error,
   * so success should be tested with getError()
   * immediately after construction of this object
   */
  createSocket(&_socket, OSC_IN_UDP, _addrType);

  // see above
  setSocketNonBlock(&_socket);

  // see above
  allocSockAddr(_addrType, &_remoteSendAddr, &_addrlen);
  allocSockAddr(_addrType, &_remoteRecvAddr, &_addrlen);


  memcpy(_remoteSendAddr, from._remoteSendAddr, _addrlen);
  memcpy(_remoteRecvAddr, from._remoteRecvAddr, _addrlen);
  memcpy(_rbuf, from._rbuf, OSC_DGRAM_MAXLENGTH);

  if(from._listening) {
	  this->startlisten(from._port);
  }
}


bool
InetUDPMaster::handle ()
{
  int32_t length;
  int cycles = _maxHandleCycles;

  while (cycles--)
    {
      // reset cycles if necessary
      if (cycles < 0)
	{
	  cycles = -1;
	}

      if ((length = recvfrom(_socket, &_rbuf, OSC_DGRAM_MAXLENGTH, 0,
			     _remoteRecvAddr, &_addrlen)) == -1)
	{
	  switch (errno)
	    {
	    case EINTR : // interrupted by signal

#ifdef LIBOSC_DEBUG
            std::cerr << "InetUDPMaster::handle(): recvfrom(): got EINTR. retrying" << std::endl;
#endif
	      continue; // while (1)

	    case EAGAIN : // no more data

#ifdef LIBOSC_DEBUG
          std::cerr << "InetUDPMaster::handle(): recvfrom(): got EAGAIN. leaving" << std::endl;
#endif
	      _error = OSC_IN_SUCCESS;

	      return true;

	    default :

	      _lasterrno = errno;
	      _error = OSC_IN_RECV_ERRNO;

#ifdef LIBOSC_DEBUG
          std::cerr << "InetUDPMaster::handle(): recvfrom(): " << strerror(errno) << std::endl;
#endif

	      return false;

	    } // switch (errno)
	} // if (recvfrom (...) == -1)
      else // success
	{
	  // process message
	  if (_processor)
	    {
	      string rhost = getHostAddress(_addrType, _remoteRecvAddr);
	      short port = getPortNumber(_addrType, _remoteRecvAddr);
	      bool error = false;

#ifdef LIBOSC_REPLY

	      Transmit *reply = new InetUDPReplier(this);
	      error |= ((InetOSC *)reply)->getError();

#ifdef LIBOSC_DEBUG
	      //	      std::cerr << "InetUDPMaster::handle(): reply object points to " << getHostName(_addrType, _remoteRecvAddr) << " (" << rhost << ") port " << (uint16_t)port << std::endl;
          std::cerr << "InetUDPMaster::handle(): reply object points to " << rhost << " port " << (uint16_t)port << std::endl;
#endif // #ifdef LIBOSC_DEBUG

	      error |= (! rhost.size());
	      error |= (port == -1);
	      error |= (! ((InetUDPReplier *)reply)->setDestination(rhost, port));

	      if (error)
		{
#ifdef LIBOSC_DEBUG
            std::cerr << "InetUDPMaster::handle(): creation of InetUDPReplier object failed. using NullTransmit" << std::endl;
#endif // #ifdef LIBOSC_DEBUG
		  delete reply;
		  reply = new NullTransmit();
		}

	      _processor->process(string(_rbuf, length), reply);
	      // clients have to copy reply ...
	      delete reply;

#else // #ifdef LIBOSC_REPLY

#ifdef LIBOSC_DEBUG
          std::cerr << "InetUDPMaster::handle(): setting reply destination to" << rhost << " port " << (uint16_t)port << std::endl;
#endif // #ifdef LIBOSC_DEBUG
	      error |= (! setDestination(rhost, port));

#ifdef LIBOSC_DEBUG
	      if (error)
		{
            std::cerr << "InetUDPMaster::handle(): setting of reply destination failed. keeping it as is" << std::endl;
		}
#endif // #ifdef LIBOSC_DEBUG
	      _processor->process(string(_rbuf, length), this);

#endif // #ifdef LIBOSC_REPLY
	    }
	  else // no processor set
	    {
#ifdef LIBOSC_DEBUG
            std::cerr << "InetUDPMaster::handle(): no processor set" << std::endl;
#endif
	      _error = OSC_IN_NO_PROCESSOR;
	      return false;
	    }
	}
    } // while (1)

  // only reached if no EAGAIN before _maxHandleCycles
#ifdef LIBOSC_DEBUG
  std::cerr << "InetUDPMaster::handle(): _maxHandleCycles reached. leaving" << std::endl;
#endif
  _error = OSC_IN_MAXCYCLES;

  return true; // strictly speaking we had to return false here...
} // handle



// i almost #ifdef REPLY'd transmitTo, but it would have been too long and ugly
// - and #ifdef REPLY will go away some time, because sending (this) to
// the Transmit as a reply object (not #ifdef REPLY) and sending a new
// InetUDPReplier(this) to the Transmit as a reply object (#ifdef REPLY) 
// both have memory management problems: 
// they allow the possibility of dangling pointers.

bool
InetUDPMaster::transmit (const std::string& data)
{
    return this->transmitTo(_remoteSendAddr, data);
}

bool
InetUDPMaster::transmitTo (struct sockaddr *address, const std::string& data)
{
  while (1)
    {
      if (sendto(_socket, (void*)data.c_str(), data.size(), 0,
		 address, _addrlen) == -1)
	{
	  if (errno == EINTR)
	    {
#ifdef LIBOSC_DEBUG
            std::cerr << "InetUDPMaster::transmit(): sendto(): got EINTR. retrying" << std::endl;
#endif
	      continue; // while (1)
	    }
	  else
	    {
	      _lasterrno = errno;
	      _error = OSC_IN_SEND_ERRNO;

#ifdef LIBOSC_DEBUG
          std::cerr << "InetUDPMaster::transmit(): sendto(): " << strerror(errno) << std::endl;
#endif

	      return false;
	    }
	}
      else // success
	{
	  break;
	}
    } // while (1)

  _error = OSC_IN_SUCCESS;

  return true;
} // transmitTo


const int
InetUDPMaster::getHandle () const
{
  return _socket;
} // handle


bool
InetUDPMaster::setDestination (const std::string& host,
			       const short port)
{
  if (! setRemoteAddr(host, htons(port), _addrType, _remoteSendAddr))
    {
      return false;
    }

  return true;
} // setDestination


bool
InetUDPMaster::setReturnPort (const short port)
{
  // (why did we do that if above? lucidity i suppose?)
  return setRemotePort(htons(port), _addrType, _remoteSendAddr);
}


bool
InetUDPMaster::startlisten (const short port)
{
  if (! _transMan)
    {
#ifdef LIBOSC_DEBUG
        std::cerr << "InetUDPMaster::startlisten(): no transport manager set. aborting" << std::endl;
#endif
      _error = OSC_IN_NO_TRANSMGR;

      return false;
    }

  _port = port;

  // reuse address, if desired
  if (_reuseAddr)
    {
      setSocketReuseAddr(&_socket);
    }

  // bind
  if (! bindSocketToAny(&_socket, htons(port), _addrType))
    {
      return false;
    }

  // add to the transport manager and finish
  _transMan->addInetTransport(this);

  _listening = true;

  return true;
} // startlisten


bool
InetUDPMaster::stoplisten ()
{
  // delete from the service manager
  _transMan->delInetTransport(this);

  _listening = false;

  /* we close and recreate the socket, since another startlisten()
   * call would try to rebind the socket, which fails
   */
  if (! closeSocket(&_socket))
    {
      return false;
    }

  // recreate socket
  if (! createSocket(&_socket, OSC_IN_UDP, _addrType))
    {
      return false;
    }
  setSocketNonBlock(&_socket);

  return true;
} // stoplisten


bool
InetUDPMaster::setTransportManager (InetTransportManager *transMan)
{
  if (! _listening)
    {
      _transMan = transMan;
    }
  else
    {
#ifdef LIBOSC_DEBUG
        std::cerr << "InetUDPMaster::setTransportManager(): object ist still listening" << std::endl;
#endif

      _error = OSC_IN_NOT_IDLE;
      return false;
    }

  return true;
} // setTransManager


void
InetUDPMaster::setProcessor (Processor *processor)
{
  _processor = processor;
} // setProcessor


bool
InetUDPMaster::isListening ()
{
  return _listening;

} // isListening


const std::string
InetUDPMaster::getStatusString (void)
{
  ostringstream status;

  status << "InetUDPMaster: ";

  if (_listening)
    {
      status << "listening to port: ";
      status << _port;
    }
  else
    {
      status << "<not listening>" << endl;
    }

  status << "; sending to host: "
	 << getHostAddress(_addrType, _remoteSendAddr)
	 << ", port: "
	 << (unsigned short)getPortNumber(_addrType, _remoteSendAddr)
	 << ends;

  return status.str();
} // getStatusString


/** Uniquely identifies the other end of this connection (i.e. address and
 * port). Does not uniquely identify this object (i.e. should not contain any
 * representation of this), because we clone InetUDPMasters sometimes, and we
 * want to know if they're connected to the same other end. */
std::string
InetUDPMaster::getName ()
{
  ostringstream name;
  name << "[UDP master: " << getHostAddress(_addrType, _remoteSendAddr) 
       << " port " << (unsigned short) getPortNumber(_addrType, _remoteSendAddr) 
       << "]";
  return name.str();
}


Transmit *
InetUDPMaster::clone ()
{
  return new InetUDPMaster(*this);
}

// EOF
