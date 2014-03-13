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
 * @class Transmit
 *
 * An abstract way to send packets of data.
 *
 * The Transmit class abstracts away the particulars of a 
 * networking protocol, so that all that is left is a generic
 * way of sending a packet of data.
 *
 * This class is then subclassed to provide networking services
 * to the rest of the program. 
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#ifndef LIBOSC_TRANSMIT_H
#define LIBOSC_TRANSMIT_H

#include <string>

class Transmit
{
public:
  /// destructor
  virtual ~Transmit() { }

  /// Process packet @a data, typically sending it over the network.
  virtual bool transmit (const std::string& data) = 0;

  virtual std::string getName();

  /// The return port. See Transmit.cc for explanation.
  //    virtual void setReturnPort(int);
  //
  //  (i added this back in below: it relates to supporting Max. See
  //  InetOSC.h:300 or so for an explanation. and i found nothing in
  //  Transmit.cc in the way of an explanation -jared)

  /// Copies self.
  // better use a copy constructor
  // -- yes, but the trouble is we don't know the type of the Transmit we're
  // trying to clone. that's why this is here: to satisfy polymorphism.
  // See Broadcaster.cc:48.
  //
  virtual Transmit* clone() { return NULL; }

  /// Set the return port. Defined only for UDP. A hack to get Max to work with wireTAP.
  virtual bool setReturnPort(const short) { return false; }
};

#endif // #ifndef LIBOSC_TRANSMIT_H

