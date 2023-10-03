#include "artd/JPath.h"
#include "artd/Logger.h"

ARTD_BEGIN

JPath::JPath(const StringArg parent, const StringArg &child) {

}
JPath
JPath::getCanonicalPath() const {
    return(std::filesystem::canonical( *this ));
}
bool
JPath::exists() const {
    return(std::filesystem::exists( *this ));
}
bool
JPath::isDirectory() const {
    return(std::filesystem::is_directory( *this ));
}
bool
JPath::isFile() const {
    return(std::filesystem::is_regular_file( *this ));
}


ARTD_END
