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

#ifndef LIBOSC_INETOSC_H
#define LIBOSC_INETOSC_H

#ifdef    __CYGWIN__
#include <w32api/winsock2.h>
#include <w32api/ws2tcpip.h>
#include <poll.h>
#include <unistd.h>

#else  // __CYGWIN__
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#endif // __CYGWIN__

#include <fcntl.h>

#include <iostream>
#include <string>

// defines

// The maximum datagram length for udp packets
#define OSC_DGRAM_MAXLENGTH 65536

// The maximum length for TCP OSC messages
#define OSC_STREAM_MAXLENGTH 65536

// The maximum number of pending TCP connections
#define OSC_STREAM_MAXCONN 12


/**
 * @class InetOSC
 *
 *  Abstract OSC over the IP protocol.
 *
 *  @author Martin Rumori
 */
class InetOSC
{

public:
  /// constructor
  InetOSC ();

  /// destructor
  virtual ~InetOSC ();

  /// address family type
  typedef enum
    {
      /// internet protocol version 4
      OSC_IN_IPv4,
      /// internet protocol version 6
      OSC_IN_IPv6
    } InetOSCAddrType;

  /// protocol type
  typedef enum
    {
      /// user datagram protocol
      OSC_IN_UDP,
      /// transmission control protocol
      OSC_IN_TCP
    } InetOSCProto;

  /** OSC Inet errors.
   *
   *  This enum is used to sign an error occurred
   *  in a OSCInet object
   *
   *  An error usually is signed by a 'false' return value of
   *  methods in the OSCInet objects.
   *  Use getError() in these objects to get the error code.
   *
   *  \note Error codes ending on _ERRNO (and >= 0xC000) sign that an
   *  error occured during a system call and that the errno (or
   *  h_errno in case of OSC_IN_GETHOST_ERRNO) was saved for further
   *  processing.  You have to use getLastErrno() to get these saved
   *  value and process it by your own, if you need detailed error
   *  codes.
   *
   *  \see InetTCPMaster, InetTCPSlave, InetUDPMaster
   */
  typedef enum
    {
      /// no error, succeeded
      OSC_IN_SUCCESS = 0x0000,
      /// unspecified error
      OSC_IN_UNKNOWN = 0x0001,
      /// object is not idle
      OSC_IN_NOT_IDLE = 0x0002,
      /// operation would block
      OSC_IN_WOULDBLOCK = 0x0003,
      /// _maxHandleCycles reached before EAGAIN (no error)
      OSC_IN_MAXCYCLES = 0x0004,
      /// object is not longer connected
      OSC_IN_CLOSED_CONN = 0x0005,
      /// given protocol unknown
      OSC_IN_PROTO_UNKNOWN = 0x0100,
      /// given address type unknown
      OSC_IN_ADDR_UNKNOWN = 0x0101,
      /// no processor set (e. g. in InetTCPMaster)
      OSC_IN_NO_PROCESSOR = 0x0200,
      /// no transport manager set (e.g. in InetTCPMaster)
      OSC_IN_NO_TRANSMGR = 0x0201,
      /// no tcp master set (e.g. in InetTCPSlave)
      OSC_IN_NO_TCPMASTER = 0x0202,
      /// slave is already inserted (InetTCPMaster)
      OSC_IN_ALREADY_SLAVE = 0x0203,
      /// slave object not found (InetTCPMaster)
      OSC_IN_NOT_SLAVE =  0x0204,
      /// error while gethostbyname()
      OSC_IN_GETHOST_ERRNO = 0xC000,
      /// error while getprotobyname()
      OSC_IN_GETPROTO_ERRNO = 0xC001,
      /// error while socket()
      OSC_IN_SOCKET_ERRNO = 0xC002,
      /// error while setsockopt()
      OSC_IN_SETSOCKOPT_ERRNO = 0xC003,
      /// error while fcntl()
      OSC_IN_FCNTL_ERRNO = 0xC004,
      /// error while bind()
      OSC_IN_BIND_ERRNO = 0xC005,
      /// error while listen()
      OSC_IN_LISTEN_ERRNO = 0xC006,
      /// error while connect()
      OSC_IN_CONNECT_ERRNO = 0xC007,
      /// error while shutdown()
      OSC_IN_SHUTDOWN_ERRNO = 0xC008,
      /// error while close()
      OSC_IN_CLOSE_ERRNO = 0xC009,
      /// error while recv() or recvfrom()
      OSC_IN_RECV_ERRNO = 0xC00A,
      /// error while send() or sendto()
      OSC_IN_SEND_ERRNO = 0xC00B,
      /// error while poll()
      OSC_IN_POLL_ERRNO = 0xC00C,
      /// error while accept()
      OSC_IN_ACCEPT_ERRNO = 0xC00D,
      /// error while malloc()
      OSC_IN_MALLOC_ERRNO = 0xC00E,
    } InetOSCError;

  /** get last error
   *
   *  \return the code of the last error occured
   */
  const InetOSCError getError () const;

  /** get last errno code
   *
   *  \return the last errno returned by a system call
   */
  const int getLastErrno () const;

  /** get a string description of an InetOSCError
   *
   *  \param err the error to get the string for
   *  \param errString a reference to a string to fill with the description
   *
   *  \return true on success, false on unknown error code given
   */
  static const bool getErrorString (const InetOSCError err,
				    std::string& errString);

  /** get address type of this object
   *
   *  \return the address type of this object
   */
  const InetOSCAddrType getAddrType (void) const;

  /** set maximum number of cycles in handle()
   *
   *  this method determines the maximum number of cycles
   *  to read within a handle() call.  this is to avoid a
   *  "block" of the system if a handle() call never comes back
   *  on never-ending receiving of messages.  handle() returns
   *  in any case if recv got EAGAIN.
   *
   *  for InetUDPMaster this settings correlates directly to
   *  the maximum number of received OSC packets within one
   *  one handle() call, for InetTCPSlave this number means the
   *  the number of receive buffers filled within one handle() call,
   *  which may contain more or less OSC packets.  for InetTCPMaster,
   *  this parameter means the maximum number of pending connections
   *  getting accepted within one handle() call.
   *
   *  \param cycles number of cycles.
   *  a negative value means unlimited (read always until EAGAIN),
   *  a value of zero means no handle() cycles (receiving disabled)
   */
  void setMaxHandleCycles (const int cycles = -1);

  /** get maximum number of cycles in handle() setting
   *
   *  \see setMaxHandleCycles()
   *
   *  \return the maximum number of handle() cycles set to this object
   */
  const int getMaxHandleCycles (void) const;


protected :

  /** create socket
   *
   *  getprotobyname, socket ...
   *
   *  \param sd a pointer to the socket descriptor
   *  \param proto the protocol to use for the socket
   *  \param addrType the address type to use for the socket
   *
   *  \return true on success, false on failure
   */
  bool createSocket (int *const sd,
		     const InetOSCProto proto,
		     const InetOSCAddrType addrType);

  /** bind socket to any local address
   *
   *  \param sd a pointer to the socket descriptor
   *  \param port the port in network byteorder to bind the socket to
   *  \param addrType the address type of the socket
   *
   *  \return true on success, false on failure
   */
  bool bindSocketToAny (int *const sd,
			const short port,
			const InetOSCAddrType addrType);

  /** close socket
   *
   *  \param sd a pointer to the socket descriptor
   *  \param setError false: do not set _error variable
   *  this might be useful in the case where you
   *  close a socket as reaction on a previous error
   *  and you don't want to overwrite the "real" error
   *
   *  \return true on success, false on failure
   */
  bool closeSocket (int *const sd, const bool setError = true);

  /** allocate memory for socket address structs
   *
   *  allocates a struct sockaddr_inX according
   *  to the given address type
   *
   *  \param addrType the address type to use
   *  \param addr pointer to to allocate
   *  \param addrlen pointer where the address length gets filled in
   *
   *  \return true on success, false on failure
   */
  bool allocSockAddr (const InetOSCAddrType addrType,
		      struct sockaddr * *const addr,
		      socklen_t *const addrlen);

  /** resolve hostname and set remote address struct
   *
   * 
   *  \param host the hostname or address to set
   *  \param port the port number in network byteorder to set
   *  \param addrType the address type to use
   *  \param addr pointer to the (allocated) address structure to fill
   *
   *  \return true on success, false on failure
   */
  bool setRemoteAddr (const std::string& host,
		      const short port,
		      const InetOSCAddrType addrType,
		      struct sockaddr *const addr);

  /** This is here to support a hack which supports the Max wireTAP stack.
   *
   * The Max OSC objects won't let you send and receive UDP from the
   * same socke t * (thus port): so you must receive OSC on a
   * different port than you send on.  * But the default behavior when
   * someone sends a subscribe message is to * subscribe the host and
   * port on which the request was sent (so the reply wi ll * go back
   * to the same place.) The hack lets Max people send an integer
   * value * in the subscribe message, which will be used as the port
   * to send future * traffic back to, on the same address. This
   * requires the ability to reset t he * port on a Transmit. But only
   * really a UDPTransmit - except those don't exi st * anymore,
   * they're InetUDPMasters now.
   *
   * \param port the port number in network byteorder to set
   * \param addrType the address type to use
   * \param addr pointer to the (allocated) address structure to change
   * \return true if ok, false if error
   */

  bool setRemotePort (const short port,
		      const InetOSCAddrType addrType,
		      struct sockaddr *const addr);

  /** get character string notation out of a network address structure
   *
   *  \param addrType the address type to interpret
   *  \param addr pointer to the sockaddr struct to extract the address from
   *
   *  \return a string containing the character string notation of the address
   *  or empty string ("") on error
   */
  std::string getHostAddress (const InetOSCAddrType addrType,
			 const struct sockaddr *const addr);

  /** reverse lookup hostname out of a network address structure
   *
   *  \param addrType the adress type to interpret
   *  \param addr pointer to the sockadddr struct to extract the address from
   *
   *  \return the hostname looked up or "" if not found
   *
   *  \note this method currently only works with IPv4
   */
  std::string getHostName (const InetOSCAddrType addrType,
		      const struct sockaddr *const addr);

  /** get port number out of a network address structure
   *
   *  \param addrType the adress type to interpret
   *  \param addr pointer to the sockadddr struct to extract the port from
   *
   *  \return the port number entry (network byteorder)
   *  from the address structure or 0 on error
   */
  short getPortNumber (const InetOSCAddrType addrType,
		       const struct sockaddr *const addr);

  /** set a socket descriptor non blocking
   *
   *  \param sd pointer to the socket descriptor
   *
   *  \return true on sucess, false on failure
   */
  bool setSocketNonBlock (int *const sd);

  /** set SO_REUSEADDR option of a socket descriptor
   *
   *  \param sd pointer to the socket descriptor
   *  \param reuse true: set SO_REUSEADDR, false: unset SO_REUSEADDR
   *
   *  \return true on success, false on failure
   */
  bool setSocketReuseAddr (int *const sd, bool reuse = true);


  /* protected members */

  /// address type of this object
  InetOSCAddrType _addrType;

  /// error code of this object
  InetOSCError _error;

  /// last errno returned by system call
  int _lasterrno;

  /// socket descriptor
  int _socket;

  /// maximum handle() cycles
  int _maxHandleCycles;

};

#endif // #ifndef LIBOSC_INETOSC_H
