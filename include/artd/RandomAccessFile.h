#ifndef __artd_RandomAccessFile_h
#define __artd_RandomAccessFile_h

#include "artd/RandomAccessRecord.h"
#include "artd/StringArg.h"

ARTD_BEGIN

class ARTD_API_JLIB_IO RandomAccessFile
	: public RandomAccessRecord
{
public:

	// mode must be "rw" or "r"
	static ObjectPtr<RandomAccessFile> open(StringArg path, const char *mode="rw" );

	// to be like Java
	virtual void *	getFD() = 0;
	// returns < 0 if error
	virtual int64_t	getFilePointer() = 0;
};



ARTD_END

#endif // __artd_RandomAccessFile_h
