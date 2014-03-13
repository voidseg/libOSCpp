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
/* http://wiretap.stetson.edu/trac/                                          */

/**
 *  @file OSCHierarchicalNamespace.cc
 *
 *  Hierarchical implementation of a namespace, including pattern matching.
 *
 *  @bug Does not yet implement 'remove'
 *  @bug needs testing
 *
 *  @author Daniel Holth
 */

#include "OSCHierarchicalNamespace.h"

OSCHierarchicalNamespace::OSCHierarchicalNamespace()
{
    this->myRoot = new OSCNamespaceItem();
}


OSCHierarchicalNamespace::~OSCHierarchicalNamespace()
{
    delete this->getRoot();
}


bool OSCHierarchicalNamespace::add(const std::string &address, 
        OSCCallable *thingamabob)
{
    OSCNamespaceItem *root;

    root = this->getRoot();
    return root->add(address, thingamabob);
}


bool OSCHierarchicalNamespace::remove(const std::string& address)
{
    return false;
}


OSCCallable* OSCHierarchicalNamespace::find(const std::string& address)
{
    return NULL;
}


// listing will be internal to the namespace
std::vector<std::string> OSCHierarchicalNamespace::list(const std::string& basename)
{
    std::vector<std::string> addresses;
    OSCNamespaceItem *root;
    root = this->getRoot();
    // root->traverse((void*)&addresses, &dispatchOSC)
    return addresses;
}

// this is confusing.
typedef std::pair <const std::string, Transmit*> Cook;

void dispatchOSC(void *cookie, std::vector < std::string > &address, OSCNamespaceItem *item)
{
    OSCCallable *callback;

    if(item != NULL)
    {
        callback = item->getCallable();
        if(callback != NULL && cookie != NULL)
        {
            callback->call(((Cook*)cookie)->first, ((Cook*)cookie)->second);
        }
    }
}


bool OSCHierarchicalNamespace::dispatch(const std::string &address, 
                                        const std::string &data, 
                                        Transmit *const reply)
{
    OSCNamespaceItem *root;
    Cook cookie(data, reply);

    root = this->getRoot();
    return root->traverse(address, &dispatchOSC, (void*)&cookie);
}

