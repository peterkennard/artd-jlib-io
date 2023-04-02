#include "artd/InputStreamReader.h"
#include "artd/utf8util.h"

ARTD_BEGIN

InputStreamReader::InputStreamReader(ObjectPtr<InputStream> is, const char *charset)
	: in_(is)
{
}

InputStreamReader::~InputStreamReader()
{
}

unsigned int
InputStreamReader::getFlags(unsigned int toGet)
{
	return(in_->getFlags(toGet));
}
int
InputStreamReader::close()
{
	return(in_->close());
}
size_t
InputStreamReader::getBytePosition()
{
	return(in_->getBytePosition());
}
int
InputStreamReader::read(void)
{
	return(in_->read());
}
int
InputStreamReader::read(wchar_t *chars, int len)
{
	// TODO: have this handle utf-8 etc
	uint8_t *bytes = (uint8_t*)chars;
	bytes += len;

	len = (int)in_->read(bytes,len);
	if(len < 0) {
		goto error;
	}
	uint8_t *bmax;
	bmax = bytes + len;
	while(bytes < bmax) {
		*chars++ = *bytes++;
	}
error:
	return(len);
}

ARTD_END
