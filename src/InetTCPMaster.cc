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

#include "InetTCPMaster.h"
#include "InetTCPSlave.h"

using namespace std;

InetTCPMaster::InetTCPMaster (InetOSCAddrType addrType)
{
  _addrType = addrType;
  _processor = 0;
  _transMan = 0;
  _port = 0;
  _listening = false;

  setSlavesMaxHandleCycles();

} // InetTCPMaster


InetTCPMaster::~InetTCPMaster ()
{
  if (_listening)
    {
      stoplisten();
    }

  disconnectAll();

} // ~InetTCPMaster


bool
InetTCPMaster::handle ()
{
  // accept connection and create new slave object
  int connsock;
  socklen_t addrlen;
#ifdef __CYGWIN__
  // no problems here: the address length should be well under 2GB.
  int cyg_addrlen;
#endif
  struct sockaddr *addr = 0;

  if (! allocSockAddr(_addrType, &addr, &addrlen))
    {
      return false;
    }

  int cycles = _maxHandleCycles;

  while (cycles--)
    {
      // reset cycles if necessary
      if (cycles < 0)
	{
	  cycles = -1;
	}

#ifdef __CYGWIN__
      cyg_addrlen = addrlen;
      if ((connsock = accept(_socket, addr, &cyg_addrlen)) == -1)
#else
      if ((connsock = accept(_socket, addr, &addrlen)) == -1)
#endif
	{
	  switch (errno)
	    {
	    case EINTR : // interrupted by signal

#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPMaster::handle():accept(): EINTR. retrying" << endl;
#endif
	      continue;

	    case EAGAIN : // no connection pending

#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPMaster::handle():accept(): EAGAIN. leaving" << endl;
#endif
	      _error = OSC_IN_SUCCESS;
	      free(addr);

	      return true;

	    default : // other error

#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPMaster::handle():accept(): " << strerror(errno) << endl;
#endif
	      _lasterrno = errno;
	      _error = OSC_IN_ACCEPT_ERRNO;
	      free(addr);

	      return false;

	    } // switch (errno)
	} // if (accept(...) == -1)
      else // new connection established
	{
	  setSocketNonBlock(&connsock);
#ifdef LIBOSC_DEBUG
	  setSocketReuseAddr(&connsock);
#endif

	  InetTCPSlave *slave = new InetTCPSlave(this,
						 _addrType,
						 connsock,
						 addr);
	  slave->setMaxHandleCycles(_slavesMaxHandleCycles);
	  registerSlave(slave);

#ifdef LIBOSC_DEBUG
	  cerr << "InetTCPMaster::handle(): new inbound connection (sd " << connsock << ")" << endl;
#endif

	  // alloc mem for next cycle
	  if (! allocSockAddr(_addrType, &addr, &addrlen))
	    {
	      return false;
	    }
	}
    } // while (1)

  // only reached if no EAGAIN before _maxHandleCycles
#ifdef LIBOSC_DEBUG
  cerr << "InetTCPMaster::handle(): _maxHandleCycles reached. leaving" << endl;
#endif
  _error = OSC_IN_MAXCYCLES;

  return true; // strictly speaking we had to return false here...
} // handle


bool
InetTCPMaster::transmit (const std::string& data)
{
  std::vector<InetTCPSlave *>::iterator iter = _slaves.begin();
  bool error = true;

  while (iter != _slaves.end())
    {
      error &= (*iter)->transmit(data);
      ++iter;
    }

  if (! error)
    {
      _error = OSC_IN_UNKNOWN;
      return false;
    }

  _error = OSC_IN_SUCCESS;
  return true;
} // transmit


const int
InetTCPMaster::getHandle () const
{
  return _socket;
} // handle


bool
InetTCPMaster::process (const std::string& data, Transmit *reply)
{
  if (_processor)
    {
      _processor->process(data, reply);
    }
  else
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::process(): no processor set" << endl;
#endif

      _error = OSC_IN_NO_PROCESSOR;
      return false;
    }

  return true;
} // process


InetTCPSlave *const
InetTCPMaster::newconnection (const std::string& host,
			      const short port,
			      const InetOSCAddrType addrType)
{
  /* connecting by now is blocking
   * this may block the system until having sucessfully
   * connecting or timeout
   */

  if (! _transMan)
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::connect(): no transport manager set. aborting" << endl;
#endif
      _error = OSC_IN_NO_TRANSMGR;
      return 0;
    }

  // build address struct
  struct sockaddr *addr = 0;
  socklen_t addrlen;

  if (! allocSockAddr(addrType, &addr, &addrlen))
    {
      return 0;
    }

  if (! setRemoteAddr(host, htons(port), addrType, addr))
    {
      free(addr);
      return 0;
    }

  // create socket
  int newsock;

  if (! createSocket(&newsock, OSC_IN_TCP, addrType))
    {
      free(addr);
      return 0;
    }

  // connect
  while (1)
    {
      if (connect(newsock, addr, addrlen))
	{
	  switch (errno)
	    {
	    case EINTR :
#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPMaster::newconnection():connect(): EINTR. retrying" << endl;
#endif
	      continue;

	    default :
	      _lasterrno = errno;
	      _error = OSC_IN_CONNECT_ERRNO;

#ifdef LIBOSC_DEBUG
	      cerr << "InetTCPMaster::newconnection():connect(): " << strerror(errno) << endl;
#endif
	      closeSocket(&newsock, false);
	      free(addr);

	      return 0;
	    } // switch (errno)
	} // if (connect(...))

      break; // success
    } // while (1)

  setSocketNonBlock(&newsock);

  if (InetMaster::_reuseAddr)
    {
      setSocketReuseAddr(&newsock);
    }

  InetTCPSlave *slave = new InetTCPSlave(this,
					 addrType,
					 newsock,
					 addr);
  registerSlave(slave);

#ifdef LIBOSC_DEBUG
  cerr << "InetTCPMaster::newconnection(): new outbound connection (sd " << newsock << ")" << endl;
#endif

  return slave;
} // newconnection


bool
InetTCPMaster::disconnectAll ()
{
  bool error = true;
  InetTCPSlave *slave;

  while (! _slaves.empty())
    {
      slave = _slaves.back();
      error &= slave->justDisconnect();
      delete slave; // this deregisters from _transMan as well
    }

  if (error)
    {
      _error = OSC_IN_SUCCESS;
    }
  else
    {
      _error = OSC_IN_UNKNOWN;
    }

  return error;
} // disconnectAll


bool
InetTCPMaster::startlisten (const short port)
{
  if (! _transMan)
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::startlisten(): no transport manager set. aborting" << endl;
#endif

      _error = OSC_IN_NO_TRANSMGR;
      return false;
    }

  _port = port;

  // create socket
  if (! createSocket(&_socket, OSC_IN_TCP, _addrType))
    {
      return false;
    }

  // set socket nonblocking
  setSocketNonBlock(&_socket);

  // reuse address, if desired
  if (_reuseAddr)
    {
      setSocketReuseAddr(&_socket);
    }

  // bind to any interface
  if (! bindSocketToAny(&_socket, htons(port), _addrType))
    {
      closeSocket(&_socket, false);
      return false;
    }

  // establish listen queue
  if (listen(_socket, OSC_STREAM_MAXCONN))
    {
      _lasterrno = errno;
      _error = OSC_IN_LISTEN_ERRNO;
      closeSocket(&_socket, false);

#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::startlisten():listen(): " << strerror(errno) << endl << endl;
#endif

      return false;
    }

  // add to the service manager and finish
  _transMan->addInetTransport(this);
  _listening = true;

  return true;
} // startlisten


bool
InetTCPMaster::stoplisten ()
{

  if (! closeSocket(&_socket))
    {
      return false;
    }

  // delete from the service manager
  _transMan->delInetTransport(this);

  _listening = false;

  return true;
} // stoplisten


bool
InetTCPMaster::registerSlave (InetTCPSlave *slave)
{
  if (! _transMan)
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::registerSlave(): no transport manager set. aborting" << endl;
#endif
      _error = OSC_IN_NO_TRANSMGR;
      return false;
    }

  std::vector<InetTCPSlave *>::const_iterator iter
    = find(_slaves.begin(), _slaves.end(), slave);

  if (iter != _slaves.end())
    { 
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::registerSlave(): slave already registered" << endl;
#endif
      _error = OSC_IN_ALREADY_SLAVE;
     return false;
    }

  _slaves.push_back(slave);

  _transMan->addInetTransport(slave);

#ifdef LIBOSC_DEBUG
  cerr << "InetTCPMaster::registerSlave(): we have now " << _slaves.size() << " slaves" << endl;
#endif

  return true;
} // registerSlave


bool
InetTCPMaster::deregisterSlave (InetTCPSlave *slave)
{
  std::vector<InetTCPSlave *>::iterator iter
    = find(_slaves.begin(), _slaves.end(), slave);

  if (iter == _slaves.end())
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::deregisterSlave(): slave not registered" << endl;
#endif
      _error = OSC_IN_NOT_SLAVE;
      return false;
    }

  /* that depends: perhaps this master get notified
   * if the transport manager has removed the object
   * itself after a false returning handle() method...
   * but since the InetTransportManager checks itself for
   * duplicated delService() calls, it is safe to do so
   */
  _transMan->delInetTransport(slave);

  _slaves.erase(iter);

  return true;
} // deregisterSlave


bool
InetTCPMaster::setTransportManager (InetTransportManager *transMan)
{
  if (_slaves.empty() && ! _listening)
    {
      _transMan = transMan;
    }
  else
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::setTransportManager(): object is still listening or there are still slaves being served" << endl;
#endif

      _error = OSC_IN_NOT_IDLE;
      return false;
    }

  return true;
} // setTransportManager


void
InetTCPMaster::setProcessor (Processor *processor)
{
  _processor = processor;
} // setProcessor


bool
InetTCPMaster::isListening ()
{
  return _listening;

} // isListening


const int
InetTCPMaster::getConnectedSlavesNum (void) const
{
  int num = 0;

  // get connection state from all slaves
  std::vector<InetTCPSlave *>::const_iterator iter = _slaves.begin();

  while (iter != _slaves.end())
    {
      if ((*iter)->isConnected())
	{
	  ++num;
	}
      ++iter;
    }

  return num;
} // getConnectedSlavesNum


void
InetTCPMaster::setSlavesMaxHandleCycles (const int cycles)
{
  _slavesMaxHandleCycles = cycles;

  // set new value to all registered slaves
  std::vector<InetTCPSlave *>::iterator iter = _slaves.begin();

  while (iter != _slaves.end())
    {
      (*iter)->setMaxHandleCycles(cycles);
      ++iter;
    }
} // setSlavesMaxHandleCycles


const int
InetTCPMaster::getSlavesMaxHandleCycles (void) const
{
  return _slavesMaxHandleCycles;

} // getSlavesMaxHandleCycles


const std::string
InetTCPMaster::getStatusString (void)
{
  ostringstream status;

  status << "InetTCPMaster: ";

  if (_listening)
    {
      status << "listening to port: ";
      status << _port;
    }
  else
    {
      status << "<not listening>";
    }

  status << ". currently "
	 << _slaves.size()
	 << " slaves"
	 << ends;

  return status.str();
} // getStatusString


/** Uniquely identifies the other end of this connection (i.e. address and
 * port). Does not uniquely identify this object (i.e. should not contain any
 * representation of this), because we clone InetUDPMasters sometimes, and we
 * want to know if they're connected to the same other end. */
std::string
InetTCPMaster::getName ()
{
  ostringstream name;
  name << "InetTCPMaster@" << this;
  return name.str();
}


bool
InetTCPMaster::setSocketTcpNodelay (int *const sd, bool nodelay)
{
  int optval = nodelay?1:0;

#ifdef __CYGWIN__
#warning "setsockopt prototype problem"
  if (0)
#else
  if (setsockopt(*sd, getprotobyname("tcp")->p_proto, TCP_NODELAY, &optval, sizeof(optval)))
#endif
    {
      _lasterrno = errno;
      _error = OSC_IN_SETSOCKOPT_ERRNO;

#ifdef LIBOSC_DEBUG
      cerr << "InetTCPMaster::setSocketTcpNodelay():setsockopt(): " << strerror(errno) << endl;
#endif

      return false;
    }

  return true;
} // setSocketTcpNodelay


// EOF
