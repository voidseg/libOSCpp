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
 * @class OSCCountCallable
 *
 * A Callable that prints (debug) messages.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 */

#ifndef OSC_COUNT_CALLABLE_H
#define OSC_COUNT_CALLABLE_H

#include "OSCCallable.h"
#include "OSCUnpacker.h"

class OSCCountCallable : public OSCCallable
{
  private:
    string message;
    int highest;
    int missed;
    OSCUnpacker *unpacker;
    
  public:
    OSCCountCallable(string message) { this->message = message; 
                                       this->highest = 0;
                                       this->missed  = 0; 
                                       this->unpacker = new OSCUnpacker(); }
    virtual ~OSCCountCallable()      { }
    
    virtual void call(const std::string& data, Transmit *const reply) 
    { 
      string address;
      string typetags;
      int report;
      
      this->unpacker->setData(data);
      this->unpacker->unpackString(&address);
      this->unpacker->unpackString(&typetags);
      this->unpacker->unpackInt(&report);

      if(report != this->highest + 1)
      {
        cout << this->message << " DISCREPANCY " 
          << (report - this->highest) << "\t" 
          << this->missed << endl;

        this->missed++;
      }
      
      if(report > this->highest) this->highest = report;
    }
};

#endif
