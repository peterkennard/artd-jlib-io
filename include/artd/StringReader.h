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

#ifndef __artd_StringReader_h
#define __artd_StringReader_h

#include "artd/jlib_io.h"
#include "artd/TextReader.h" 
#include "artd/RcString.h" 

ARTD_BEGIN

class ARTD_API_JLIB_IO StringReader
	: public TextReader
{

protected:

	const wchar_t *	str_;
	const wchar_t *	p_;
	int				mask_; 
	int				esize_; 
	RcWString		rc_;
	
	void *reserved_[1];

public:

	explicit StringReader(const wchar_t *str);
	StringReader(const char *str);
	StringReader(RcWString str);

	~StringReader();

	int				close();
	size_t			getBytePosition();
	unsigned int	getFlags(unsigned int toGet);
	int				read();
	int				read(wchar_t *b, int len);

	void			setSource(RcWString str);
};

ARTD_END

#endif // __artd_StringReader_h


