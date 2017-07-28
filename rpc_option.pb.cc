// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "rpc_option.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace tencent {
namespace pbrpc {

namespace {

const ::google::protobuf::EnumDescriptor* CompressType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_rpc_5foption_2eproto() {
  protobuf_AddDesc_rpc_5foption_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "rpc_option.proto");
  GOOGLE_CHECK(file != NULL);
  CompressType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_rpc_5foption_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_rpc_5foption_2eproto() {
}

void protobuf_AddDesc_rpc_5foption_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::protobuf_AddDesc_google_2fprotobuf_2fdescriptor_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\020rpc_option.proto\022\rtencent.pbrpc\032 googl"
    "e/protobuf/descriptor.proto*}\n\014CompressT"
    "ype\022\024\n\020CompressTypeNone\020\000\022\024\n\020CompressTyp"
    "eGzip\020\001\022\024\n\020CompressTypeZlib\020\002\022\026\n\022Compres"
    "sTypeSnappy\020\003\022\023\n\017CompressTypeLZ4\020\004:A\n\017se"
    "rvice_timeout\022\037.google.protobuf.ServiceO"
    "ptions\030\240\234\001 \001(\003:\00510000::\n\nServerType\022\037.go"
    "ogle.protobuf.ServiceOptions\030\241\234\001 \001(\t:\003UD"
    "P:6\n\004port\022\037.google.protobuf.ServiceOptio"
    "ns\030\242\234\001 \001(\005:\00545000:8\n\016method_timeout\022\036.go"
    "ogle.protobuf.MethodOptions\030\240\234\001 \001(\003:n\n\025r"
    "equest_compress_type\022\036.google.protobuf.M"
    "ethodOptions\030\241\234\001 \001(\0162\033.tencent.pbrpc.Com"
    "pressType:\020CompressTypeNone:o\n\026response_"
    "compress_type\022\036.google.protobuf.MethodOp"
    "tions\030\242\234\001 \001(\0162\033.tencent.pbrpc.CompressTy"
    "pe:\020CompressTypeNone", 660);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "rpc_option.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    &::google::protobuf::ServiceOptions::default_instance(),
    20000, 3, false, false);
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    &::google::protobuf::ServiceOptions::default_instance(),
    20001, 9, false, false);
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    &::google::protobuf::ServiceOptions::default_instance(),
    20002, 5, false, false);
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    &::google::protobuf::MethodOptions::default_instance(),
    20000, 3, false, false);
  ::google::protobuf::internal::ExtensionSet::RegisterEnumExtension(
    &::google::protobuf::MethodOptions::default_instance(),
    20001, 14, false, false,
    &tencent::pbrpc::CompressType_IsValid);
  ::google::protobuf::internal::ExtensionSet::RegisterEnumExtension(
    &::google::protobuf::MethodOptions::default_instance(),
    20002, 14, false, false,
    &tencent::pbrpc::CompressType_IsValid);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_rpc_5foption_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_rpc_5foption_2eproto {
  StaticDescriptorInitializer_rpc_5foption_2eproto() {
    protobuf_AddDesc_rpc_5foption_2eproto();
  }
} static_descriptor_initializer_rpc_5foption_2eproto_;

const ::google::protobuf::EnumDescriptor* CompressType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CompressType_descriptor_;
}
bool CompressType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::ServiceOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int64 >, 3, false >
  service_timeout(kServiceTimeoutFieldNumber, GOOGLE_LONGLONG(10000));
const ::std::string ServerType_default("UDP");
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::ServiceOptions,
    ::google::protobuf::internal::StringTypeTraits, 9, false >
  ServerType(kServerTypeFieldNumber, ServerType_default);
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::ServiceOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int32 >, 5, false >
  port(kPortFieldNumber, 45000);
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int64 >, 3, false >
  method_timeout(kMethodTimeoutFieldNumber, GOOGLE_LONGLONG(0));
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::EnumTypeTraits< tencent::pbrpc::CompressType, tencent::pbrpc::CompressType_IsValid>, 14, false >
  request_compress_type(kRequestCompressTypeFieldNumber, static_cast< tencent::pbrpc::CompressType >(0));
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::EnumTypeTraits< tencent::pbrpc::CompressType, tencent::pbrpc::CompressType_IsValid>, 14, false >
  response_compress_type(kResponseCompressTypeFieldNumber, static_cast< tencent::pbrpc::CompressType >(0));

// @@protoc_insertion_point(namespace_scope)

}  // namespace pbrpc
}  // namespace tencent

// @@protoc_insertion_point(global_scope)
