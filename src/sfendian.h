/*
** Copyright (C) 1999-2012 Erik de Castro Lopo <erikd@mega-nerd.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation; either version 2.1 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef SFENDIAN_INCLUDED
#define SFENDIAN_INCLUDED

#include "sfconfig.h"

#include <stdint.h>
#include <inttypes.h>


#if HAVE_BYTESWAP_H			/* Linux, all CPUs */
#include <byteswap.h>

#define	ENDSWAP_16(x)		(bswap_16 (x))
#define	ENDSWAP_32(x)		(bswap_32 (x))
#define	ENDSWAP_64(x)		(bswap_64 (x))

#elif HAVE_X86INTRIN_H		/* x86 and x86_64 with GCC */

#include <x86intrin.h>

#define	ENDSWAP_16(x)		((((x) >> 8) & 0xFF) + (((x) & 0xFF) << 8))
#define	ENDSWAP_32(x)		(__bswapd (x))
#define	ENDSWAP_64(x)		(_bswap64 (x))

#else

#define	ENDSWAP_16(x)		((((x) >> 8) & 0xFF) + (((x) & 0xFF) << 8))
#define	ENDSWAP_32(x)		((((x) >> 24) & 0xFF) + (((x) >> 8) & 0xFF00) + (((x) & 0xFF00) << 8) + (((x) & 0xFF) << 24))

#endif

/*
** Many file types (ie WAV, AIFF) use sets of four consecutive bytes as a
** marker indicating different sections of the file.
** The following MAKE_MARKER macro allows th creation of integer constants
** for these markers.
*/

#if (CPU_IS_LITTLE_ENDIAN == 1)
	#define	MAKE_MARKER(a, b, c, d)		((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))
#elif (CPU_IS_BIG_ENDIAN == 1)
	#define	MAKE_MARKER(a, b, c, d)		(((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
#else
	#error "Target CPU endian-ness unknown. May need to hand edit src/sfconfig.h"
#endif

/*
** Macros to handle reading of data of a specific endian-ness into host endian
** shorts and ints. The single input is an unsigned char* pointer to the start
** of the object. There are two versions of each macro as we need to deal with
** both big and little endian CPUs.
*/

#if (CPU_IS_LITTLE_ENDIAN == 1)
	#define LE2H_16(x)			(x)
	#define LE2H_32(x)			(x)

	#define BE2H_16(x)			ENDSWAP_16 (x)
	#define BE2H_32(x)			ENDSWAP_32 (x)
	#define BE2H_64(x)			ENDSWAP_64 (x)

	#define H2BE_16(x)			ENDSWAP_16 (x)
	#define H2BE_32(x)			ENDSWAP_32 (x)

#elif (CPU_IS_BIG_ENDIAN == 1)
	#define LE2H_16(x)			ENDSWAP_16 (x)
	#define LE2H_32(x)			ENDSWAP_32 (x)

	#define BE2H_16(x)			(x)
	#define BE2H_32(x)			(x)
	#define	BE2H_64(x)			(x)

	#define H2LE_16(x)			ENDSWAP_16 (x)
	#define H2LE_32(x)			ENDSWAP_32 (x)

#else
	#error "Target CPU endian-ness unknown. May need to hand edit src/sfconfig.h"
#endif

#define LET2H_16_PTR(x)			((x) [1] + ((x) [2] << 8))
#define LET2H_32_PTR(x)			(((x) [0] << 8) + ((x) [1] << 16) + ((x) [2] << 24))

#define BET2H_16_PTR(x)			(((x) [0] << 8) + (x) [1])
#define BET2H_32_PTR(x)			(((x) [0] << 24) + ((x) [1] << 16) + ((x) [2] << 8))

/*-----------------------------------------------------------------------------------------------
** Generic functions for performing endian swapping on integer arrays.
*/

static inline void
endswap_short_array (short *ptr, int len)
{	short	temp ;

	while (--len >= 0)
	{	temp = ptr [len] ;
		ptr [len] = ENDSWAP_16 (temp) ;
		} ;
} /* endswap_short_array */

static inline void
endswap_short_copy (short *dest, const short *src, int len)
{
	while (--len >= 0)
	{	dest [len] = ENDSWAP_16 (src [len]) ;
		} ;
} /* endswap_short_copy */

static inline void
endswap_int_array (int *ptr, int len)
{	int temp ;

	while (--len >= 0)
	{	temp = ptr [len] ;
		ptr [len] = ENDSWAP_32 (temp) ;
		} ;
} /* endswap_int_array */

static inline void
endswap_int_copy (int *dest, const int *src, int len)
{
	while (--len >= 0)
	{	dest [len] = ENDSWAP_32 (src [len]) ;
		} ;
} /* endswap_int_copy */

/*========================================================================================
*/

static inline void
endswap_int64_t_array (int64_t *ptr, int len)
{	int64_t value ;

	while (--len >= 0)
	{	value = ptr [len] ;
		ptr [len] = ENDSWAP_64 (value) ;
		} ;
} /* endswap_int64_t_array */

static inline void
endswap_int64_t_copy (int64_t *dest, const int64_t *src, int len)
{	int64_t value ;

	while (--len >= 0)
	{	value = src [len] ;
		dest [len] = ENDSWAP_64 (value) ;
		} ;
} /* endswap_int64_t_copy */

/* A couple of wrapper functions. */

static inline void
endswap_float_array (float *ptr, int len)
{	endswap_int_array ((int *) ptr, len) ;
} /* endswap_float_array */

static inline void
endswap_double_array (double *ptr, int len)
{	endswap_int64_t_array ((int64_t *) ptr, len) ;
} /* endswap_double_array */

static inline void
endswap_float_copy (float *dest, const float *src, int len)
{	endswap_int_copy ((int *) dest, (const int *) src, len) ;
} /* endswap_float_copy */

static inline void
endswap_double_copy (double *dest, const double *src, int len)
{	endswap_int64_t_copy ((int64_t *) dest, (const int64_t *) src, len) ;
} /* endswap_double_copy */

#endif /* SFENDIAN_INCLUDED */

