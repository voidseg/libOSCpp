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
 * @file socklen_t.h
 *
 * ensure socklen_t is defined.
 *
 * makes sure socklen_t is defined whether we're on a BSD-like system or not.
 * BSD and systems like it in networking (e.g. MacOSX, BeOS) use int where
 * other systems (e.g. Linux) use socklen_t
 *
 * @author Jared Jennings
 */

#ifndef INCLUDED_SOCKLEN_T_H
#define INCLUDED_SOCKLEN_T_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef    __CYGWIN__

#else  // __CYGWIN__
#ifndef HAVE_SOCKLEN_T
/* everywhere there's no socklen_t (i.e., BSD) we use int instead */
// typedef int32_t socklen_t; // now done in config.h
#endif

#endif // __CYGWIN__

#endif
