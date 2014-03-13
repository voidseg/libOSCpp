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
 * @class OSCTypeReturner
 *
 * Returns the type signature
 *
 * Sends a reply with the type signature
 * of the queried object as well as the 
 * address of the queried object to the 
 * requester of the type signature.
 */

#ifndef OSCTYPE_RETURNER_H
#define OSCTYPE_RETURNER_H

#include "OSCPacker.h"
#include "OSCUnpacker.h"
#include "OSCNamespace.h"
#include "OSCCallable.h"

using namespace std;

class OSCTypeReturner : public OSCCallable
{
  private:
    OSCNamespace *mySpace;
  protected:
    OSCNamespace* getNamespace()      { return this->mySpace; }
  public:
    OSCTypeReturner()          { }
    virtual ~OSCTypeReturner() { }

    void setNamespace(OSCNamespace *space)  { this->mySpace = space; }
    
    void call(const std::string& data, Transmit *const replier) 
    {
      std::string queryAddress;
      std::string typeSignature;
      std::string reply;
      OSCNamespace  *space;
      OSCCallable   *queryItem;
      OSCUnpacker u;
      OSCPacker   p;

      u.setData(data);
      u.unpackString(&queryAddress);
      u.unpackString(&queryAddress);
      u.unpackString(&queryAddress);

      cout << "Type Signature request on: " << queryAddress << endl;  // DEBUG
      space     = this->getNamespace();
      queryItem = space->find(queryAddress);

      if(queryItem != NULL)
      {
        typeSignature = queryItem->getTypeSignature();
        p.packString("/typeSig");
        p.packString(",ss");
        p.packString(queryAddress.c_str());
        p.packString(typeSignature.c_str());
      }
      else
      {
        // Is this the best approach for an error? (may confuse the recipient)
        p.packString("/typeSig");
        p.packString(",s");
        p.packString("item not found");
      }

      reply = p.getData();
      replier->transmit(reply);
    };
};

#endif
