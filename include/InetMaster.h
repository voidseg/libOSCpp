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

/**
 * @class InetMaster
 *
 * The internetwork masters' base class
 *
 * @author  Martin Rumori
 */

#ifndef LIBOSC_INETMASTER_H
#define LIBOSC_INETMASTER_H

#include "Processor.h"
#include "InetTransport.h"

class InetTransportManager; // compiler food

class InetMaster : public InetTransport
{
public:
  /// constructor
  InetMaster () { _reuseAddr = false; }

  /// destructor
  virtual ~InetMaster () { }

  /** start listening on port
   *
   *  \param port the port to listen to
   *
   *  \return true on success, false on failure
   */
  virtual bool startlisten (const short port) = 0;

  /** stop listening
   *
   *  \return true on success, false on failure
   */
  virtual bool stoplisten () = 0;

  /** set the processor for this object
   *
   *  \param processor a pointer to the OSCProcessor
   */
  virtual void setProcessor (Processor *processor) = 0;

  /** return listening status of this object
   *
   *  \return true if listening, false if not listening
   */
  virtual bool isListening () = 0;

  /** set the transport manager for this master
   *
   *  \param transMan a pointer to the transport manager
   *
   *  \return true on success, false on failure
   */
  virtual bool setTransportManager (InetTransportManager *const transMan) = 0;

  /** set SO_REUSEADDR after socket creation
   *
   * \param reuseAddr true: reuse socket address, false: do not reuse
   * socket address
   */
  void setReuseSockAddr (bool reuseAddr) { _reuseAddr = reuseAddr; }


protected :

  /// true: set SO_REUSEADDR on socket
  bool _reuseAddr;

};

#endif // #ifndef LIBOSC_INETMASTER_H
