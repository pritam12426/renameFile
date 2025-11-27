#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_


#include <filesystem>

namespace fs = std::filesystem;

namespace Fun {


bool formatPathName(fs::path &newString);

}  // namespace Fun


#endif  // _FUNCTIONS_HPP_
