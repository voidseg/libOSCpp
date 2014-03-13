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
 * @class OSCService
 *
 * Accepts OSC Packets and sends them to the Namespace.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#ifndef LIBOSC_OSCPROCESSOR_H
#define LIBOSC_OSCPROCESSOR_H

#include <iostream>

#include "Processor.h"
#include "OSCNamespace.h"
#include "OSCCallable.h"
#include "OSCUnpacker.h"


class OSCProcessor : public Processor
{
  public:
  /// constructor
    OSCProcessor();

    /// destructor
    virtual ~OSCProcessor();

    /** Process an OSC packet.
     *
     *  The address of the message will get unpacked and the
     *  corresponding callable from the namespace will get
     *  called.
     *  OSC bundles are processed recursively.
     *
     *  \bugs no timetag (sched) support.
     *
     *  \param s a C++ string containing the OSC packet
     *
     *  \return true on success, false on failure
     */
    virtual bool process (const std::string& s, Transmit *const reply);
    

    /** Set the namespace for this OSC processor.
     *
     *  The namespace will be used to lookup the corresponding
     *  callable on arrival of an incoming message.
     *
     *  \param n a pointer to the namespace to set for this processor.
     */
    void setNamespace (OSCNamespace *n);


    /** Get a pointer to the namespace associated with this processor.
     *
     *  \return a pointer to the namespace set to this processor.
     */
    OSCNamespace* getNamespace (void);


  protected:    


  private:
    OSCNamespace *myNamespace;
};

#endif // #ifndef LIBOSC_OSCPROCESSOR_H

// EOF
