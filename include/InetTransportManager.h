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
 * @class InetTransportManager
 *
 * A service manager that holds its InetTransports in an STL map.
 *
 * It checks each service's handle for read
 * activity, if there is data available to
 * read, the service's handle() method is
 * called.
 * checking for write activity will be added later.
 *
 * @author  Daniel Holth
 * @author  Martin Rumori
 */

/*
 * this class was written by Martin Rumori (martin@rumori.de)
 */

#ifndef LIBOSC_INETTRANSPORTMANAGER_H
#define LIBOSC_INETTRANSPORTMANAGER_H

#include "ServiceManager.h"
#include "InetTransport.h"

#ifdef    __CYGWIN__
#include <w32api/winsock2.h>

#else  // __CYGWIN__
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/time.h>
#endif // __CYGWIN__
#include <errno.h>

#include <map>
#include <vector>


class InetTransportManager : public ServiceManager
{

public:
  /// constructor
  InetTransportManager ();

  /// destructor
  virtual ~InetTransportManager ();

  /** Run active transports.
   *
   *  This method waits for activity on all ports listened to
   *  by the transports in the transport map and calls the
   *  corresponding transports if necessary.
   *
   *  \note This method never stops listening.
   */
  void serveForever ();

  /** Run active transports once.
   *
   *  This method looks for activity on all ports listened to
   *  by the transports in the transport map and calls all
   *  necessary  transports to process the data.
   *
   *  \note This method is equivalent to runCylce(int timeout)
   *  with a timeout of 0.
   */
  void runCycle ();

  /** Run active transports once with given timeout.
   *
   *  This method looks for activity on all ports listened to
   *  by the transports in the transport map and calls all
   *  necessary transports to process the data.
   *
   *  \note This method is useful if you want to use this
   *  class synchronized from another mainloop ("poll").
   *
   *  \param timeout the timeout in milliseconds to wait on socket events
   */
  void runCycle (const int timeout);

  /** Add an InetTransport to the map of active transports.
   *
   *  \param itrans a pointer to the InetTransport to add
   */
  void addInetTransport (InetTransport *const itrans);

  /** Remove a transport from the map of active transports.
   *
   *  \param itrans a pointer to the InetTransport to remove
   */
  void delInetTransport (InetTransport *const itrans);

  /** Add a service to the map of active transport.
   *
   *  \param service a pointer to the service to add
   *
   *  \note since this ServiceManager is dedicated to the use
   *  with InetTransport, arguments to this function will
   *  get tried to dynamic cast them to (InetTransport *).
   *  if that fails, no action will be performed.
   *  use addInetTransport to avoid the overhead of the
   *  dynamic cast
   */
  void addService (Service *const service);

  /** Remove a service from the map of active transports.
   *
   *  \param service a pointer to the service to remove
   *
   *  \note since this ServiceManager is dedicated to the use
   *  with InetTransports, arguments to this function will
   *  get tried to dynamic cast them to (InetTransport *).
   *  if that fails, no action will be performed.
   *  use addInetTransport to avoid the overhead of the
   *  dynamic cast
   */
  void delService (Service *const service);

  /** get pointers to transports in a vector
   *
   *  \param transports the transport vector to fill with the pointers
   *
   *  \note the pointers to the transports will be simply
   *  pushed back to the vector.  to have to ensure
   *  that it is empty before calling getTransports() by yourself
   */
  void getTransports (std::vector<InetTransport *>& transports) const;


protected:
  /** add handle entry to _pollfd
   *
   *  \param sd socket descriptor to add
   *
   *  \return true on success, false on failure
   */
  bool addHandleToPollfd (const int handle);

  /** delete handle entry from _pollfd
   *
   *  \param sd socket descriptor to delete
   *
   *  \return true on success, false on failure
   */
  bool delHandleFromPollfd (const int handle);

  /** show state of this object (for debugging)
   */
  void showState ();

  /** show contents of _pollfd struct array (for debugging)
   */
  void validatePollfd ();


private:
  /// TransportMap type definition
  typedef std::map < int, InetTransport *, std::less<int> > InetTransportMap;

  /// The map in which the transports are held.
  InetTransportMap _itrans;

  /// pollfd struct array to hold the socket descriptors for poll
  struct pollfd *_pollfd;

  /// size of pollfd struct array (number of services being served)
  int _pollnum;

};

#endif // #ifndef LIBOSC_INETTRANSPORTMANAGER_H
