#!/bin/bash

/usr/local/bin/protoc --proto_path=. \
    --grpc_out=. \
    --cpp_out=. \
    --python_out=. \
    --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
    proto/pbpipes.proto

