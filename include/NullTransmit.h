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
 * @class NullTransmit
 *
 * A test transmitter for debugging.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#ifndef NULL_TRANSMIT_H
#define NULL_TRANSMIT_H

#include "Transmit.h"

#include <string>

class NullTransmit : public Transmit
{
  public:
    virtual ~NullTransmit() {};
    /** Print a hex dump of @a data if DEBUG was defined during
     * compilation.  Otherwise, do nothing. Always return true.
     * @param data The OSC message to be printed
     */
    virtual bool transmit(const std::string& data);
    
	std::string getName();
};

#endif
