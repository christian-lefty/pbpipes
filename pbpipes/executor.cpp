#include <pbpipes/executor.h>

#include <glog/logging.h>

namespace pbpipes {
Executor::Executor() {
}

Executor::~Executor() {
}

grpc::Status Executor::PostTask(grpc::ServerContext* ctx,
                                const proto::TaskInfo* task,
                                proto::TaskResult* tr) {
  LOG(INFO) << "processing task: " << task->index() << "/" << task->total();
  const proto::HBaseRegionInfo& info = task->region();


  return grpc::Status::OK;
}
}  // pbpipes

