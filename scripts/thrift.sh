#!/bin/bash

thrift -out hbase -gen cpp scripts/Hbase.thrift
# This is just an example and is useless for us.
rm hbase/Hbase_server.skeleton.cpp

