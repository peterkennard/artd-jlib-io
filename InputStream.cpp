#include "artd/InputStream.h"

ARTD_BEGIN


ptrdiff_t 
InputStream::_read(void *buf, ptrdiff_t len)
{
	uint8_t *p = (uint8_t *)buf;
	uint8_t *end = p + len;	
	while(p < end)
	{    
		int c = _get();
		if(c < 0) {
			if(p == (uint8_t *)buf) {
				return(c);
			}
			break;
		}
		*p++ = (uint8_t)c;
	}
	return(p - (uint8_t *)buf);
}
ptrdiff_t 
InputStream::_skip(ptrdiff_t bytes)
{
	uint8_t buf[512];
	ptrdiff_t outb = 0;

	while(outb < bytes)
	{    
		ptrdiff_t skipb = (bytes-outb);
		if(skipb > (ptrdiff_t)sizeof(buf))
			skipb = sizeof(buf);
		skipb = read(buf,skipb);
		if(skipb <= 0) {
			if(outb == 0)
				outb = skipb;
			break;
		}
		outb += skipb;
	}
	return(outb);
}
int 
InputStream::_get()
{
	uint8_t c;
	if(read(&c,1) == 1)
		return(c);
	else
		return(-1);
}

ARTD_END
