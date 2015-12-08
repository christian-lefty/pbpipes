#include <glog/logging.h>
#include <google/protobuf/compiler/plugin.h>

#include <pbpipes/bigquery_schema_generator.h>

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  VLOG(1) << "running biquery generator...";
  pbpipes::BigQuerySchemaGenerator gen;
  return google::protobuf::compiler::PluginMain(argc, argv, &gen);
}

