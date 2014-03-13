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
 *  @class OSCNamespaceItem
 *
 *  Represents a single component of the OSC namespace and contains the
 *  items underneath it.
 *
 *  / foo / bar / baz
 *
 *  (the delimiters are only stored implicitly)
 *
 *  @author Daniel Holth
 */

// eventually but not right now, in order to have an elegant implementation,
// there may be a null string item at the beginning of every / :
// ""/foo/bar/baz .  that is because the root of the entire osc namespace,
// before the first /, can also contain a number of elements beginning
// with the # character. Our container will do a very efficient lookup via
// binary search or hashing, and we avoid the horror of ten if statements
// or a completely separate root for the hash namespace.

/* ChangeLog:
 *
 * 18 Oct. 2004: created - dwh
 * 29 Oct. 2004: begin multiple-result query functions
 *  1 Nov. 2004: enough functionality for multiple dispatch
 */

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "OSCCallable.h"

class OSCNamespaceItem {
    public:
        // consider substituting hash map
        typedef std::map < std::string, OSCNamespaceItem*, std::less<std::string> > ItemMap;

        typedef void (*TraverseFunction)(void* cookie,
                                         std::vector < std::string > &address,
                                         OSCNamespaceItem *item);

    protected:
        bool search_internal(void *cookie,
                TraverseFunction tf, std::vector < std::string > &address,
                const std::string &name, size_t offset);
        
    public:
        ItemMap* mySubItems;
        OSCCallable *myCallable; // NULL unless this is a leaf node

    public:
        OSCNamespaceItem();
        ~OSCNamespaceItem();

        void setCallable(OSCCallable *callback) { this->myCallable = callback; }
        OSCCallable *getCallable()              { return this->myCallable; }

        /// Call function tf for each address matching const std::string &pattern
        /// @arg pattern address or pattern to look up
        /// @arg tf void (*)(void*, std::vector <std::string>&, OSCNamespaceItem*)
        ///  called for each matching address in the namespace
        /// @arg cookie optional state for function tf
        bool traverse(const std::string &pattern, TraverseFunction tf, void *cookie = NULL);

        /// Add address and optional callback to the namespace.
        bool add(std::string name, OSCCallable *callback = NULL);
    
        /// print namespace recursive function
        /// for debugging
        bool list(int level);
};
