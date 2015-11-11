#include <memory>
#include <string>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <grpc++/grpc++.h>

#include <pbpipes/executor.h>
#include <proto/pbpipes.grpc.pb.h>
#include <proto/pbpipes.pb.h>

DEFINE_int32(port, 5001, "Port to listen on.");

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  std::unique_ptr<pbpipes::Executor> executor(new pbpipes::Executor());

  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:" + std::to_string(FLAGS_port),
                           grpc::InsecureServerCredentials());
  builder.RegisterService(executor.get());
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  LOG(INFO) << "executor is taking the stage on port: " << FLAGS_port;
  server->Wait();
  return 0;
}

