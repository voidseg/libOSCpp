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
 * @class OSCUnpacker
 *
 * Deserialize OSC data. Does not automatically process type tags.
 *
 * This class unpacks four-byte aligned binary
 * data packed in the Open SoundControl format.
 *
 * Here is a simple example.  An OSC packet has been given to us
 * in @c oscdata and we would like to unpack it.
 *
 * If we want to be safe, we should check that our typetags are 
 * correct (here, ",if") and that all of the unpack methods return true.
 * (The object into which the data is unpacked will never be invalid
 * after an unsuccessful unpack - if there is some error the result
 * is 0 or an empty string.  However, if we can't unpack the arguments
 * we expected, we should probably discard the message.)
 *
 * @code
 * bool   success;
 * string address;
 * string typetags;
 * int    i;
 * float  f;
 * OSCUnpacker *p;
 * 
 * p = new OSCUnpacker();
 * 
 * p->setData(oscdata);
 * success  = p->skipString();  // we pass over the first string, the address
 * success &= p->unpackString(&typetags);
 *
 * if(success)  // we have unpacked the typetags
 * {
 *   success = false;
 *   if(typetags == ",if")
 *   {
 *     success  = p->unpackInt(&i);
 *     success &= p->unpackFloat(&f);
 *   }
 * }
 *
 * // if success == true, we have correctly unpacked the message.
 * 
 * delete p;
 * @endcode
 *
 * @see OSCPacker
 *
 * @author  Daniel Holth
 * @author  Martin Rumori
 */

#ifndef OSCUNPACKER_H
#define OSCUNPACKER_H

#include "int64_t.h"

#ifdef __MACH__
#include <sys/types.h>
#include <machine/endian.h>
#else
#include <netinet/in.h>
#endif

#include <string>
#include <string.h>
#include <utility>

#include <iostream>


class OSCUnpacker
{
  private:
    std::string    myData;
    int       myPosition;

  protected:
    std::string *getDataPointer()  { return &(this->myData); }
    /// Copy @a bytes octets of data into @a *out, padding to keep alignment.
    bool unpackBytes(char *out, int bytes);
    /// Skip @a bytes octets of data, adding padding to keep alignment.
    bool skipBytes(int bytes);
    int  getPosition()        { return this->myPosition; }
    void setPosition(int p)   { this->myPosition = p;    }
    
  public:
    OSCUnpacker();
    
    void init()   { this->reset(); }
    void reset()  { this->setPosition(0); }
    
    /// Copy a double-precision floating point number into @a *out.
    bool unpackDouble (double  *out);
    /// Copy a single-precision floating point number into @a *out.
    bool unpackFloat  (float   *out);
    /// Copy an eight-byte integer into @a *out.
    bool unpackHyper  (int64_t *out);
    /// Copy a four-byte integer into @a *out.
    bool unpackInt    (int32_t *out);
    
    /// Copy a null-terminated string into *out.
    bool unpackString (std::string  *out);
    /// Copy a counted string (blob) into *out.
    bool unpackBlob   (std::string  *out);

    // These functions skip data.
    // For example: If you skip an int and the next argument
    // is a string, you will be positioned to read the string.
    
    /// Skip eight bytes of data.
    bool skipDouble();
    /// Skip four bytes of data.
    bool skipFloat();
    /// Skip eight bytes of data.
    bool skipHyper();
    /// Skip four bytes of data
    bool skipInt();
    /// Skip a null-terminated string in the data.
    bool skipString();
    /// Skip a blob (counted string) in the data.
    bool skipBlob();

    void setData(const std::string& data);
};

#endif
