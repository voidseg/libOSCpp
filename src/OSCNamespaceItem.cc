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

#include "OSCNamespaceItem.h"
#include "oscmatch.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

OSCNamespaceItem::OSCNamespaceItem()
{
    this->myCallable = NULL;
    this->mySubItems = new OSCNamespaceItem::ItemMap();
}

OSCNamespaceItem::~OSCNamespaceItem()
{
    delete this->myCallable;
    delete this->mySubItems;
}


// non-polymorphic, non-recursive version.
/// Add OSCNamespaceItem to a tree of OSCNamespaceItems, putting in
/// intermediate items as we go along.
// bool OSCNamespaceItem::add(std::string &name, OSCNamespaceItem *addme)
bool OSCNamespaceItem::add(std::string name, OSCCallable *callback)
{
    std::vector<std::string> tokens;

    std::string delimiters = "/";

    std::string::size_type lastPos = name.find_first_not_of("/", 0);
    std::string::size_type pos     = name.find_first_of("/", lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(name.substr(lastPos, pos - lastPos));
        lastPos = name.find_first_not_of(delimiters, pos);
        pos = name.find_first_of(delimiters, lastPos);
    }

    std::vector<std::string>::iterator iter;
    std::vector<std::string>::iterator end;
    OSCNamespaceItem::ItemMap *subitems;
    OSCNamespaceItem::ItemMap::iterator item;
    OSCNamespaceItem *current = NULL;

    iter = tokens.begin();
    end  = tokens.end();
    subitems = this->mySubItems;

    while(iter != end)
    {
        if((item = subitems->find(*iter)) == subitems->end())
        {
            std::cout << "adding " << *iter << "\n";
            current = new OSCNamespaceItem();
            (*subitems)[*iter] = current;
        }
        else
        {
            current = item->second;
        }
        subitems = current->mySubItems;
        iter++;
    }

    if(current)
    {
        current->setCallable(callback);
    }

    return true;
}


// For debugging.
bool OSCNamespaceItem::list(int level)
{
    ItemMap *subitems;
    ItemMap::iterator iter;
    ItemMap::iterator end;

    subitems = this->mySubItems;

    iter = subitems->begin();
    end  = subitems->end();

    while (iter != end)
    {
        std::cout << std::string(level, ' ') << iter->first << std::endl;
        if (iter->second != NULL)
        {
            iter->second->list(level+1);
        }
        iter++;
    }

    return true;
}


bool OSCNamespaceItem::traverse(const std::string &pattern,
        TraverseFunction tf, void *cookie) {
    std::vector<std::string> address;
    return this->search_internal(cookie, tf, address, pattern, 1);
}


bool OSCNamespaceItem::search_internal(void *cookie,
        TraverseFunction tf,
        std::vector < std::string > &address,
        const std::string &pattern,
        size_t offset)
{
    OSCNamespaceItem *item = NULL;
    std::string target;
    size_t next;
    ItemMap *subitems;
    ItemMap::iterator iter;
    subitems = this->mySubItems;

    next = pattern.find('/', offset);

    target = pattern.substr(offset, next - offset);

    iter = subitems->find(target);

    if(iter != subitems->end())
    {
        // found it the cheap way... no pattern matching.
        address.push_back(target);
        if(next == pattern.npos)
        {
            item = iter->second;
            tf(cookie, address, item);
        }
        else
        {
            return iter->second->search_internal(cookie,
                    tf,
                    address,
                    pattern,
                    next+1);
        }
    }
    else if(target.size() > 0)
    {
        // attempt pattern matching
        ItemMap::iterator end;
        iter = subitems->begin();
        end  = subitems->end();
        const char* glob = target.c_str();

        // cout << "globbing on \"" << target << "\"" << endl;

        while(iter != end)
        {
            if(oscmatch(iter->first.c_str(), glob))
            {
                // cout << "Matched "
                //     << glob
                //     << " "
                //     << iter->first << endl;

                item = iter->second;
                address.push_back(iter->first);

                if(next == target.npos)
                {
                    tf(cookie, address, item);
                }
                else
                {
                    iter->second->search_internal(cookie, tf, address, 
                            pattern, next+1);
                }

                address.pop_back();
            }
            iter++;
        }
    }
    else
    {
        // time to list the address, if this is how we want to implement it.
        // it is also possible to have a "" entry in the namespace
        // that had a special listing callback.
        address.push_back(target);
        tf(cookie, address, this);
    }

    return true;
}

