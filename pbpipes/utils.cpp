#include <pbpipes/utils.h>

#include <sstream>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <glog/logging.h>

namespace pbpipes {
std::string ReadTextFromFileOrDie(const boost::filesystem::path& file) {
  boost::filesystem::ifstream in;
  in.open(file, std::ios::in);
  CHECK(in) << "couldn't open: " << file.string();

  std::stringstream out;
  out << in.rdbuf();
  in.close();
  return out.str();
}
}  // namespace pbpipes

