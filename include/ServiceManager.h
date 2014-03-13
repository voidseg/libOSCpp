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
 * @class ServiceManager
 *
 * This class is an interface to the
 * set of listening Service classes.
 *
 * A service can use this to spawn new
 * services.
 */

#ifndef LIBOSC_SERVICEMANAGER_H
#define LIBOSC_SERVICEMANAGER_H

class Service;

#include "Service.h"

class ServiceManager
{
public:
  virtual ~ServiceManager (void) { }

  virtual void addService(Service *service) = 0;

  virtual void delService(Service *service) = 0;

};

#endif // #ifndef LIBOSC_SERVICEMANAGER_H

