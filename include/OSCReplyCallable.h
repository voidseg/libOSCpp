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
 * @class OSCReplyCallable
 *
 * A Callable that prints (debug) messages
 * and replies.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 * @author Martin Rumori
 */

#ifndef OSC_REPLY_CALLABLE_H
#define OSC_REPLY_CALLABLE_H

#include "OSCCallable.h"
#include "Transmit.h"

class OSCReplyCallable : public OSCCallable
{
  private:
    std::string message;
  public:
    OSCReplyCallable(std::string message) { this->message = message; }
    virtual ~OSCReplyCallable()      { }
    virtual void call(const std::string& data, Transmit *const reply) 
    {
      cout << this->message << " " << data << endl;
      reply->transmit("Hey man.");
    };
};

#endif
