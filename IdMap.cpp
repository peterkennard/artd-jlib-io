#include "artd/IdMap.h"
#include "artd/RcString.h"

ARTD_BEGIN


class EmptyIdMap
	: public IdMap 
{
public:

	RcString emptyString;

	EmptyIdMap() {
		emptyString = "";
	}
	
	RcString get(int id) {
		return(emptyString);
	}
    int get(StringArg name) {
		return(-1);
	}

};

static EmptyIdMap emptyMap;

IdMap &IdMap::emptyInstance = emptyMap; 	


ARTD_END

