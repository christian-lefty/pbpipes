#include <pbpipes/hbase_stream.h>

#include <boost/shared_ptr.hpp>
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <hbase/Hbase.h>

using ::apache::hadoop::hbase::thrift::HbaseClient;
using ::apache::hadoop::hbase::thrift::TRegionInfo;
using ::apache::thrift::protocol::TBinaryProtocol;
using ::apache::thrift::protocol::TProtocol;
using ::apache::thrift::transport::TFramedTransport;
using ::apache::thrift::transport::TSocket;
using ::apache::thrift::transport::TTransport;

namespace pbpipes {
namespace {
proto::HBaseRegionInfo ToProtoRegionInfo(const TRegionInfo& i) {
  proto::HBaseRegionInfo region;
  region.set_start_key(i.startKey);
  region.set_end_key(i.endKey);
  region.set_id(i.id);
  region.set_name(i.name);
  region.set_version(i.version);
  region.set_server_name(i.serverName);
  region.set_port(i.port);

  return region;
}
}  // namespace

HBaseSplit::HBaseSplit(proto::HBaseRegionInfo region) {
  region_.Swap(&region);
}

HBaseStream::HBaseStream(std::string host, int port) : port_(port) {
  host_.swap(host);
}

std::vector<HBaseSplit> HBaseStream::SplitRegions() {
  const int64_t kHBaseTimeout = 30000L;
  boost::shared_ptr<TSocket> socket(new TSocket(host_, port_));
  socket->setConnTimeout(kHBaseTimeout);
  socket->setRecvTimeout(kHBaseTimeout);
  socket->setSendTimeout(kHBaseTimeout);
  boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  HbaseClient hbase(protocol);
  transport->open();

  std::vector<TRegionInfo> regions;
  hbase.getTableRegions(regions, "lefty:images");

  LOG(INFO) << "found: " << regions.size() << " hbase regions.";

  std::vector<HBaseSplit> splits;
  splits.reserve(regions.size());
  for (const auto& region : regions) {
    splits.emplace_back(ToProtoRegionInfo(region));
  }

  transport->close();

  return splits;
}
}  // namespace pbpipes

