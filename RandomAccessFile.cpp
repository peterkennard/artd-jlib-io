#include "artd/RandomAccessFile.h"
#include "artd/StreamsImpl.h"

ARTD_BEGIN


ObjectPtr<RandomAccessFile> 
RandomAccessFile::open(StringArg path, const char *mode)
{
	return(RandomAccessFileImpl::open(path,mode));
}

ARTD_END
