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
 * @class OSCBundleHandler
 *
 * Dispatch the contents of a #bundle.
 *
 * This class is not generally used anymore. OSCProcessor
 * has #bundle handling logic now.
 *
 * Daniel Holth
 * 12 Nov. 2002
 */

#ifndef OSCBUNDLE_HANDLER_H
#define OSCBUNDLE_HANDLER_H

#include "OSCPacker.h"
#include "OSCUnpacker.h"
#include "OSCNamespace.h"
#include "OSCCallable.h"
#include "OSCProcessor.h"

class OSCBundleHandler : public OSCCallable
{
  private:
    OSCProcessor* myProcessor;
  protected:
    OSCProcessor* getProcessor()        { return this->myProcessor; }
  public:
    OSCBundleHandler()          { }
    virtual ~OSCBundleHandler() { }

    /// Set the processor
    /** Assign this to the normal processor that dispatches incoming messages. */
    void setProcessor(OSCProcessor *p)  { this->myProcessor = p; }
    
    void call(const std::string& data, Transmit *const replier) 
    {
      bool success;
      std::string queryAddress;
      std::string itemAddress;
      std::string itemData;
      OSCProcessor *processor;
      OSCUnpacker   u;
      OSCUnpacker   item;

      u.setData(data);

      // DEBUG
      u.unpackString(&queryAddress);               // Probably #bundle
      std::cout << "Bundle: " << queryAddress << std::endl;
      // END DEBUG

      u.skipHyper();                  // Ignore 8-byte time tag
      
      processor = this->getProcessor();

      success = true;

      while (success)
      {
        success = u.unpackBlob(&itemData);
        if(success)
        {
          // DEBUG
          item.setData(itemData);
          item.unpackString(&itemAddress);
          std::cout << "Bundle item: " << itemAddress << std::endl;
          // END DEBUG
          
          processor->process(itemData, replier);
        }
      }
    };
};

#endif
