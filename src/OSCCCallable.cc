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
 * @file OSCCCallable.c
 * 
 * Implements the functionality defined in OSCCCallable.h
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 *
 * @bug Uses old OSC code to skip ahead in the string.
 */

#include "OSCCCallable.h"

using namespace std;

void OSCCCallable::call(const std::string& data, Transmit *const reply)
{
  const char  *endArguments;
  const char  *arguments;
  int          argumentLength;
  void        *context;
  int64_t      when;

  methodCallback function;
  
  when = 1; // NOW
  
  function  = this -> getFunction();

  if(function != NULL)
  {
    context   = this -> getContext();  
    arguments = data.data();

    argumentLength  = data.size();
    endArguments    = arguments + argumentLength;

    // must skip two strings to get to meat of arguments?
    arguments      = OSCDataAfterAlignedString(arguments, endArguments, NULL);
    argumentLength = endArguments - arguments;
    (*function)(context, argumentLength, arguments, when, (NetworkReturnAddressPtr)NULL);
  }
  else
  {
    cerr << "NULL function pointer in OSCCCallable::call" << endl;
  }
}
