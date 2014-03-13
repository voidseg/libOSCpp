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

#ifndef LIBOSC_INETUDPMASTER_H
#define LIBOSC_INETUDPMASTER_H

#include "InetMaster.h"
#include "InetTransportManager.h"
#include "NullTransmit.h"

#ifdef LIBOSC_REPLY
#include "InetUDPReplier.h"
#endif

#include <sstream>
#include <iosfwd>

/**
 * @class InetUDPMaster
 *
 *  handle UDP input and output.
 *
 *  this class listens on a particular UDP port,
 *  transmits data using it or just transmits data
 *  without receiving
 *
 *  @author Martin Rumori
 */
class InetUDPMaster : public InetMaster
{
#ifdef LIBOSC_REPLY
    friend class InetUDPReplier;
#endif
public:
  /** constructor
   *
   *  \param addrType the address type to use for this object
   *
   *  \bugs currently only IPv4 is tested
   */
  InetUDPMaster (InetOSCAddrType addrType = OSC_IN_IPv4);

  /** copy constructor
   */
  InetUDPMaster (InetUDPMaster &);

  /// destructor
  virtual ~InetUDPMaster ();

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

  /** transmit outgoing data to a different address
   *
   *  \param address the address to send to
   *
   *  \param data the data buffer to be transmitted
   *
   *  \return true on success, false on error
   */
  bool transmitTo(struct sockaddr *address, const std::string &data);

  /** return socket descriptor
   *
   *  \return the socket descriptor of this object
   */
  const int getHandle () const;

  /** set address and port of the destination host
   *
   *  this method sets the destination address of data
   *  being sent with send()
   *
   *  \param host the hostname or address to send to
   *  \param port the port number to send to
   *
   *  \return true on success, false on failure
   */
  bool setDestination (const std::string& host,
		       const short port);

  /** set return port
   * same as setDestination above, but specialized a bit.
   */
  bool setReturnPort (const short port);

  /** start listening on port
   *
   *  this method bounds the socket descriptor to a particular
   *  port for listening.  all data sent by this object
   *  will appear as sent by this port at the other party,
   *  thus replies will occur at this object as well
   *
   *  \note before calling this method, you have to set
   *  a service manager for this object
   *
   *  \param port the port to listen to
   *
   *  \return true on success, false on failure
   */
  bool startlisten (const short port);

  /** stop listening
   *
   *  this method stops listening by deregistering this object
   *  from the service manager.  sending data is still possible
   *
   *  \note the port to which the descriptor is bound will not change.
   *  you have to delete this object or to call startlisten() again
   *  with a different port before you can reuse the original port
   *  of this object (e.g. in another InetUDPMaster object)
   *
   *  \return true on success, false on failure
   */
  bool stoplisten ();

  /** set the transport manager for this master
   *
   *  \note you may not call this method if this object
   *  is still listening (use stoplisten() first)
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

  virtual Transmit *clone ();

protected:


private:

  /// my TransportManager
  InetTransportManager *_transMan;

  /// my OSCProcessor
  Processor *_processor;

  /// listening state
  bool _listening;

  /// port listening to
  short _port;

  /// remote address for sending
  struct sockaddr *_remoteSendAddr;

  /// remote address of last received datagram
  struct sockaddr *_remoteRecvAddr;

  /// length of socket address structs
  socklen_t _addrlen;

  /// receive buffer
  char _rbuf[OSC_DGRAM_MAXLENGTH];

};

#endif // #ifndef LIBOSC_INETUDPMASTER_H
