#include <pbpipes/driver.h>

#include <stdio.h>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <glog/logging.h>
#include <google/protobuf/text_format.h>
#include <grpc++/grpc++.h>

#include <pbpipes/utils.h>
#include <proto/pbpipes.pb.h>

namespace pbpipes {
using google::protobuf::TextFormat;

static const int kExecutorPort = 5000;

bool Driver::Init(const std::string& file) {
  const boost::filesystem::path p(file);
  if (!boost::filesystem::exists(p)) {
    LOG(ERROR) << "conf file doesn't exist: " << file;
    return false;
  }

  const std::string conf_text = ReadTextFromFileOrDie(p);
  CHECK(TextFormat::ParseFromString(conf_text, &conf_));

  LOG(INFO) << "we have: " << conf_.executors_size()
            << (conf_.executors_size() == 1 ? " executor" : " executors");
  return true;
}

namespace {
std::string ToAddress(const std::string& host, int port) {
  return host + ":" + std::to_string(port);
}
}  // anonymous namespace

bool Driver::StartExecutors() {
  CHECK(executors_ == nullptr) << "must only start once";
  executors_.reset(new ExecutorsList());

  for (const auto& e : conf_.executors()) {
    for (int i = 0; i < e.num_executors(); ++i) {
      int port = kExecutorPort + i;
      std::string cmd = "ssh " + e.host() + " ./executor_main --logtostderr --port "
          + std::to_string(port);
      CHECK(std::system(cmd.c_str()) == 0);

      executors_->emplace_back();
      ExecutorState& state = executors_->back();
      state.chan = grpc::CreateChannel(ToAddress(e.host(), port), grpc::InsecureCredentials());
      state.stub = proto::Executor::NewStub(state.chan);
    }
  }

  return true;
}

void Driver::ProcessSplit(const HBaseSplit& split) {
  int index = 0;
  proto::Executor::Stub* stub = nullptr;
  for (int i = 0; i < executors_->size(); ++i) {
    const auto& e = (*executors_)[i];
    if (!e.used) {
      index = i;
      stub = e.stub.get();
      break;
    }
  }

  proto::TaskInfo task;
  *task.mutable_region() = split.region();

  grpc::ClientContext ctx;
  proto::TaskResult tr;
  const grpc::Status status = stub->PostTask(&ctx, task, &tr);
  CHECK(status.ok());
  LOG(INFO) << "done task";
}
}  // namespace pbpipes

