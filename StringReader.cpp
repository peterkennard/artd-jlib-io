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
#include "artd/StringReader.h"
#include "artd/pointer_math.h"

ARTD_BEGIN

StringReader::StringReader(const wchar_t *str)
{
	mask_ = 0x0FFFFFFF;
	esize_ = sizeof(wchar_t);
	if(str == 0) {
		close();
	} else {
		p_ = str_ = str;
	}
}
StringReader::StringReader(const char *str)
{
	if(str == 0) {
		close();
	} else {
		mask_ = 0x0FF;
		esize_ = sizeof(char);
		p_ = str_ = (const wchar_t *)str;
	}
}
StringReader::StringReader(RcWString str)
{
	setSource(str);
}
void
StringReader::setSource(RcWString str)
{
	if(!str) {
		close();
	} else {
		mask_ = 0x0FFFFFFF;
		esize_ = sizeof(wchar_t);
		rc_ = str;
		p_ = str_ = rc_.c_str();
	}
}
StringReader::~StringReader()
{	
}
unsigned int 
StringReader::getFlags(unsigned int toGet)
{
	return(0);
}
int	
StringReader::close()
{
	str_ = L"";
	p_ = str_;
	rc_ = nullptr;
	return(0);
}
size_t			
StringReader::getBytePosition()
{
	return(0);
}
int 
StringReader::read(void)
{
	int ret = *((wchar_t *)p_) & mask_;
	if(ret) {
		p_ = (const wchar_t *)ARTD_OPTR(p_,esize_);
		return(ret);
	}
	return(-1);
}
int
StringReader::read(wchar_t *b, int len)
{
	int lout = 0;
	if(esize_ == sizeof(char)) {
		while(lout < len) {
			if(!(*b++ = *((unsigned char *)p_))) {
				break;
			}
			p_ = (const wchar_t *)ARTD_OPTR(p_,sizeof(char));
			++lout;
		}
	} else {
		while(lout < len) {
			if(!(*b++ = *p_++)) {
				break;
			}
			++lout;
		}
	}
	return(lout);
}

ARTD_END
