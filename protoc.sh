#!/bin/bash


/usr/local/bin/protoc --proto_path=. \
    --java_out=src/main/java \
    pbpipes/proto/bigquery_schema.proto

/usr/local/bin/protoc --proto_path=. \
    --cpp_out=. \
    pbpipes/proto/bigquery_schema.proto

