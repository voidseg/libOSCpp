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
 * @class OSCLister
 *
 * Replies to requests with a list of the addresses in the namespace.
 */

#ifndef OSCLISTER_H
#define OSCLISTER_H

#include <iostream>

#include "OSCNamespace.h"
#include "OSCCallable.h"
#include "OSCPacker.h"
#include "OSCUnpacker.h"

class OSCLister : public OSCCallable
{
  private:
    OSCNamespace *mySpace;
  protected:
    OSCNamespace* getNamespace()      { return this->mySpace; }
  public:
    OSCLister()          { }
    virtual ~OSCLister() { }

    void setNamespace(OSCNamespace *space)  { this->mySpace = space; }

    void call(const std::string& data, Transmit *const replier);
};

#endif
