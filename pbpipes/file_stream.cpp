#include <pbpipes/file_stream.h>

#include <boost/filesystem/path.hpp>
#include <glog/logging.h>

namespace pbpipes {
FileStream::FileStream(const std::string& file) {
  boost::filesystem::path p(file);
  CHECK(boost::filesystem::exists(p)) << file << " doesn't exist.";
  in_.open(p, );
}
}  // namespace pbpipes

