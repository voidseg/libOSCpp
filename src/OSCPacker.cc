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
 * @file  OSCPacker.cc
 *
 * Implementation of the interface in
 * OSCPacker.h
 *
 * @author  Daniel Holth
 */

#include "OSCPacker.h"
#include "int64_t.h"
#include "hyperswap.h"

#ifndef assert
#define assert(x) 
#endif

using namespace std;

bool OSCPacker::appendFourBytes(char *bytes)
{
  return this->appendBytes(bytes, 4);
}

bool OSCPacker::appendEightBytes(char *bytes)
{
  return this->appendBytes(bytes, 8);
}

bool OSCPacker::appendBytes(char *bytes, int count)
{
  string *data;

  data = this->getDataPointer();
  (*data).append(bytes, count);

  return true;
}


bool OSCPacker::packInt(int32_t in)
{
  bool status;
  in = htonl(in);
  status = this->appendFourBytes((char*)&in);
  return status;
}


bool OSCPacker::packHyper(int64_t in)
{
  bool status;

  in = htonll(in);      // this is a libOSC++ function

  status = this->appendEightBytes((char*)&in);
  return status;
}


bool OSCPacker::packFloat(float in)
{
  bool status;
  int32_t inf = *((int*) &in);
  inf = htonl(inf);
  assert(sizeof(float) == 4);
  status = this->appendFourBytes((char*)&inf);
  return status;
}


bool OSCPacker::packDouble(double in)
{
  bool status;
  status = this->appendEightBytes((char*)&in);
  return status;
}


bool OSCPacker::packString(const string *in)
{
  return OSCPacker::packString((*in).c_str());
}


bool OSCPacker::packString(const char *in)
{
  string *data;

  data = this->getDataPointer();
  (*data).append(in);

  this->padString(data);

  return true;
}


// Untested!
bool OSCPacker::packBlob(const char *in, int bytes)
{
  bool success;

  success = false;
  
  if(bytes >= 0)
  {
    success = this->packInt(bytes);
    if(success)
    {
      success = this->packBytes(in, bytes);
    }
  }

  return success;
}


bool OSCPacker::packBytes(const char *in, int bytes)
{
  int length;
  int pad;
  string *data;

  data = this->getDataPointer();
  (*data).append(in, bytes);

  length = (*data).size();
  pad = (4 - (length % 4)) % 4;
  (*data).append(pad, '\0');

  return false;
}


void OSCPacker::padString(string *data)
{
  int length;
  int pad;

  (*data).append(1, '\0');
  length = (*data).size();
  pad = (4 - (length % 4)) % 4;
  (*data).append(pad, '\0');
}


string OSCPacker::getData()
{
  return this->myData;
}


void OSCPacker::reset()
{
  myData = "";
}
