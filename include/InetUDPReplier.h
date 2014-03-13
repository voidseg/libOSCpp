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
 * This class was written by Jared Jennings <jjenning@fastmail.fm>
 */

#ifndef LIBOSC_INETUDPREPLIER_H
#define LIBOSC_INETUDPREPLIER_H

#include "Transmit.h"
#include "NullTransmit.h"
#include "InetOSC.h"

#include <sstream>
#include <iosfwd>

class InetUDPMaster; // compiler food

/**
 * @class InetUDPReplier
 *
 *  this class sends from a particular UDP port
 *  using an InetUDPReplier.
 *
 *  @author Jared Jennings
 */
class InetUDPReplier : public Transmit, InetOSC {
public:
  /** constructor
   *
   *  \param master a pointer to the InetUDPMaster to use to transmit
   *  this InetUDPMaster is expected to be the same one that is used
   *  to listen on; a pointer to it will probably not dangle, because 
   *  that object will not go away as long as we're listening to the 
   *  network.
   */
  InetUDPReplier(InetUDPMaster *master);

  /** copy constructor
   */
  InetUDPReplier(InetUDPReplier &);

  /// destructor
  virtual ~InetUDPReplier ();

  /** transmit outgoing data
   *
   *  \param data the data buffer to be transmit
   *
   *  \return true on success, false on error
   */
  bool transmit(const std::string& data);

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
  bool setReturnPort(short);

  /** get human readable string describing the status of this object
   *
   *  \return status string of this object
   */
  const std::string getStatusString (void);

  std::string getName();

  virtual Transmit * clone();

protected:


private:

  /// remote address for sending
  struct sockaddr *_remoteSendAddr;

  /// length of socket address structs
  unsigned int _addrlen;

  /// the InetUDPMaster to use to send replies.
  InetUDPMaster *_master;

};

#endif // #ifndef LIBOSC_INETUDPREPLIER_H
