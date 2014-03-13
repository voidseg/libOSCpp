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
 * @class OSCCallable
 *
 * The superclass of all objects which reside in the OSCNamespace.
 * 
 * OSCCallable objects process OSC messages.  For example, the simple
 * OSCCallable OSCPrintCallable simply prints data sent to it.  More 
 * complex OSCCallables might command sound synthesis.
 *
 * This class should be extended whenever you wish to do something
 * new with OSC messages.
 * 
 * All OSCCallable objects have a type signature that represents the
 * kinds of arguments that the OSCCallable expects, and a documentation
 * string that describes what the OSCCallable does.  These tell users
 * of the server how to use the objects in the OSCNamespace.
 *
 * The transmit object is available so your callable can send a message
 * in reply to the message it's just received. Its creator manages it, so
 * if your callable must reply again *after* return it must clone Transmit*.
 * 
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#ifndef OSC_CALLABLE_H
#define OSC_CALLABLE_H

#include <iostream>
#include <string>
#include "Transmit.h"

class OSCCallable
{
  protected:
    std::string myDocumentation;
    std::string myTypeSignature;

  public:
    virtual ~OSCCallable();
    
    /// Process @a data, then use @a reply to send a response.
    virtual void call(const std::string& data, Transmit *const reply) = 0;

    // may soon include one with a separate address argument
    // virtual void call(const std::string& data, Transmit *const reply) = 0;
    
    virtual std::string getDocumentation() 
    { 
      return this->myDocumentation; 
    }
    
    virtual void setDocumentation(std::string documentation)
    {
      this->myDocumentation = documentation;
    }

    virtual std::string getTypeSignature()
    {
      return this->myTypeSignature;
    }

    virtual void setTypeSignature(std::string typeSignature)
    {
      this->myTypeSignature = typeSignature;
    }

  /**
   * \return a string which identifies this object (incl. type).
   */
  virtual std::string getName ();

};

/// an aberration. the more bugs we find, the more we need these for all classes
std::ostream &operator<<(std::ostream &, OSCCallable &);

#endif
