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

#ifndef __artd_Stream_h
#define __artd_Stream_h

#include "artd/jlib_io.h"
#include "artd/ObjectBase.h"

ARTD_BEGIN

#define INL ARTD_ALWAYS_INLINE

class ARTD_API_JLIB_IO Stream
	: public ObjectBase
{
protected:
    INL Stream() {}
public:
	enum {
		ISATTY			= 0x01,  // it is an interactive stream
		fMarkSupported	= 0x02,
		fIsFdStream		= 0x04,  // is derived from a platform "file descriptor"
		fIsStdioStream	= 0x08,  // is derived from a stdio FILE*
		fIsOpen		    = 0x10,  // stream is open and not in error status
		
		fFirstAvailable = 0x100,
	};

	virtual int			close() = 0;
	virtual unsigned int getFlags(unsigned int toGet) = 0;
	inline unsigned int isInteractive() { return(getFlags(ISATTY)); }
	inline unsigned int markSupported() { return(getFlags(fMarkSupported)); }
	inline bool isClosed() { return(getFlags(fIsOpen) == 0); }

	virtual size_t		getBytePosition() = 0;
};

ARTD_END

#endif // __artd_Stream_h


