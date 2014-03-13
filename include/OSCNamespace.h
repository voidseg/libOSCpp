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
 * @class OSCNamespace
 *
 * Associate OSC addresses with OSC callbacks.
 *
 * Typical usage:
 * @code
 * void process(OSCNamespace *namespace, string address, Transmit *reply)
 * {
 *     namespace->dispatch(address, reply);
 * }
 * @endcode
 * 
 * @author Daniel Holth
 * @author Clinton McChesney
 */

#ifndef OSCNAMESPACE_H
#define OSCNAMESPACE_H

#include <string>
#include <vector>
#include "OSCCallable.h"

class OSCNamespace
{
  private:
  protected:
  public:
    virtual ~OSCNamespace(void) = 0;
    
    /// Add OSCCallable @a thingamabob to the namespace 
    /// with address @a address.
    virtual bool add(const std::string &address, OSCCallable *thingamabob) = 0;

    /// Remove the callable at address @a address.
    virtual bool remove(const std::string &address) = 0;
    
    /// Get a pointer to the first OSCCallable matching the given address,
    /// or NULL if not found.
    virtual OSCCallable* find(const std::string& address) = 0;

    /// Since one address can match multiple callbacks, use this.
    virtual bool dispatch(const std::string &address, const std::string& data, Transmit *const reply) = 0;
   
    /// Return a list of all addresses in the namespace beginning with
    /// the string @a base.
    virtual std::vector<std::string> list(const std::string& base)  = 0;
};

#endif
