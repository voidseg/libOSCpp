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

#ifndef INCLUDED_INT64_T_H
#define INCLUDED_INT64_T_H

/**
 * @file int64_t.h
 *
 * attempts to pull in a definition for 64-bit integers which is as portable as
 * possible. inttypes.h if possible, stdint.h if not. 
 *
 * stdint.h is part of the C99 standard - this is not a subset of C++ (as
 * dholth has said.) inttypes.h is apparently available in more places (e.g.,
 * macosx) - i think i found this out via googling.
 *
 * @author Jared Jennings
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef    __CYGWIN__
#include <sys/types.h>
#include <cygwin/types.h>
#else  // __CYGWIN__

#ifndef HAVE_INTTYPES_H
#include <inttypes.h>
#else
#include <stdint.h>
#endif

#endif // __CYGWIN__

#endif /* INCLUDED_INT64_T_H */

