#ifndef PBPIPES_HBASE_STREAM_H_
#define PBPIPES_HBASE_STREAM_H_

#include <string>
#include <vector>

#include <proto/pbpipes.pb.h>

namespace pbpipes {
class HBaseSplit {
 public:
  explicit HBaseSplit(proto::HBaseRegionInfo info);

  const proto::HBaseRegionInfo& region() const { return region_; }

 private:
  proto::HBaseRegionInfo region_;
};

class HBaseStream {
 public:
  HBaseStream(std::string host, int port);

  std::vector<HBaseSplit> SplitRegions();

 private:
  std::string host_;
  int port_{-1};
};

class RegionStream {
 public:
  
 private:
  std::shared_ptr<> ;
};
}  // namespace pbpipes

#endif  // PBPIPES_HBASE_STREAM_H_

