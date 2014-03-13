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

#ifndef HYPERSWAP_H
#define HYPERSWAP_H

// #include <byteswap.h>

# if __BYTE_ORDER == __BIG_ENDIAN
/* The host byte order is the same as network byte order,
 *    so these functions are just identity */
# define ntohll(x)   (x)
# define htonll(x)   (x)
# else

/* copied from GNU /usr/include/bits/byteswap.h as it is not present
 * e.g. on mac.  this is ugly.
 */
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#if defined __GNUC__ && __GNUC__ >= 2
/* Swap bytes in 64 bit value.  */
# define __bswap_constant_64(x) \
     ((((x) & 0xff00000000000000ull) >> 56)				      \
      | (((x) & 0x00ff000000000000ull) >> 40)				      \
      | (((x) & 0x0000ff0000000000ull) >> 24)				      \
      | (((x) & 0x000000ff00000000ull) >> 8)				      \
      | (((x) & 0x00000000ff000000ull) << 8)				      \
      | (((x) & 0x0000000000ff0000ull) << 24)				      \
      | (((x) & 0x000000000000ff00ull) << 40)				      \
      | (((x) & 0x00000000000000ffull) << 56))

# if __WORDSIZE == 64
#  define __bswap_64(x) \
     (__extension__							      \
      ({ register unsigned long __v, __x = (x);				      \
	 if (__builtin_constant_p (__x))				      \
	   __v = __bswap_constant_64 (__x);				      \
	 else								      \
	   __asm__ ("bswap %q0" : "=r" (__v) : "0" (__x));		      \
	 __v; }))
# else
#  define __bswap_64(x) \
     (__extension__                                                           \
      ({ union { __extension__ unsigned long long int __ll;                   \
                 unsigned int __l[2]; } __w, __r;                             \
         if (__builtin_constant_p (x))                                        \
           __r.__ll = __bswap_constant_64 (x);                                \
         else                                                                 \
           {                                                                  \
             __w.__ll = (x);                                                  \
             __r.__l[0] = __bswap_32 (__w.__l[1]);                            \
             __r.__l[1] = __bswap_32 (__w.__l[0]);                            \
           }                                                                  \
         __r.__ll; }))
# endif
#endif
/* end copied from /usr/include/bits/byteswap.h */

#   define ntohll(x) __bswap_64 (x)
#   define htonll(x) __bswap_64 (x)
#  endif
# endif

#endif
