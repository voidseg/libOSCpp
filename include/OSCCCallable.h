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
 * @class OSCCCallable
 *
 * Takes an 'old' OSC C function and calls it.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 */

#ifndef OSC_C_CALLABLE_H
#define OSC_C_CALLABLE_H

#include "OSCCallable.h"
#include "Transmit.h"
#include <string>

#include "int64_t.h"

#include "OSC-c-types.h"
#include "OSC-string-help.h"


class OSCCCallable : public OSCCallable
{

  private:
    methodCallback myFunction;
    void *myContext;
  protected:
    methodCallback getFunction() { return this->myFunction; }
    void *getContext()  { return this->myContext; }
  public:
    OSCCCallable() { this->setFunction(NULL, NULL); }
    
    /**
     * Sets the function.
     *
     * @param function Pointer to an OSC-Kit classic function
     * @pre   None
     * @post  Function will be called when this->call() is called.
     */
    void setFunction(methodCallback function, void *context) 
    {
      this->myFunction = function; 
      this->myContext  = context;
    }
    
    virtual void call(const std::string& data, Transmit *const reply);
};

#endif

