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

/**
 * @class OSCPacker
 *
 * Serialize data, OSC style. Manual typetags.
 *
 * This class packs binary data into a string,
 * where each piece of that data is aligned on a
 * four-byte boundary.
 *
 * @author  Daniel Holth
 */

#ifndef OSCPACKER_H
#define OSCPACKER_H

#include "int64_t.h"

#ifdef __MACH__
#include <sys/types.h>
#include <machine/endian.h>
#else
#include <netinet/in.h>
#endif


#include <string>


class OSCPacker
{
  private:
    std::string myData;

  protected:
    std::string *getDataPointer()  { return &(this->myData); }
    bool appendFourBytes  (char *bytes);
    bool appendEightBytes (char *bytes);
    bool appendBytes      (char *bytes, int count);
    /// Pads the given string to a multiple of four bytes.
    void padString(std::string *data);
    virtual bool packBytes  (const char*, int bytes);

  public:
    OSCPacker() {}
    virtual ~OSCPacker()  {}
    
    virtual void init()     {}
    virtual void reset();

    virtual bool packInt    (int32_t);
    virtual bool packHyper  (int64_t);
    virtual bool packFloat  (float);
    virtual bool packDouble (double);
    virtual bool packString (const std::string*);
    virtual bool packString (const char*);
    virtual bool packBlob   (const char*, int bytes);
      
    virtual std::string getData();
};

#endif
