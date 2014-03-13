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
 * @file c_callbacks.h
 *
 * Contains declarations for the old callbacks.
 * 
 * @author Daniel Holth
 * @author Clinton McChesney
 */

#ifndef C_CALLBACKS_H
#define C_CALLBACKS_H

#include "OSC-c-types.h"

#ifdef __cplusplus
extern "C" {
#endif
  
void BarCallback(void *context, 
                 int arglen, 
                 const void *args, 
                 OSCTimeTag when, 
                 NetworkReturnAddressPtr returnAddr);

void FarCallback(void *context, 
                 int arglen, 
                 const void *args,
                 OSCTimeTag when, 
                 NetworkReturnAddressPtr returnAddr);

void PlayCallback(void *context, 
                  int arglen, 
                  const void *args,
                  OSCTimeTag when, 
                  NetworkReturnAddressPtr returnAddr); 

void PlaySpeedCallback(void *context, 
                       int arglen, 
                       const void *args, 
                       OSCTimeTag when, 
                       NetworkReturnAddressPtr returnAddr);

void PlayPanVolumeSpeedCallback(void *context, 
                                int arglen,
                                const void *args, 
                                OSCTimeTag when,
                                NetworkReturnAddressPtr returnAddr);

#ifdef __cplusplus
}
#endif

#endif
