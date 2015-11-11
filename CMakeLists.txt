project(pbpipes)
cmake_minimum_required(VERSION 2.8)

find_package(Boost 1.55 COMPONENTS thread program_options system filesystem REQUIRED)
include_directories(.)

SET(CMAKE_CXX_FLAGS "-O3")
add_definitions(
  -std=c++11
)

add_library(hbase_lib
  ${PROJECT_SOURCE_DIR}/hbase/Hbase.cpp
  ${PROJECT_SOURCE_DIR}/hbase/Hbase_constants.cpp
  ${PROJECT_SOURCE_DIR}/hbase/Hbase_types.cpp
)

target_link_libraries(hbase_lib
  thrift
)

add_library(proto_lib
  ${PROJECT_SOURCE_DIR}/proto/pbpipes.grpc.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/pbpipes.pb.cc
)

target_link_libraries(proto_lib
  grpc++_unsecure
  grpc
  protobuf
)

add_library(pbpipes_lib
  ${PROJECT_SOURCE_DIR}/pbpipes/driver.cpp
  ${PROJECT_SOURCE_DIR}/pbpipes/executor.cpp  
  ${PROJECT_SOURCE_DIR}/pbpipes/hbase_stream.cpp
  ${PROJECT_SOURCE_DIR}/pbpipes/utils.cpp
)

target_link_libraries(pbpipes_lib
  hbase_lib
  proto_lib
  glog
  grpc++_unsecure
  grpc
  gpr
  protobuf
  ${Boost_LIBRARIES}
)

add_executable(driver_main
  ${PROJECT_SOURCE_DIR}/pbpipes/driver_main.cpp
)

target_link_libraries(driver_main
  pbpipes_lib
  gflags
  glog
)

add_executable(executor_main
  ${PROJECT_SOURCE_DIR}/pbpipes/executor_main.cpp
)

target_link_libraries(executor_main
  pbpipes_lib
  gflags
  glog
)

