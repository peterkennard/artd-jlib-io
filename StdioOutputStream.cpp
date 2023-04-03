#include "artd/StdioOutputStream.h"

#ifdef ARTD_WINDOWS
	#include <io.h>
#else
  #include <unistd.h>
  #ifndef ARTD_IOS
	#include <sys/uio.h>
  #endif
#endif

ARTD_BEGIN

StdioOutputStream::StdioOutputStream(FILE *file, bool ownsFile)
{
	flags_ = fIsStdioStream;
	if(ownsFile) {
		flags_ |= fOwnsFILE;
	}
	file_ = file;
	if(isatty(::fileno(file))) {
		flags_ |= ISATTY;
	}
}
StdioOutputStream::~StdioOutputStream()
{
	close();
}
int
StdioOutputStream::close()
{
	if((flags_ & fOwnsFILE) && file_) {
		::fclose(file_);
	}
	file_ = 0;
	return(0);
}
int         
StdioOutputStream::put(unsigned char b)
{
	int ret = (int)::fwrite(&b,1,1,file_);
	if(ret <= 0) {
		return(-1);
	}
	return(1);
}
ptrdiff_t   
StdioOutputStream::write(const void *buf, ptrdiff_t len)
{
	size_t ret = ::fwrite(buf,1,len,file_);
	return(ret); // TODO: handle errors
}
int 	    
StdioOutputStream::flush()
{
	if(::fflush(file_) == 0) {
		return(0);
	}
	return(-1);
}
unsigned int 
StdioOutputStream::getFlags(unsigned int toGet)
{
	return(flags_ & toGet);
}
size_t
StdioOutputStream::getBytePosition()
{
	return(::ftell(file_));
}


ARTD_END
