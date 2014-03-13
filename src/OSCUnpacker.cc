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


/* this class was modified by Martin Rumori (martin@rumori.de) */

/**
 * @file OSCUnpacker.cc
 *
 * Implements the interface described in OSCUnpacker.h
 *
 * @author Daniel Holth
 * @author Martin Rumori
 */

/*
 * Test cases:
 *
 * Blob (typetag 'b'):
 * - zero-length blob
 * - multiple-of-4-byte blob on end and middle of packet
 * - blobs with invalid sizes (<0, >packetlength)
 *
 * All:
 * - Attempt to unpack when at the end of data, should return false
 *
 * Integers:
 * - Make sure bytes are swapped
 */

#include "OSCUnpacker.h"
#include "hyperswap.h"

using namespace std;

#define OSC_INT_SIZE    4
#define OSC_FLOAT_SIZE  4
#define OSC_DOUBLE_SIZE 8
#define OSC_HYPER_SIZE  8

OSCUnpacker::OSCUnpacker()
{
  this->setPosition(0);
}


void OSCUnpacker::setData(const string& data)
{
  reset();
  myData = data;
}


bool OSCUnpacker::skipBytes(int bytes)
{
  bool success;

  int pad;
  int position;
  int end;
  string *data;
  
  success   = false;
  position  = this->getPosition();
  data      = this->getDataPointer();
  end       = (*data).size();

  if(position < end)
  {
    if(bytes >= 0)
    {
      position = position + bytes;
      pad = (4 - (position % 4)) % 4;
      this->setPosition(position+pad);
      success = true;
    }
  }

  return success;
}


// This method is unsafe, therefore it is protected.
// For a method like this to be exposed to the user
// we would need a counted-string type in OSC.
// (On the other hand, we could tell the user that
// they are doing low level stuff and are on their
// own.  But is that the proper attitude for a
// product that is supposed to be easy to use?)
bool OSCUnpacker::unpackBytes(char *out, int bytes)
{
  bool success;

  int copied;
  int pad;
  int position;
  string *data;
  
  data      = this->getDataPointer();
  position  = this->getPosition();

  copied = (*data).copy(out, bytes, position);

  if(copied == bytes)
  {
    position = position + bytes;
    pad = (4 - (position % 4)) % 4;
    this->setPosition(position+pad);
    success = true;
  }
  else
  {
	// bzero deprecated: memset conforms to more standards
    // bzero(out, bytes);
	memset(out, 0, bytes);
    success = false;
  }

  return success;
}


bool OSCUnpacker::unpackDouble(double *out)
{
  bool success;
  
  success = this->unpackBytes((char*)out, OSC_DOUBLE_SIZE);
  if(!success)
  {
    *out = 0.0;
  }
  
  return success;
}


bool OSCUnpacker::unpackFloat(float *out)
{  
  bool success;
  int32_t *outf = (int32_t*)out;

  success = this->unpackBytes((char*)out, OSC_FLOAT_SIZE);
  *outf = (int32_t)ntohl(*outf);
  if(!success)
  {
    *out = 0.0;
  }
  
  return success;
}


bool OSCUnpacker::unpackHyper(int64_t *out)
{
  bool success;
  success = this->unpackBytes((char*)out, OSC_HYPER_SIZE);
  *out = ntohll(*out);  
  return success;
}


bool OSCUnpacker::unpackInt(int32_t *out)
{
  bool success;
  int32_t integer;

  success = this->unpackBytes((char*)&integer, OSC_INT_SIZE);
  if(success)
  {
    integer = ntohl(integer);
    *out = integer;
  }
  else
  {
    *out = 0;
  }
  
  return success;
}


bool OSCUnpacker::unpackString(string *out)
{
  bool success;
  
  int pad;
  int position;
  int endPosition;
  int bytes;
  string *data;

  success     = false;
  
  data        = this->getDataPointer();
  position    = this->getPosition();  
  endPosition = (*data).find('\0', position);

  if(endPosition != (int)(*data).npos)
  {
    bytes  = endPosition - position;

    if(out != NULL)
    {
        (*out).assign(*data, position, bytes);
    }

    position = position + bytes + 1;  // + 1 for the '\0'
    pad = (4 - (position % 4)) % 4;   // OSC's 4 byte alignment
    this->setPosition(position+pad);

    success = true;
  }

  return success;
}


bool OSCUnpacker::unpackBlob(string *out)
{
  bool success;

  int32_t bytes;
  int dataLength;
  int pad;
  int position;

  string *data;
 
  success = this->unpackInt(&bytes);

  if(success)
  {
    success = false;
    if(bytes >= 0)
    {
      data        = this->getDataPointer();
      position    = this->getPosition();

      dataLength  = (*data).size();

      if(bytes + position <= dataLength)
      {
        (*out).assign(*data, position, bytes);
        position = position + bytes;
        pad = (4 - (position % 4)) % 4;
        this->setPosition(position+pad);
        success = true;
      }
    }
  }

  return success;
}


bool OSCUnpacker::skipDouble()
{
  return this->skipBytes(OSC_DOUBLE_SIZE);
}


bool OSCUnpacker::skipFloat()
{
  return this->skipBytes(OSC_FLOAT_SIZE);
}


bool OSCUnpacker::skipHyper()
{
  return this->skipBytes(OSC_HYPER_SIZE);
}


bool OSCUnpacker::skipInt()
{
  return this->skipBytes(OSC_INT_SIZE);
}


bool OSCUnpacker::skipString()
{
    return this->unpackString((string*)NULL);
}


bool OSCUnpacker::skipBlob()
{
  bool success;
  int  bytes;
 
  success = this->unpackInt(&bytes);
  if(success)
  {
    success = this->skipBytes(bytes);
  }

  return success;
}
