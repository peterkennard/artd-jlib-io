#pragma once
#include "jlib_io.h"
#include "artd/RcString.h"
#include <filesystem>

ARTD_BEGIN

#define INL ARTD_ALWAYS_INLINE

class ARTD_API_JLIB_IO JPath
    : public std::filesystem::path
{
    typedef std::filesystem::path super;
public:
    INL JPath() {}
    INL JPath(const super &p) : super(p) {
    }
    INL explicit JPath(StringArg s) : super(s.c_str(), generic_format) {
    }
    JPath(const StringArg parent, const StringArg &child);
    
    INL JPath &operator=(const super &p) {
        super::operator=(p);
        return(*this);
    }
    JPath getCanonicalPath() const;
    bool exists() const;
    bool isDirectory() const;
    bool isFile() const;

};


#undef INL

ARTD_END
