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
 * @class InetTransport
 *
 * The internetwork services'/transmits' base class
 *
 * The handle() method should not block and should
 * do some small unit of work.  The Services are
 * designed to live in a single thread, so if one
 * does block other work will be suspended.
 *
 * @author  Martin Rumori
 */

#ifndef LIBOSC_INETTRANSPORT_H
#define LIBOSC_INETTRANSPORT_H

#include "Service.h"
#include "Transmit.h"
#include "InetOSC.h"


class InetTransport : public Service, public Transmit, public InetOSC
{
public:
  /// destructor
  virtual ~InetTransport () { }

  /** handle the service
   *
   *  \return true on success,
   *  false on error or if service has finished
   *  use getError() and getLastErrno() to get more
   *  information about the failure
   */
  bool handle() = 0;

  /** transmit outgoing data
   *
   *  \param data the data buffer to be transmit
   *
   *  \return true on success, false on error
   */
  bool transmit(const std::string& data) = 0;

  /** the network socket descriptor used by the service
   *
   *  this is used e.g. by the InetServiceManager to
   *  call a poll()
   *
   *  \return the socket descriptor of this service
   */
  virtual const int getHandle() const = 0;

  /** get human readable string describing the status of this object
   *
   *  \return status string of this object
   */
  virtual const std::string getStatusString (void) = 0;

  /** get name (specializes getName in Transmit)
   *
   * \return a string which identifies this object (incl. type).
   */
  virtual std::string getName () = 0;

};

#endif // #ifndef LIBOSC_INETTRANSPORT_H
