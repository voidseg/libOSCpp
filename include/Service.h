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
 * @class Service
 *
 * Services base class, independent of network type.
 *
 * this class is the services base class, which
 * is independent of the underlying transport system
 *
 * The handle() method should not block and should
 * do some small unit of work.  The Services are
 * designed to live in a single thread, so if one
 * does block other work will be suspended.
 *
 * @author  Daniel Holth
 * Created: 23 Jan. 2002
 * @author  Martin Rumori
 * Modified: 02/2003
 */

#ifndef LIBOSC_SERVICE_H
#define LIBOSC_SERVICE_H

class Service
{
public:
  /// destructor
  virtual ~Service () { }

  /** handle the service
   *
   *  \return true on success,
   *  false on error or when the Service is finished
   */
  virtual bool handle() = 0;

};

#endif // #ifndef LIBOSC_SERVICE_H
