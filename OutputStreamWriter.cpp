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
#include "artd/OutputStreamWriter.h"
#include "artd/Formatf.h"
#include <string>

#ifdef ARTD_LINUX
	#include <wchar.h>
#endif

ARTD_BEGIN

OutputStreamWriter::OutputStreamWriter(ObjectPtr<OutputStream> os, const char *charset)
	: out_(os)
{
}
OutputStreamWriter::~OutputStreamWriter()
{
}

unsigned int
OutputStreamWriter::getFlags(unsigned int toGet)
{
	return(out_->getFlags(toGet));
}
int
OutputStreamWriter::close()
{
	return(out_->close());
}
size_t
OutputStreamWriter::getBytePosition()
{
	return(out_->getBytePosition());
}
int
OutputStreamWriter::putchar(int c)
{
	return(out_->put(c));
}
int
OutputStreamWriter::flush()
{
	return(out_->flush());
}
int
OutputStreamWriter::puts(const char *buf, int len)
{
	return((int)out_->write(buf,len));
}
int
OutputStreamWriter::puts(const wchar_t *buf, int len)
{
	// TODO: real utf-8 and unicode, optimize for big lengths

	char out[128];
	int ix = 0;
	while(len > 0)
	{
		out[ix] = (char)*buf++;
		if(++ix >= (int)sizeof(out)) {
			if(out_->write(out,ix) < 0 ) {
				return(-1);
			}
			ix = 0;
		}
		--len;
	}
	if(ix) {
		if(out_->write(out,ix) < 0) {
			return(-1);
		}
	}
	return(len);
}
int
OutputStreamWriter::cputs(const char *buf)
{
	return((int)out_->write(buf,::strlen(buf)));
}
int
OutputStreamWriter::cputs(const wchar_t *buf)
{
		return((int)out_->write(buf,::wcslen(buf)));
}
int
OutputStreamWriter::vwritef(const char *fmt, const FormatfArglist<> &args)
{
	char out[128];
	int ret;
	int bytesdone = 0;
	FormatfStream fa;
	fa.va_init(fmt,args);

	for(;;)
	{
		if((ret = fa.read(out,sizeof(out))) <= 0) {
			break;
		}
		bytesdone += ret;
		if((ret = (int)(out_->write(out,ret))) < 0) {
			return(ret);
		}
	}
	return(bytesdone);
}
int
OutputStreamWriter::vwritef(const wchar_t *fmt, const FormatfArglist<> &args)
{
	wchar_t out[128];
	int ret;
	int bytesdone = 0;
	FormatfStream fa;
	fa.va_init(fmt,args);

	for(;;)
	{
		if((ret = fa.read(out,sizeof(out))) <= 0) {
			break;
		}
		bytesdone += ret;
		// TODO: real utf-8 and unicode optimize
		if((ret = puts(out,ret/sizeof(wchar_t))) < 0) {
			return(ret);
		}
	}
	return(bytesdone);
}


ARTD_END
