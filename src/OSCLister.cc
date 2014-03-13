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
 * @file OSCLister.cc
 *
 * Implementation of OSCLister.h
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#include "OSCLister.h"

using namespace std;

void OSCLister::call(const std::string& data, Transmit* replier) 
{
  string queryAddress;
  string docstring;
  string reply;
  string typetags;
  vector<string> addresses;

  int size;

  OSCNamespace  *space;
  // OSCCallable   *queryItem;
  OSCUnpacker u;
  OSCPacker   p;

  u.setData(data);
  u.unpackString(&queryAddress);
  u.unpackString(&queryAddress);

  //    May support listing a subset of the namespace in the future:   
  //      u.unpackString(&queryAddress);

  space     = this->getNamespace();

  // Test the list method
  addresses = space->list("");
  size = addresses.size();

#ifdef LIBOSC_DEBUG
  cout << "Listing query: ";
  cout << "Reporting " << size << " addresses." << endl;
#endif

  typetags = ",";
  typetags.append(size, 's');

  p.packString("/listing");
  p.packString(&typetags);

  for(int i=0; i<size; i++)
  {
    p.packString(&addresses[i]);
  }

  reply = p.getData();
  replier->transmit(reply);
};
