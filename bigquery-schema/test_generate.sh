#!/bin/bash

# generates a schema (this is meant to be used while hacking on the generator).

/usr/local/bin/protoc \
    --proto_path=. \
    --plugin=protoc-gen-bigquery-schema=protoc-gen-bigquery-schema.sh \
    --bigquery-schema_out=. \
    pbpipes/testdata/test.proto

