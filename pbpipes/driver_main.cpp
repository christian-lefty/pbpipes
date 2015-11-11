#include <vector>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <pbpipes/driver.h>
#include <pbpipes/hbase_stream.h>

using pbpipes::Driver;
using pbpipes::HBaseSplit;
using pbpipes::HBaseStream;

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  Driver driver;
  CHECK(driver.Init("driver.conf"));
  CHECK(driver.StartExecutors());

  LOG(INFO) << "reading hbase.";
  HBaseStream stream("lefty-azure-master", 9090);
  const std::vector<HBaseSplit> splits = stream.SplitRegions();

  for (int i = 0; i < splits.size(); ++i) {
    driver.ProcessSplit(splits[i]);
  }

  return 0;
}

