#ifndef __artd_ByteArrayOutputStream_h
#define __artd_ByteArrayOutputStream_h

/*-
 * Copyright (c) 1985-2022 Peter Kennard and aRt&D Lab
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
 */

#include "artd/jlib_io.h"
#include "artd/OutputStream.h"
#include "artd/RcArray.h"
#include <string.h>

ARTD_BEGIN

class ARTD_API_JLIB_IO ByteArrayOutputStream
    : public OutputStream
{
    ByteArray owned_;
    unsigned char *buff_;
	unsigned char *p_;
	unsigned char *max_;

	inline size_t available() {
		return(max_ - p_);
	}
public:

	ARTD_OBJECT_DECL

    ByteArrayOutputStream(void *buffer, size_t size) {
        buff_ = (uint8_t *)buffer;
        max_ = buff_ + size;
        reset();
    }
    ByteArrayOutputStream(ByteArray buffer) {

        if (!buffer) {
            return;  // TODO: handle no buffer 
        }
        buff_ = (uint8_t*)buffer->data();
        max_ =  buff_ + buffer->dataSize();
        reset();
        owned_ = buffer;
    }
	~ByteArrayOutputStream() {
        // TODO: handle no buffer 
	}

	unsigned int getFlags(unsigned int toGet) {
		return(0);
	}
	int close() {
		return(0);
	}
	size_t getBytePosition() {
		return(p_ - buff_);
	}
	// same as setSize(0);
	void reset() {
		p_ = buff_;
	}
	// get the buffer
	inline uint8_t *getBuffer() {
		return(buff_);
	}

	// set size of filled area - TODO: will leave garbage in buffer if beyond fill area
	void setSize(size_t size) {
		
		size_t capacity = max_ - buff_;
		if (size > capacity) {
			size = capacity;
		}
		p_ = buff_ + size;
	}

	int put(unsigned char b) {
		if (p_ >= max_) {
			return(-1);
		}
		*p_++ = b;
		return(0);
	}
	ptrdiff_t write(const void *buf, ptrdiff_t len) {
		if (len >= 0 && ((ptrdiff_t)available()) >= len) {
			::memcpy(p_, buf, len);
			p_ += len;
			return(len);
		}
		return(-1); // just for now no reallocation.
	}
	int flush() {
		return(0);
	}
};

ARTD_END

#endif // __artd_ByteArrayOutputStream_h
