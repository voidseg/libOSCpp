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

#ifndef LIBOSC_INETTCPSLAVE_H
#define LIBOSC_INETTCPSLAVE_H

#include "InetTransport.h"
#include "InetTCPMaster.h"

#include <errno.h>

#ifdef    __CYGWIN__
#include <w32api/winsock2.h>
#include <w32api/ws2tcpip.h>
#include <poll.h>
// this is defined in sys/socket.h, but we mustn't include that and winsock2
#define SHUT_RDWR 2
#else  // __CYGWIN__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // __CYGWIN__


/**
 * @class InetTCPSlave
 *
 *  Abstract an established TCP connection.
 *
 *  \see InetTCPMaster
 *
 *  @author Martin Rumori
 */
class InetTCPSlave : public InetTransport
{

  friend class InetTCPMaster;

public:
  /** constructor
   *
   *  \param tcpMaster the InetTCPMaster to which this slave belongs
   *  \param addrType the address type of this slave
   *  \param socket a connected socket to use with this object
   */
  InetTCPSlave (InetTCPMaster *tcpMaster,
		InetOSCAddrType addrType,
		int socket,
		struct sockaddr *remoteAddr);

  /// destructor
  virtual ~InetTCPSlave ();

  /** read incoming data from socket
   *
   *  \return true on success, false on error
   */
  bool handle ();

  /** transmit outgoing data
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

  /** close connection and deregister from InetTCPMaster
   *
   *  this method closes the connection, if it is established.
   *  it deregisters this object from the
   *  associated InetTCPMaster which deregisters it from
   *  serviceManager
   *
   *  \return true on success, false on failure
   */
  bool disconnect ();

  /** get a pointer to the struct sockaddr containing the remote host address
   *
   *  \return a pointer to the remote host address struct
   */
  const struct sockaddr *const getRemoteAddr () const;

  /** get the remote host name in string notation
   *
   *  \return a string containing the host name in character notation
   */
  const std::string getRemoteHost () const;

  /** get the remote port
   *
   *  \return the remote port
   */
  const int getRemotePort () const;

  /** return connect status of this object
   *
   *  \return true: object is connected, false: object is disconnected
   */
  const bool isConnected () const;

  /** return current blocking on write setting of this object
   *
   *  \return true: objects block on write,
   *  false: object returns error (EAGAIN) if writing would block
   */
  const bool isWriteBlocking () const;

  /** set blocking on write behaviour of this object
   *
   *  \param writeblock true: block if writing is not
   *  currently possible,
   *  false: return false and set OSC_IN_WOULDBLOCK instead
   *
   *  \note this feature is not yet implemented
   */
  void setWriteBlocking (bool writeblock = true);

  /** get human readable string describing the status of this object
   *
   *  \return status string of this object
   */
  const std::string getStatusString (void);

  /** get name (specializes getName in Transmit)
   *
   * \return a string which identifies this object (incl. type).
   */
  virtual std::string getName();


protected:

  /** close connection with out deregistering
   *
   *  this method closes the connection, if it is established.
   *  it DOES NOT deregister this object from the
   *  associated InetTCPMaster which deregisters it from
   *  serviceManager.
   *
   *  \note this is used by the disconnectAll() method
   *  of the InetTCPMaster itself.
   *
   *  \return true on success, false on failure
   */
  bool justDisconnect ();


private:

  /// state of this object
  typedef enum
    {
      IDLE,		// nothing to do
      GETTING_SIZE,	// reading the packet size
      GETTING_PACKET,	// reading the packet itself
      DROPPING_PACKET	// dropping a packet being too long
			// (to regain 'sync' with data stream)
    } InetTCPSlaveState;

  /// connect status of this object
  bool _connected;

  /// block on EAGAIN while sending or return error
  bool _writeblock;

  /// receive state of this object
  InetTCPSlaveState _rstate;

  /// receive buffer string
  std::string _rbuf;

  /// size of next entity to read
  uint32_t _rsize;

  /// number of bytes read so far
  uint32_t _gotbytes;

  /// my InetTCPMaster
  InetTCPMaster *const _tcpMaster;

  /// the host name/address to which connected to
  const struct sockaddr *_remoteAddr;

};

#endif // #ifndef LIBOSC_INETTCPSLAVE_H
