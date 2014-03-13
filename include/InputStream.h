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
 * @class InputStream
 *
 * A Callable to forward data via its Transmit object.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 */

#ifndef OSC_INPUTSTREAM_H
#define OSC_INPUTSTREAM_H

#include "OSCCallable.h"
#include "Transmit.h"

#ifdef LIBOSC_DEBUG
#include <iostream>
#endif

class InputStream : public OSCCallable
{
 private:
  Transmit* myTransmit;
 public:
  InputStream()                   { this -> myTransmit = NULL; }
  InputStream(Transmit* transmit) { this -> setTransmit(transmit); }
  
  virtual ~InputStream()          { delete myTransmit; }
  
  virtual void call(const std::string& data, Transmit *const reply) 
    {
#ifdef LIBOSC_DEBUG
      std::cout << "InputStream::call: " << this->getName() << " sending to " 
		<< myTransmit->getName() << std::endl;
#endif
      myTransmit -> transmit(data);
    }

  void setTransmit(Transmit* transmit)  { this -> myTransmit = transmit; }
  Transmit* getTransmit()               { return this -> myTransmit; }
};

#endif
