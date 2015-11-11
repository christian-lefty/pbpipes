#ifndef PBPIPES_EXECUTOR_H_
#define PBPIPES_EXECUTOR_H_

#include <grpc++/grpc++.h>

#include <proto/pbpipes.grpc.pb.h>
#include <proto/pbpipes.pb.h>

namespace pbpipes {
class Executor : public proto::Executor::Service {
 public:
  Executor();
  ~Executor() override;

  grpc::Status PostTask(grpc::ServerContext* ctx,
                        const proto::TaskInfo* task,
                        proto::TaskResult* tr) override;

 private:

};
}  // pbpipes

#endif  // PBPIPES_EXECUTOR_H_

