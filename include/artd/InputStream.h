/*-
 * Copyright (c) 1985-2009 Peter Kennard and aRt&D Lab
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of the source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Any redistribution solely in binary form must conspicuously
 *    reproduce the following disclaimer in documentation provided with the
 *    binary redistribution.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'', WITHOUT ANY WARRANTIES, EXPRESS
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  LICENSOR SHALL
 * NOT BE LIABLE FOR ANY LOSS OR DAMAGES RESULTING FROM THE USE OF THIS
 * SOFTWARE, EITHER ALONE OR IN COMBINATION WITH ANY OTHER SOFTWARE.
 * 
 * 	$Id$
 */

#ifndef __artd_InputStream_h
#define __artd_InputStream_h

#include "artd/Stream.h"

ARTD_BEGIN

class InputStreamReader;

#define INL ARTD_ALWAYS_INLINE

class ARTD_API_JLIB_IO InputStream
	: public Stream
{
protected:
    InputStream() {}
public: // Stream:: overrides
	int flush() { return(-1); }
public: // InputStream::
	// returns 0 to 0xFF for valid reads < 0 for error 
	virtual int		    get(void) = 0;
	virtual ptrdiff_t   read(void *buf, ptrdiff_t len) = 0;
	virtual ptrdiff_t   skip(ptrdiff_t bytes) = 0;
	INL  int         read() { return(get()); }

	// calls "get()" for every character
	/* ARTD_API_JLIB_CORE */ ptrdiff_t    _read(void *buf, ptrdiff_t len);
	// calls "read(ret[1],1)"
	/* ARTD_API_JLIB_CORE */ int          _get();
	// calls "read" into dummy buffer
	/* ARTD_API_JLIB_CORE */ ptrdiff_t	_skip(ptrdiff_t bytes);
};

#undef INL

ARTD_END

#endif // __artd_InputStream_h

