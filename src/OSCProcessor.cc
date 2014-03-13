/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002-2004 libOSC++ contributors. See AUTHORS                */
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


/* this class was modified by Martin Rumori (martin@rumori.de) */

/**
 * @file OSCProcessor.cc
 *
 * Implementation.
 * Accepts OSC Packets and sends them to the Namespace.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#include "OSCProcessor.h"

using namespace std;

OSCProcessor::OSCProcessor ()
{

}


OSCProcessor::~OSCProcessor ()
{

}


bool OSCProcessor::process(const std::string& packet, Transmit *const reply)
{
    OSCUnpacker unpacker;
    std::string address;

    unpacker.setData(packet); // init unpacker
    unpacker.unpackString(&address); // get address

    // check for bundle
    if (address == "#bundle")
    {
        // support timetags!
        // replace this block with your favorite priority queue.

#ifdef LIBOSC_DEBUG
        cerr << "OSCProcessor::process(): processing bundle" << endl;
#endif

        unpacker.skipHyper(); // skip timetag

        std::string bundleElem;

        while (unpacker.unpackBlob(&bundleElem))
        {
            // recursively process bundleElem
            process(bundleElem, reply);
        }
    }
    else
    { // no bundle, but message
        // now get our callable object from the namespace
        if (myNamespace)
        {
            myNamespace->dispatch(address, packet, reply);
        }
        else
        {
#ifdef LIBOSC_DEBUG
            cerr << "OSCProcessor::process(): no namespace set to this processor" << endl;
#endif
        }
    }

    return true;
}


void OSCProcessor::setNamespace (OSCNamespace *n)
{
  myNamespace = n;
}


OSCNamespace* OSCProcessor::getNamespace (void)
{
  return myNamespace;
}

// EOF
