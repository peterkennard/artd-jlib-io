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

#ifndef __artd_OutputStream_h
#define __artd_OutputStream_h

#include "artd/Stream.h"

ARTD_BEGIN

class OutputStreamWriter;

class ARTD_API_JLIB_IO OutputStream
	: public Stream
{
protected:
    OutputStream() {}
public:

	virtual int         put(unsigned char b) = 0;
	// java like inline
	inline  int         write(int oneChar) { return(put(oneChar)); }
	
	virtual ptrdiff_t   write(const void *buf, ptrdiff_t len) = 0;
	virtual int 	    flush() = 0;

	// calls write(&b,1)
	int					_put(unsigned char b);
	// calls put for each byte
	ptrdiff_t			_write(const void *buf, ptrdiff_t len);
};

ARTD_END


#endif // __artd_OutputStream_h
