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

#ifndef __artd_TextWriter_h
#define __artd_TextWriter_h

#include "artd/jlib_io.h"
#include "artd/OutputStream.h" 
#include "artd/int_types.h"
#include "artd/FormatfArglist.h"

ARTD_BEGIN

class ARTD_API_JLIB_IO TextWriter
	: public Stream
{
public:	
	virtual OutputStream *getOutputStream() = 0;
	
	virtual int flush() = 0;
	virtual int putchar(int c) = 0;
	virtual int puts(const char *buf, int len) = 0;
	virtual int puts(const wchar_t *buf, int len) = 0;
	
	virtual int cputs(const char *buf) = 0;
	virtual int cputs(const wchar_t *buf) = 0;
	virtual int vwritef(const char *fmt, const FormatfArglist<> &args) = 0;
	virtual int vwritef(const wchar_t *fmt, const FormatfArglist<> &args) = 0;

	template <typename... Args>
	int writef(const StringArg &fmt,const Args &... args) {
		FormatfArglist<sizeof...(Args)> arglist;
		FormatfArgBase::addArgs(arglist.args(), args...);
		return(vwritef(fmt.c_str(), arglist));
	}
	template <typename... Args>
	int writef(const WStringArg &fmt,const Args &... args) {
		FormatfArglist<sizeof...(Args)> arglist;
		FormatfArgBase::addArgs(arglist.args(), args...);
		return(vwritef(fmt.c_str(), arglist));
	}
};

class ARTD_API_JLIB_IO PrintWriter 
	: public TextWriter
{
public:
	inline operator OutputStream &() { return(*getOutputStream()); }
	inline operator OutputStream *() { return(getOutputStream()); }
};

ARTD_END

#endif // __artd_TextWriter_h


