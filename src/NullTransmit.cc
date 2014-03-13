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
 * @file NullTransmit.cc
 *
 * Implementation of the interface in NullTransmit.h
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#include "NullTransmit.h"

#include <stdio.h>
#include <string>
#include <sstream>

bool NullTransmit::transmit(const std::string& data)
{
  std::string::const_iterator iterator;
  iterator = data.begin();

#ifdef LIBOSC_DEBUG
  const int hexwidth = 8;
  int count;
  count     = data.size();

  for(int i=0; i<count; i++)
  {
	if(count % hexwidth == 0)
	{
	  printf("\n");
	}
	printf("%02x ", (int)*iterator);
	iterator++;
  }
#endif
  return true;
}

std::string NullTransmit::getName() {
	std::ostringstream name;
	name << "NullTransmit@" << this;
	return name.str();
}
