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
 * @class OSCHierarchicalNamespace
 *
 * Namespace supporting pattern matching and a true address tree.
 *
 * Namespace that's hard to spell, having a true tree of items,
 * supporting pattern matching and multiple dispatch.
 *
 * @author Daniel Holth
 */

#ifndef OSC_HIERARCHICAL_NAMESPACE_H
#define OSC_HIERARCHICAL_NAMESPACE_H

#include <map>
#include <string>
#include "OSCNamespace.h"
#include "OSCNamespaceItem.h"
#include "OSCCallable.h"
#include "Transmit.h"

class OSCHierarchicalNamespace : public OSCNamespace
{
  protected:
    typedef std::map < std::string, OSCCallable*, std::less<std::string> > CallableMap;

  private:
    // root of our tree of containers and callbacks.
    OSCNamespaceItem* myRoot;
    
  protected:
    void setRoot(OSCNamespaceItem *root) { this->myRoot = root; }
    OSCNamespaceItem *getRoot()          { return this->myRoot; }

  public:
    OSCHierarchicalNamespace();
    virtual ~OSCHierarchicalNamespace();

    virtual bool add(const std::string &address, OSCCallable *thingamabob);
    virtual bool remove(const std::string& address);

    /// @bug only finds one, even though we can have wildcards in address. try dispatch()
    virtual OSCCallable* find(const std::string& address);

    /// Invoke callback for all addresses matching std::string &address.
    virtual bool dispatch(const std::string &address, const std::string& data, Transmit *const reply);

    /// This will be implemented outside the Namespace object when
    /// we obsolete the /list method in favor of appended-/ list requests.
    virtual std::vector<std::string> list(const std::string& basename);
};

#endif
