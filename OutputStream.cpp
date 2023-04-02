#include "artd/OutputStream.h"

ARTD_BEGIN

int 
OutputStream::_put(unsigned char b)
{
	return((int)write(&b,1));
}
ptrdiff_t 
OutputStream::_write(const void *buf, ptrdiff_t len)
{
	ptrdiff_t outlen = 0;
	const uint8_t *pout = (const uint8_t *)buf;
	while(outlen < len)
	{
		int ret = put(*pout++);
		if(ret < 0) {
			if(outlen == 0) {
				return(ret);
			}
			break;
		}
		++outlen;
	}
	return(outlen);
}


ARTD_END
