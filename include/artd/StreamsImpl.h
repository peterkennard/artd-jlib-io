/*
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
 *  $Id$
 */

#ifndef __artd_StreamsImpl_h
#define __artd_StreamsImpl_h

#include <artd/FileInputStream.h>
#include <artd/FileOutputStream.h>
#include <artd/RandomAccessFile.h>

ARTD_BEGIN

#define INL ARTD_ALWAYS_INLINE

#ifdef _MSC_VER
	#pragma warning(disable : 4275) // non export inteface but root is export
#endif

#ifndef WINAPI
	#ifdef ARTD_WINDOWS
		#define HANDLE void*
	#else
		#define HANDLE int
	#endif
#endif

class ARTD_API_JLIB_IO StreamBaseImpl
{
public:

protected:

	HANDLE 			hfile_;
	int				flags_;

	INL StreamBaseImpl()
		: hfile_(0)
		, flags_(0)
	{}
	~StreamBaseImpl();
	// internal flags
	enum {
		fOwnsHandle = 0x10000, // owns the handle and will close it when destroyed
		fNone = 0
	};

	unsigned int    doGetFlags(unsigned int which);
	int             getbyte();
	int             putbyte(unsigned char b);
	int             doClose();
	ptrdiff_t       doRead(void *b, ptrdiff_t l);
	ptrdiff_t       doWrite(const void *b, ptrdiff_t l);
	int             doFlush();
	int64_t         doGetBytePosition();
	int				doSkip(int64_t count);
};

class ARTD_API_JLIB_IO InputStreamImpl 
	: public FileInputStream 
	, protected StreamBaseImpl
{
public:
	ARTD_OBJECT_DECL   

	// simple open for reading
	static ObjectPtr<InputStreamImpl> open(StringArg path);

	InputStreamImpl(HANDLE hfile=0, int flags=0);

	unsigned int    getFlags(unsigned int which);
	int             get(void);
	ptrdiff_t       read(void *b, ptrdiff_t l);
	size_t          getBytePosition();
	int             close();
	ptrdiff_t       skip(ptrdiff_t count);
};


class ARTD_API_JLIB_IO OutputStreamImpl 
	: public FileOutputStream 
	, protected StreamBaseImpl
{
public:
	ARTD_OBJECT_DECL

	// simple open for reading
	
	static ObjectPtr<OutputStreamImpl> open(StringArg path, int flags = 0);

	OutputStreamImpl(HANDLE hfile=0, int flags=0);

	unsigned int    getFlags(unsigned int which);
	int             put(unsigned char b);
	ptrdiff_t       write(const void *buf, ptrdiff_t len);
	size_t          getBytePosition();
	int             close();
	int             flush();
};

class ARTD_API_JLIB_IO RandomAccessFileImpl
	: public RandomAccessFile
	, protected InputStream
	, protected OutputStream
	, protected StreamBaseImpl
{
public:

	ObjectBase *myObjectBase() {
		return(static_cast<RandomAccessFile*>(this));
	}

	// simple open for reading
	static ObjectPtr<RandomAccessFileImpl> open(StringArg path, const char *mode);

	RandomAccessFileImpl(HANDLE hfile=0, int flags=0);

	unsigned int    getFlags(unsigned int which);
	int             get(void);
	ptrdiff_t       read(void *b, ptrdiff_t l);
	size_t          getBytePosition();
	int             close();
	ptrdiff_t       skip(ptrdiff_t count);
	int             put(unsigned char b);
	ptrdiff_t       write(const void *buf, ptrdiff_t len);
	int             flush();

	// Random AccessFile methods
	InputStream *getInputStream() { return(this); }
	OutputStream *getOutputStream() { return(this); }
	void *	getFD() { return(void*)(size_t)(hfile_); }
	int64_t length();
	int		setLength(int64_t newlength);
	int		seek(int64_t absoluteByteOffset);
	int		skipBytes(int64_t numBytes);	
	int64_t getFilePointer();
};

#ifdef _MSC_VER
	#pragma warning(default : 4275) // non export interface but root is export
#endif

#ifndef WINAPI
	#undef HANDLE
#endif

#undef INL

ARTD_END

#endif // __artd_StreamsImpl_h
