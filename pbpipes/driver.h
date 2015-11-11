#ifndef PBPIPES_DRIVER_H_
#define PBPIPES_DRIVER_H_

#include <map>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include <pbpipes/hbase_stream.h>
#include <proto/pbpipes.grpc.pb.h>
#include <proto/pbpipes.pb.h>

namespace pbpipes {
// state of a single executor.
struct ExecutorState {
  std::shared_ptr<grpc::Channel> chan;
  std::unique_ptr<proto::Executor::Stub> stub;

  // true if the executor is processing a task, false otherwise.
  bool used{false};
};

typedef std::vector<ExecutorState> ExecutorsList;

class Driver {
 public:
  bool Init(const std::string& conf);

  bool StartExecutors();

  void ProcessSplit(const HBaseSplit& split);

 private:
  proto::DriverConfig conf_;
  // list of all the executors stubs and their state.
  std::unique_ptr<ExecutorsList> executors_;
};
}  // pbpipes

#endif  // PBPIPES_DRIVER_H_

