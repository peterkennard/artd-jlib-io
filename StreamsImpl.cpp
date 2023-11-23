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
 * 	$Id$ */

#ifdef ARTD_WINDOWS
	#pragma warning (disable : 4275)
#endif

#include "artd/jlib_io.h"
#include "artd/StreamsImpl.h"
#include <new>
#include <string>
#include "artd/cstring_util.h"

#ifdef ARTD_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		/// &brief Exclude rarely-used stuff from Windows headers
#endif
#include "Windows.h"

ARTD_BEGIN

StreamBaseImpl::~StreamBaseImpl()
{
	doClose();
}
unsigned int StreamBaseImpl::doGetFlags(unsigned int which)
{
	unsigned int f = (which & Stream::fIsFdStream);
	if((which & Stream::ISATTY) && (::GetFileType(hfile_) == FILE_TYPE_CHAR)) {
		f |= Stream::ISATTY;
	}
	return(f);
}
inline int StreamBaseImpl::getbyte()
{
	unsigned char b = 0;
	DWORD done = 0;
	if((!::ReadFile(hfile_,&b,1,&done,NULL)) || (done != 1)) {
		return(-1);
	}
	return(b);
}
inline int StreamBaseImpl::putbyte(unsigned char b)
{
	DWORD done;
	if(!::WriteFile(hfile_,&b,1,&done,NULL)) {
		return(-1);
	}
	return(0);
}
inline int StreamBaseImpl::doClose()
{
	if(flags_ & fOwnsHandle) {
		if (hfile_)
		{
			::CloseHandle(hfile_);
			hfile_ = 0;
		}
		flags_ &= ~fOwnsHandle;
	}
	return(0);
}
inline ptrdiff_t StreamBaseImpl::doWrite(const void *b, ptrdiff_t len)
{
#ifdef _WIN64
	ptrdiff_t done = 0;
	ptrdiff_t totalWritten = 0;
	char *buf = (char *) b;
	while(len > 0)
	{
		DWORD bytes;
		DWORD writeThis = len > 0x7FFFFFFF ? 0x7FFFFFFF : (DWORD)len;
		if(!::WriteFile(hfile_,buf, writeThis, &bytes,NULL))
			return totalWritten;
		buf += writeThis;
		totalWritten += writeThis;
		len -= writeThis;
	}
	return(totalWritten);
#else

	#pragma warning(disable : 4244) // conversion from 'ptrdiff_t' to 'DWORD', possible loss of data

	DWORD done;
	if(!::WriteFile(hfile_,b,(DWORD)len,&done,NULL)) {
		return(-1);
	}
	return(done);

	#pragma warning(default : 4244)

#endif
}
inline ptrdiff_t StreamBaseImpl::doRead(void *b, ptrdiff_t len)
{
#ifdef _WIN64
	ptrdiff_t done = 0;
	ptrdiff_t totalRead = 0;
	char *buf = (char *) b;
	while(len > 0)
	{
		DWORD bytes;
		DWORD readThis = len > 0x7FFFFFFF ? 0x7FFFFFFF : (DWORD)len;
		if(!::ReadFile(hfile_,b, readThis, &bytes,NULL))
			return totalRead;
		buf += readThis;
		totalRead += readThis;
		len -= readThis;
	}
	return(totalRead);
#else
	#pragma warning(disable : 4244) // conversion from 'ptrdiff_t' to 'DWORD', possible loss of data

	DWORD done;
	if(!::ReadFile(hfile_,b,len,&done,NULL)) {
		return(-1);
	}
	return(done);

	#pragma warning(default : 4244)
#endif
}
inline int64_t StreamBaseImpl::doGetBytePosition()
{
	LARGE_INTEGER ret;
	ret.QuadPart = 0;
	if(!::SetFilePointerEx(hfile_,ret,&ret,FILE_CURRENT)) {
		return(-1);
	}
	return(ret.QuadPart);
	/*
	DWORD ret = ::SetFilePointer(hfile_,0,0,FILE_CURRENT);
	if(ret == 0xFFFFFFFF) {
		return(-1);
	}
	return(ret);
	*/
}
inline int StreamBaseImpl::doSkip(int64_t count)
{
#ifndef _WIN64
	#pragma warning(disable : 4244) // conversion from 'ptrdiff_t' to 'DWORD', possible loss of data
#endif

	LARGE_INTEGER &ret = *(LARGE_INTEGER *)&count;
	if(!::SetFilePointerEx(hfile_,ret,0,FILE_CURRENT)) {
		return(-1);
	}
	return(0);

#ifndef _WIN64
	#pragma warning(default : 4244)
#endif
}
inline int StreamBaseImpl::doFlush()
{
	if(!::FlushFileBuffers(hfile_)) {
		return(-1);
	}
	return(0);
}
ObjectPtr<InputStreamImpl> InputStreamImpl::open(StringArg path)
{
	HANDLE hfile = ::CreateFileA( path.c_str(),
							 GENERIC_READ,
							 FILE_SHARE_READ,
							 NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL,
							 NULL);
	if(hfile != INVALID_HANDLE_VALUE) {
		return(ObjectPtr<InputStreamImpl>::make(hfile,fOwnsHandle));
	}
	return(0);
}

InputStreamImpl::InputStreamImpl(HANDLE hfile, int flags)
{
	flags_ = (flags & fOwnsHandle);
	hfile_ = hfile;
}
unsigned int InputStreamImpl::getFlags(unsigned int which)
{
	return(doGetFlags(which));
}
int InputStreamImpl::get()
{
	return(getbyte());
}
ptrdiff_t InputStreamImpl::read(void *b, ptrdiff_t len)
{
	return(doRead(b,len));
}
size_t InputStreamImpl::getBytePosition()
{
	return((size_t)doGetBytePosition());
}
ptrdiff_t InputStreamImpl::skip(ptrdiff_t count)
{
	return((size_t)doSkip(count));
}
int InputStreamImpl::close()
{
	return(doClose());
}
OutputStreamImpl::OutputStreamImpl(HANDLE hfile, int flags)
{
	flags_ = (flags & fOwnsHandle);
	hfile_ = hfile;
}
ObjectPtr<OutputStreamImpl>
OutputStreamImpl::open(StringArg path, int flags)
{
	int mode = CREATE_ALWAYS;
	if(flags & fOPEN_APPEND) {
		mode = OPEN_ALWAYS;
	}
	HANDLE hfile = ::CreateFileA( path.c_str(),
							 GENERIC_WRITE|GENERIC_READ,
							 FILE_SHARE_WRITE|FILE_SHARE_READ,
							 NULL,
							 mode,
							 FILE_ATTRIBUTE_NORMAL,
							 NULL);

	if(hfile != INVALID_HANDLE_VALUE)
	{
		if(flags & fOPEN_APPEND) {
			::SetFilePointer(hfile,0,0,FILE_END);
		}
		return(ObjectPtr<OutputStreamImpl>::make(hfile,fOwnsHandle));
	}
	return(0);
}
unsigned int OutputStreamImpl::getFlags(unsigned int which)
{
	return(doGetFlags(which));
}
int OutputStreamImpl::put(unsigned char b)
{
	return(putbyte(b));
}
ptrdiff_t OutputStreamImpl::write(const void *b, ptrdiff_t len)
{
	return(doWrite(b,len));
}
int OutputStreamImpl::close()
{
	return(doClose());
}
size_t OutputStreamImpl::getBytePosition()
{
	return((size_t)doGetBytePosition());
}
int OutputStreamImpl::flush()
{
	return(doFlush());
}

ObjectPtr<RandomAccessFileImpl> RandomAccessFileImpl::open(StringArg path, const char *mode)
{
	int fmode = OPEN_ALWAYS;

	if(mode[0] != 'r') {
		return(0);
	}
	int rwmode = GENERIC_READ;
	if(mode[1] == 'w') {
		rwmode = GENERIC_WRITE|GENERIC_READ;
	} else if(mode[1] != 0) {
		return(0);
	}

	HANDLE hfile = ::CreateFileA( path.c_str(), rwmode,
							 FILE_SHARE_READ | FILE_SHARE_WRITE,
							 NULL,
							 fmode,
							 FILE_ATTRIBUTE_NORMAL,
							 NULL);
	if(hfile != INVALID_HANDLE_VALUE) {
		return(ObjectPtr<RandomAccessFileImpl>::make(hfile,fOwnsHandle));
	}
	return(nullptr);
}
RandomAccessFileImpl::RandomAccessFileImpl(HANDLE hfile, int flags)
{
	flags_ = (flags & fOwnsHandle);
	hfile_ = hfile;
}

unsigned int
RandomAccessFileImpl::getFlags(unsigned int which)
{
	return(doGetFlags(which));
}
int
RandomAccessFileImpl::get()
{
	return(getbyte());
}
ptrdiff_t
RandomAccessFileImpl::read(void *b, ptrdiff_t len)
{
	return(doRead(b,len));
}
size_t
RandomAccessFileImpl::getBytePosition()
{
	return((size_t)doGetBytePosition());
}
int64_t
RandomAccessFileImpl::getFilePointer()
{
	return(doGetBytePosition());
}
ptrdiff_t
RandomAccessFileImpl::skip(ptrdiff_t count)
{
	return(doSkip(count));
}
int
RandomAccessFileImpl::close()
{
	return(doClose());
}
int
RandomAccessFileImpl::put(unsigned char b)
{
	return(putbyte(b));
}
ptrdiff_t
RandomAccessFileImpl::write(const void *b, ptrdiff_t len)
{
	return(doWrite(b,len));
}
int
RandomAccessFileImpl::flush()
{
	return(doFlush());
}
int64_t
RandomAccessFileImpl::length()
{
	LARGE_INTEGER ret;
	ret.QuadPart = 0;
	if(!::SetFilePointerEx(hfile_,ret,&ret,FILE_END)) {
		return(-1);
	}
	return(ret.QuadPart);
}
int
RandomAccessFileImpl::setLength(int64_t newlength)
{
	LARGE_INTEGER &li = *(LARGE_INTEGER *)&newlength;
	if(!::SetFilePointerEx(hfile_,li,&li,FILE_BEGIN)) {
		return(-1);
	}
	if(!::SetEndOfFile(hfile_)) {
		return(-1);
	}
	return(0);
}
int
RandomAccessFileImpl::seek(int64_t absoluteByteOffset)
{
	LARGE_INTEGER &li = *(LARGE_INTEGER *)&absoluteByteOffset;
	if(!::SetFilePointerEx(hfile_,li,&li,FILE_BEGIN)) {
		return(-1);
	}
	return(0);
}
int
RandomAccessFileImpl::skipBytes(int64_t numBytes)
{
	return(doSkip(numBytes));
}


ARTD_END

#else

#ifndef _POSIX_SOURCE
	#define _POSIX_SOURCE
#endif

#ifndef ARTD_IOS
	#include <sys/uio.h>
#endif

#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <new>

ARTD_BEGIN

StreamBaseImpl::~StreamBaseImpl()
{
	doClose();
}
unsigned int StreamBaseImpl::doGetFlags(unsigned int which)
{
unsigned int f = 0;

	if((which & Stream::ISATTY) && isatty(hfile_))
		f |= Stream::ISATTY;
	return(f);
}
inline int StreamBaseImpl::getbyte()
{
unsigned char b;

	if(::read(hfile_,&b,1) <= 0)
		return(-1);
	return(b);
}
inline int StreamBaseImpl::putbyte(unsigned char b)
{
	if(::write(hfile_,&b,1) <= 0)
		return(-1);
	return(0);
}
inline int StreamBaseImpl::doClose()
{
	if((flags_ & fOwnsHandle) && hfile_ != -1) {
		::close(hfile_);
		hfile_ = -1;
		flags_ = 0;
	}
	return(0);
}
inline ptrdiff_t StreamBaseImpl::doWrite(const void *b, ptrdiff_t len)
{
	if(::write(hfile_,b,(int)len) <= 0)
		return(-1); // error	return(-1);
	return(len);
}
inline ptrdiff_t StreamBaseImpl::doRead(void *b, ptrdiff_t len)
{
	for(;;)
	{
		ptrdiff_t ret = ::read(hfile_,b,(int)len);
		if(ret <= 0) {
			return(-1); // error ?
		}
		return(ret);
	}
}
inline int64_t StreamBaseImpl::doGetBytePosition()
{
	return(lseek(hfile_,0,SEEK_CUR));
}
inline int StreamBaseImpl::doSkip(int64_t count)
{
	return(lseek(hfile_,(long)count,SEEK_CUR) >= (long)count);
}
inline int StreamBaseImpl::doFlush()
{
	fsync(hfile_);
	return(0);
}


ObjectPtr<InputStreamImpl>
InputStreamImpl::open(StringArg path)
{
	int file;
	file = ::open(path.c_str(),O_RDONLY);
	if(file <= 0)
		return(0);
	return(ObjectPtr<InputStreamImpl>::make(file,fOwnsHandle));
}
InputStreamImpl::InputStreamImpl(int hfile, int flags)
{
	flags_ = flags & fOwnsHandle;
	hfile_ = hfile;
}
unsigned int InputStreamImpl::getFlags(unsigned int which)
{
	return(doGetFlags(which));
}
int InputStreamImpl::get()
{
	return(getbyte());
}
ptrdiff_t InputStreamImpl::read(void *b, ptrdiff_t len)
{
	return(doRead(b,len));
}
size_t InputStreamImpl::getBytePosition()
{
	return(doGetBytePosition());
}
ptrdiff_t InputStreamImpl::skip(ptrdiff_t count)
{
	return(doSkip(count));
}
int InputStreamImpl::close()
{
	return(doClose());
}

OutputStreamImpl::OutputStreamImpl(int hfile, int flags)
{
	flags_ = flags & fOwnsHandle;
	hfile_ = hfile;
}
ObjectPtr<OutputStreamImpl>
OutputStreamImpl::open(StringArg path, int flags)
{
	int file;
	int fmode = O_CREAT | O_RDWR;
	fmode |= O_CREAT;
	if (flags & fOPEN_APPEND)
		fmode |= O_APPEND;
	file = ::open(path.c_str(),fmode,0666);
	if(file <= 0)
		return(0);
	return(ObjectPtr<OutputStreamImpl>::make(file,fOwnsHandle));
}
unsigned int OutputStreamImpl::getFlags(unsigned int which)
{
	return(doGetFlags(which));
}
int OutputStreamImpl::put(unsigned char b)
{
	return(putbyte(b));
}
ptrdiff_t OutputStreamImpl::write(const void *b, ptrdiff_t len)
{
	return(doWrite(b,len));
}
int OutputStreamImpl::close()
{
	return(doClose());
}
size_t OutputStreamImpl::getBytePosition()
{
	return(doGetBytePosition());
}
int OutputStreamImpl::flush()
{
	return(doFlush());
}


ObjectPtr<RandomAccessFileImpl> RandomAccessFileImpl::open(StringArg path, const char *mode)
{
	int file;
	if(mode[0] != 'r') {
		return(0);
	}
	int fmode = 0;
	if(mode[1] == 'w') {
		fmode |= O_RDWR;
	} else if(mode[1] != 0) {
		return(0);
	} else {
		fmode |= O_RDONLY;
	}
	file = ::open(path.c_str(),fmode);
	if(file <= 0)
		return(0);
	return(ObjectPtr<RandomAccessFileImpl>::make(file,fOwnsHandle));
}
RandomAccessFileImpl::RandomAccessFileImpl(int hfile, int flags)
{
	flags_ = (flags & fOwnsHandle);
	hfile_ = hfile;
}

unsigned int
RandomAccessFileImpl::getFlags(unsigned int which)
{
	return(doGetFlags(which));
}
int
RandomAccessFileImpl::get()
{
	return(getbyte());
}
ptrdiff_t
RandomAccessFileImpl::read(void *b, ptrdiff_t len)
{
	return(doRead(b,len));
}
size_t
RandomAccessFileImpl::getBytePosition()
{
	return((size_t)doGetBytePosition());
}
int64_t
RandomAccessFileImpl::getFilePointer()
{
	return(doGetBytePosition());
}
ptrdiff_t
RandomAccessFileImpl::skip(ptrdiff_t count)
{
	return(doSkip(count));
}
int
RandomAccessFileImpl::close()
{
	return(doClose());
}
int
RandomAccessFileImpl::put(unsigned char b)
{
	return(putbyte(b));
}
ptrdiff_t
RandomAccessFileImpl::write(const void *b, ptrdiff_t len)
{
	return(doWrite(b,len));
}
int
RandomAccessFileImpl::flush()
{
	return(doFlush());
}
int64_t
RandomAccessFileImpl::length()
{
	return(lseek(hfile_,0,SEEK_END));
}
int64_t
RandomAccessFileImpl::setLength(int64_t newlength)
{
	return(ftruncate(hfile_, newlength));
}
int64_t
RandomAccessFileImpl::seek(int64_t absoluteByteOffset)
{
	return(lseek(hfile_,absoluteByteOffset,SEEK_SET));
}
int64_t
RandomAccessFileImpl::skipBytes(int64_t numBytes)
{
	return(doSkip(numBytes));
}

ARTD_END

#endif
