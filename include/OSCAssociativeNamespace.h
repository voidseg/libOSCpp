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
 * @class OSCAssociativeNamespace
 *
 * A simple implementation of an OSC namespace
 *
 * This implementation of the OSCNamespace uses a flat mapping
 * between complete addresses and OSCCallables (callbacks).
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#ifndef OSC_ASSOCIATIVE_NAMESPACE_H
#define OSC_ASSOCIATIVE_NAMESPACE_H

#include <map>
#include <string>
#include "OSCNamespace.h"

class OSCAssociativeNamespace : public OSCNamespace
{
  protected:
    typedef std::map < std::string, OSCCallable*, std::less<std::string> > CallableMap;

  private:
    CallableMap* myCallables;
    
  protected:
    CallableMap* getCallables() { return this->myCallables; }
    void setCallables(CallableMap *cm) { this->myCallables = cm; }

  public:
    OSCAssociativeNamespace();
    virtual ~OSCAssociativeNamespace();

    virtual bool add(const std::string &address, OSCCallable *thingamabob);
    virtual bool remove(const std::string& address);
    virtual OSCCallable* find(const std::string& address);

    // perhaps we will replace the second (and third?) argument with an OSCMessage object...
    virtual bool dispatch(const std::string &address, const std::string& data, Transmit *const reply);

    /** @bug @a basename is ignored - it returns all addresses,
     * not just the ones that start with @a basename.
     */
    virtual std::vector<std::string> list(const std::string& basename);
};

#endif
