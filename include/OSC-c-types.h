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

/* @file OSC-c-types.h
 *
 * Provide some backwards compatibility for CNMAT C callbacks.
 *
 * This file contains a few typedefs necessary for old-style C callbacks
 * to work with the compatibility layer of ProctoLogic's OSC kit.
 *
 * @author Daniel Holth
 * @author Clinton McChesney
 */

#ifndef OSC_C_TYPES_H
#define OSC_C_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "int64_t.h"

typedef void* NetworkReturnAddressPtr;
typedef int64_t  OSCTimeTag;
typedef void (*methodCallback)(void *context, int arglen, const void *args, 
			        OSCTimeTag when, NetworkReturnAddressPtr returnAddr);

#ifdef __cplusplus
}
#endif

#endif
