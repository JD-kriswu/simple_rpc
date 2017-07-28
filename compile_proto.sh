#!/bin/sh
PROTOBUF_DIR=/data/norvallu/protobuf-2.4.1
PROTOC=$PROTOBUF_DIR/src/protoc
PROTO_INCLUDE=$PROTOBUF_DIR/include

$PROTOC --proto_path=. --proto_path=$PROTO_INCLUDE --cpp_out=. \
rpc_meta.proto rpc_option.proto builtin_service.proto 

