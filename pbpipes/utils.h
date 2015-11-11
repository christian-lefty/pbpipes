#ifndef PBPIPES_UTILS_H_
#define PBPIPES_UTILS_H_

#include <string>

#include <boost/filesystem/path.hpp>

namespace pbpipes {
std::string ReadTextFromFileOrDie(const boost::filesystem::path& file);

}  // namespace pbpipes

#endif  // PBPIPES_UTILS_H_

