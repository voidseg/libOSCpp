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

/*
 * This class was written by Martin Rumori (martin@rumori.de)
 */

#ifndef LIBOSC_INETTCPMASTER_H
#define LIBOSC_INETTCPMASTER_H


#ifdef __CYGWIN__
#include <w32api/winsock2.h>
#include <w32api/ws2tcpip.h>
#define SOL_TCP 6
#else
#include <sys/types.h>
#include <netinet/tcp.h>
#endif

#include "InetMaster.h"
#include "InetTransportManager.h"

#include <vector>
#include <algorithm>
#include <sstream>
#include <iosfwd>

class InetTCPSlave; // compiler food

/**
 * @class InetTCPMaster
 *
 * handle tcp connections.
 *
 *  this class listens for inbound tcp objects,
 *  connects to other hosts via tcp,
 *  creates InetTCPSlave objects...
 *
 *  \see InetTCPSlave
 *
 *  @author Martin Rumori
 */
class InetTCPMaster : public InetMaster
{
public:
  /** constructor
   *
   *  \param addrType the address type to use for this object
   *
   *  \bugs currently only IPv4 is tested
   */
  InetTCPMaster (InetOSCAddrType addrType = OSC_IN_IPv4);

  /// destructor
  virtual ~InetTCPMaster ();

  /** read incoming data from socket
   *
   *  \return true on success, false on error
   */
  bool handle ();

  /** transmit outgoing data
   *
   *  this method transmits the given data over all
   *  associated InetTCPSlave objects
   *
   *  \note if at least one slave object returns
   *  an error from its transmit() method,
   *  this method will return false with an
   *  OSC_IN_UNKNOWN error.
   *  take care especially if you set one or more
   *  of the InetTCPSlave objects non blocking on write.
   *  you should use the slave's transmit() method directly
   *  to be sure to catch the OSC_IN_WOULDBLOCK error set
   *   by the slave instead of blocking
   *
   *  \param data the data buffer to be transmit
   *
   *  \return true on success, false on error
   */
  bool transmit(const std::string& data);

  /** return socket descriptor
   *
   *  \return the socket descriptor of this object
   */
  const int getHandle () const;

  /** process an osc message by forwarding it to our processor
   *
   *  \param data the osc message buffer
   *  \param reply the Transmit object to reply to
   *
   *  \return true on success, false on failure
   */
  bool process (const std::string& data, Transmit *reply);

  /** connect to host
   *
   *  this method creates a new socket, tries to connect
   *  to the given host, after the connection has been established,
   *  it creates a new InetTCPSlave object,
   *  associates it with this master and returns a pointer to it
   *
   *  \param host the hostname or address to connect to
   *  \param port the port number to connect to
   *  \param addrType the address type to use for the connection
   *
   *  \return a pointer the connected object on success,
   *  NULL on failure
   *
   *  \bugs currently only IPv4 is tested
   */
  InetTCPSlave *const newconnection (const std::string& host,
				     const short port,
				     const InetOSCAddrType addrType = OSC_IN_IPv4);

  /** close connections of all associated InetTCPSlave objects
   *
   *  this method will force all associated InetTCPSlave objects
   *  to shutdown their connections.  they will be deleted after
   *  the connections have been shut down
   *
   *  \note if at least one slave object returns
   *  an error from its transmit() method,
   *  this method will return false with an
   *  OSC_IN_UNKNOWN error.
   *
   *  \return true on success, false on failure
   */
  bool disconnectAll ();

  /** start listening on port
   *
   *  this method creates a socket for listening.  inbound connections
   *  are processed by creating a new InetTCPSlave object, which will
   *  get served by the servicemanager of this object
   *
   *  \param port the port to listen to
   *
   *  \return true on success, false on failure
   */
  bool startlisten (const short port);

  /** stop listening
   *
   *  this method stops listening. further inbound connections are not
   *  possible. established connections are not touched by this method
   *
   *  \return true on success, false on failure
   */
  bool stoplisten ();

  /** associate a slave with this master
   *
   *  \param slave a pointer to the slave object to register
   *
   *  \return true on success, false on failure
   */
  bool registerSlave (InetTCPSlave *slave);

  /** deregister a slave from this master
   *
   *  \param slave a pointer to the slave to deregister
   *
   *  \return true on success, false on failure
   */
  bool deregisterSlave (InetTCPSlave *slave);

  /** set the transport manager for this master
   *
   *  \note you may not set nor change the transport manager
   *  if there are still slaves being served or this
   *  object is listening for inbound connections
   *
   *  \param transMan a pointer to the transport manager
   *
   *  \return true on success, false on failure
   */
  bool setTransportManager (InetTransportManager *const transMan);

  /** set the processor for this object
   *
   *  \param processor a pointer to the OSCProcessor
   */
  void setProcessor (Processor *processor);

  /** return listening status of this object
   *
   *  \return true if listening, false if not listening
   */
  bool isListening ();

  /** return number of connected slaves
   *
   *  \return true: object is connected, false: object is disconnected
   */
  const int getConnectedSlavesNum (void) const;

  /** setMaxHandleCycles() to slaves default value
   *
   *  this method sets a new value of _maxHandleCycles
   *  to all InetTCPSlaves associated with this InetTCPMaster
   *  and sets the default value which is "inherited"
   *  by all new spawned slaves.
   *
   *  \see setMaxHandleCycles()
   *
   *  \param cycles value for slaves' _maxHandleCycles
   */
  void setSlavesMaxHandleCycles (const int cycles = -1);

  /** return setting for slaves' default _maxHandleCycle
   *
   *  \return the slaves' default value for _maxHandleCycle
   */
  const int getSlavesMaxHandleCycles (void) const;

  /** get human readable string describing the status of this object
   *
   *  \return status string of this object
   */
  const std::string getStatusString (void);

  /** get name (specializes getName in Transmit)
   *
   * \return a string which identifies this object (incl. type).
   */
  virtual std::string getName ();


protected:

  /** set TCP_NODELAY option of a tcp socket
   *
   *  \param sd pointer to the socket descriptor
   *  \param nodelay true: set TCP_NODELAY, false: unset TCP_NODELAY
   *
   *  \return true on success, false on failure
   */
  bool setSocketTcpNodelay (int *const sd, bool nodelay = true);


private:

  /// the vector of mastered InetTCPSlave objects
  std::vector<InetTCPSlave *> _slaves;

  /// my ServiceManager
  InetTransportManager *_transMan;

  /// my OSCProcessor
  Processor *_processor;

  /// listening state
  bool _listening;

  /// port listening to
  short _port;

  /// slaves' default value for _maxHandleCycles
  int _slavesMaxHandleCycles;

};

#endif // #ifndef LIBOSC_INETTCPMASTER_H
