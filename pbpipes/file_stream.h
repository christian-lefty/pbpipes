#ifndef PBPIPES_FILE_STREAM_H_
#define PBPIPES_FILE_STREAM_H_

#include <boost/filesystem/fstream.hpp>
#include <google/protobuf/wrappers.pb.h>

namespace pbpipes {
class FileStream : public StreamImpl<std::nullptr_t, google::protobuf::StringValue> {
 public:
  explicit FileStream(const std::string& file);

 private:
  boost::filesystem::ifstream in_;
};
}  // namespace pbpipes

#endif  // PBPIPES_FILE_STREAM_H_

