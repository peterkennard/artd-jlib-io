#ifndef __artd_IdMap_h
#define __artd_IdMap_h

#include "artd/jlib_io.h"
#include "artd/ObjectBase.h"
#include "artd/StringArg.h"

ARTD_BEGIN

class ARTD_API_JLIB_IO IdMap {
	
public:
	virtual ~IdMap() {}
	virtual RcString get(int id) = 0;
    virtual int get(StringArg name) = 0;
	
	inline IdMap *operator*() { return(this); }
	inline const IdMap *operator*() const { return(this); }
	
    static IdMap &emptyInstance; 	
};

ARTD_END

#endif // __artd_IdMap_h
