#ifndef __artd_ByteArrayInputStream_h
#define __artd_ByteArrayInputStream_h

#include "artd/jlib_io.h"
#include "artd/InputStream.h"
#include "artd/RcArray.h"
#include <string.h>

ARTD_BEGIN

class ARTD_API_JLIB_IO ByteArrayInputStream
    : public InputStream
{
    ByteArray owned_;
    unsigned const char *buff_;
	unsigned const char *p_;
	unsigned const char *max_;

	inline size_t available() {
        return(max_ - p_);
	}
    inline void clear() {
        buff_ = max_ = p_ = 0;
    }

public:
	
	ARTD_OBJECT_DECL

	ByteArrayInputStream() {
        clear();
	}

	ByteArrayInputStream(const void *buffer, size_t size) {
		setBuffer(buffer, size);
	}
    ByteArrayInputStream(ByteArray array) {
        if (!array) {
            clear();
            return;
        }
        setBuffer(array->data(), array->dataSize());
        owned_ = array;
	}

	void setBuffer(const void *buffer, size_t size) {
		owned_ = nullptr;
		buff_ = (uint8_t *)buffer;
		max_ = buff_ + size;
		reset();
	}

	// TODO: note for now does not handle freeing or dreferencing buffer
	~ByteArrayInputStream() {
	}

	unsigned int getFlags(unsigned int toGet) {
		return(0);
	}
	int close() {
		owned_ = nullptr;
		clear();
		return(0);
	}
	size_t getBytePosition() {
		return(p_ - buff_);
	}
	void reset() {
		p_ = buff_;
	}
	int get(void) {
		if (p_ >= max_) {
			return(-1);
		}
		return(*p_++);
	}
	ptrdiff_t   read(void *buf, ptrdiff_t len) {
		if (len >= 0) {
            ptrdiff_t avail = available();
            if (avail < len) {
                if (avail <= 0) {
                    goto error;
                }
                len = avail;
            }
            ::memcpy(buf, p_, len);
            p_ += len;
            return(len);
		}
    error:
		return(-1); // end of file
	}
	ptrdiff_t   skip(ptrdiff_t bytes) {
		if (bytes >= 0) {
            ptrdiff_t avail = available();
            if (avail < bytes) {
                goto error;
            }
            p_ += bytes;
            return(bytes);
		}
    error:
        return(-1);
	}
};

ARTD_END

#endif // __artd_ByteArrayInputStream_h
