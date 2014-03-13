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
 *  @file OSCAssociativeNamespace.cc
 *
 *  The implementation of the interface described
 *  in OSCAssociativeNamespace.h
 *
 *  This is a namespace that ignores the implicit
 *  hierarchy of an OSC namespace and just maps
 *  complete names to OSCCallable objects.
 *
 *  @author Clinton McChesney
 *  @author Daniel Holth
 *  @author Martin Rumori
 */

#include "OSCAssociativeNamespace.h"

OSCAssociativeNamespace::OSCAssociativeNamespace()
{
  this -> myCallables = new CallableMap();
}

OSCNamespace::~OSCNamespace() { }

OSCAssociativeNamespace::~OSCAssociativeNamespace()
{
  delete this -> getCallables();
}


bool OSCAssociativeNamespace::add(const std::string &address, 
		OSCCallable *thingamabob)
{
  CallableMap* callables;

  callables = this -> getCallables();
  (*callables)[address] = thingamabob;

  return true;
}


bool OSCAssociativeNamespace::remove(const std::string& address)
{
  CallableMap* callables;
  callables = this->getCallables();
  int removed;

  // CAVEAT: risk of memory leak
  // >>YOU<< have to delete the callable e. g. in your server
  removed = callables->erase(address);

  return (removed > 0);
}


OSCCallable* OSCAssociativeNamespace::find(const std::string& address)
{
  CallableMap* callables;
  callables = this->getCallables();
  CallableMap::iterator iter;

  if ((iter = callables->find(address)) == callables->end())
  {
      return 0; // not found or empty
  }
  else
  {
      return iter->second;
  }
}


bool OSCAssociativeNamespace::dispatch(const std::string &address, const std::string &packet, Transmit *const reply)
{
    OSCCallable *callable;
    bool success = false;

    callable = this->find(address);

    if (callable)
    {
#ifdef LIBOSC_DEBUG
        std::cout << "callable for " << address << " is " << (*callable) << std::endl;
#endif
        // and finally call the callable
        callable -> call(packet, reply);
        success = true;
    }
    else
    {
#ifdef LIBOSC_DEBUG
        std::cerr << "OSCAssociativeNamespace::dispatch(): no callable for address " << address << " found" << std::endl;
#endif
        success = false;
    }

    return success;
}


/// Bug:  doesn't check for basename, just returns it all.
std::vector<std::string> OSCAssociativeNamespace::list(const std::string& basename)
{
  std::vector<std::string> addresses;
  CallableMap *callables;
  CallableMap::iterator iter;
  CallableMap::iterator end;

  callables = this->getCallables();
  
  // size() for the map appears to be twice what it should be!
  // addresses.resize((*callables).size());
  
  iter = (*callables).begin();
  end  = (*callables).end();

  while(iter != end)
  {
    addresses.push_back((*iter).first);
    iter++;
  }
  
  return addresses;
}
