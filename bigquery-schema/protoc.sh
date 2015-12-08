#!/bin/bash

/usr/local/bin/protoc --proto_path=. \
    --cpp_out=. \
    pbpipes/proto/bigquery_schema.proto

