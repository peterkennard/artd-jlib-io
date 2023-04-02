/*-
 * Copyright (c) 1985-2004 Peter Kennard and aRt&D Lab
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

#ifndef __artd_StdioOutputSream_h
#define __artd_StdioOutputSream_h

#include "artd/jlib_io.h"
#include "artd/OutputStream.h" 
#include "stdio.h"

ARTD_BEGIN

class ARTD_API_JLIB_IO StdioOutputStream
	: public OutputStream
{
public:

	enum {
		fOwnsFILE = OutputStream::fFirstAvailable,
		fFirstAvailable = OutputStream::fFirstAvailable << 1
	};

	StdioOutputStream(FILE *file, bool ownsFile = false);
	~StdioOutputStream();

	FILE *getFILE() const { return(file_); }

	int         put(unsigned char b);	
	ptrdiff_t   write(const void *buf, ptrdiff_t len);
	int 	    flush();
	unsigned int getFlags(unsigned int toGet);
	size_t		getBytePosition();
	int			close();

protected:
	int flags_;
	FILE *file_;
};

ARTD_END

#endif // __artd_StdioOutputSream_h


