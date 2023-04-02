#ifndef __artd_jlib_io_h
#define __artd_jlib_io_h

#include "artd/jlib_base.h"

#ifdef BUILDING_artd_jlib_io
	#define ARTD_API_JLIB_IO ARTD_SHARED_LIBRARY_EXPORT
#else
	#define ARTD_API_JLIB_IO ARTD_SHARED_LIBRARY_IMPORT
#endif

#endif // __artd_jlib_io_h
