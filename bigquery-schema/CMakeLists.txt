project(bigquery-schema)
cmake_minimum_required(VERSION 2.8)

include_directories(.)

add_definitions(
  -std=c++11
  -Wall
)

add_library(bigquery-schema_proto_lib
  # Generate the option for table name.
  # Note that the files are generated running protoc.sh
  ${PROJECT_SOURCE_DIR}/pbpipes/proto/bigquery_schema.pb.cc
)

target_link_libraries(bigquery-schema_proto_lib
  protobuf
)

add_library(bigquery-schema_lib
  ${PROJECT_SOURCE_DIR}/pbpipes/bigquery_schema_generator.cc
)

target_link_libraries(bigquery-schema_lib
  bigquery-schema_proto_lib

  jsoncpp
  protobuf
  protoc
)

add_executable(protoc-gen-bigquery-schema
  ${PROJECT_SOURCE_DIR}/pbpipes/protoc-gen-bigquery-schema.cc
)

target_link_libraries(protoc-gen-bigquery-schema
  bigquery-schema_proto_lib
  bigquery-schema_lib

  gflags
  glog
)

