#ifndef __artd_RandomAccessRecord_h
#define __artd_RandomAccessRecord_h

#include "artd/jlib_io.h"
#include "artd/InputStream.h" 
#include "artd/OutputStream.h" 

ARTD_BEGIN

class ARTD_API_JLIB_IO RandomAccessRecord
	: public Stream
{
public:

	virtual ObjectPtr<InputStream> getInputStream() = 0;
	virtual ObjectPtr<OutputStream> getOutputStream() = 0;

	// returns < 0 if error
	virtual int64_t	length() = 0;
	// returns < 0 if error
	virtual int		setLength(int64_t newlength) = 0;
	// returns < 0 if error
	virtual int		seek(int64_t absoluteByteOffset) = 0;
	// returns < 0 if error
	virtual int		skipBytes(int64_t numBytes) = 0;
};

ARTD_END

#endif // __artd_RandomAccessRecord_h
