/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002-2004 libOSC++ contributors. See AUTHORS		         */
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
 * @class OSCFunctionCallable
 *
 * Write a callable without writing a new class.
 *
 * @author Daniel Holth
 * @date 2004 Oct. 30
 */

#ifndef OSC_FUNCTION_CALLABLE_H
#define OSC_FUNCTION_CALLABLE_H

#include "OSCCallable.h"
#include "Transmit.h"
#include <string>

#include "int64_t.h"


class OSCFunctionCallable : public OSCCallable
{
    public:
        typedef void (*Callback)(void *context, const std::string &data, Transmit *const reply);
    private:
        Callback myFunction;
        void *myContext;
    protected:
        Callback getFunction() { return this->myFunction; }
        void *getContext()  { return this->myContext; }
    public:
        OSCFunctionCallable() { this->setFunction(NULL, NULL); }
        OSCFunctionCallable(Callback function, void *context) { this->setFunction(function, context); }

        /**
         * Sets the function.
         *
         * @param function Pointer to an OSCFunctionCallable::Callback-style function
         * @pre   None
         * @post  Function will be called when this->call() is called.
         */
        void setFunction(Callback function, void *context) 
        {
            this->myFunction = function; 
            this->myContext  = context;
        }

        virtual void call(const std::string& data, Transmit *const reply)
        {
            (*(this->getFunction()))(this->myContext, data, reply);
        }
};

#endif

