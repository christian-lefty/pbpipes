#include <pbpipes/bigquery_schema_generator.h>

#include <stdio.h>
#include <algorithm>
#include <memory>
#include <sstream>
#include <string>

#include <glog/logging.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <json/json.h>

#include <pbpipes/proto/bigquery_schema.pb.h>

namespace pbpipes {
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::MessageOptions;
using google::protobuf::io::Printer;
using google::protobuf::io::ZeroCopyOutputStream;

BigQuerySchemaGenerator::BigQuerySchemaGenerator() {
}

BigQuerySchemaGenerator::~BigQuerySchemaGenerator() {
}

bool BigQuerySchemaGenerator::Generate(
    const FileDescriptor* file,
    const std::string& parameter,
    GeneratorContext* ctx,
    std::string* error) const {

  for (int i = 0; i < file->message_type_count(); ++i) {
    const Descriptor* message = file->message_type(i);
    VLOG(2) << "inspecting message: " << message->name();

    if (!message->options().HasExtension(pbpipes::proto::bigquery_table)) {
      VLOG(2) << "no bigquery extension, skipping...";
    } else {
      if (!GenerateMessage(file, message, ctx)) {
        return false;
      }
    }
  }

  return true;
}

namespace {
std::string OutputLocation(const FileDescriptor* file, const std::string& opt_name) {
  std::string pkg = file->package();
  std::replace(pkg.begin(), pkg.end(), '.', '/');
  return pkg + "/" + opt_name + ".bigquery.json";
}

std::string FieldName(const std::string& proto_name) {
  // proto3 json defines that field names that are named lower underscore
  // shall be named lower camel case in json.

  std::ostringstream r;

  bool next_upper = false;
  for (char c : proto_name) {
    if (c == '_') {
      next_upper = true;
      continue;
    }

    if (!next_upper) { 
      r << c;
      continue;
    }

    next_upper = false;
    r << ((char) toupper(c));
  }

  return r.str();
}

std::string BigQueryMessageType(const FieldDescriptor* field) {
  CHECK(field->type() == FieldDescriptor::TYPE_MESSAGE);

  const Descriptor* desc = field->message_type();
  
  if (desc->full_name() == "google.protobuf.Timestamp") {
    return "TIMESTAMP";
  }

  return "RECORD";
}

std::string BigQueryType(const FieldDescriptor* field) {
  switch (field->type()) {
    case FieldDescriptor::TYPE_DOUBLE:
    case FieldDescriptor::TYPE_FLOAT:
      return "FLOAT";
    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_UINT32:
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_SFIXED32:
    case FieldDescriptor::TYPE_SINT32:
      return "INTEGER";
    // int64 and their variants are serialized to strings.
    case FieldDescriptor::TYPE_INT64:
    case FieldDescriptor::TYPE_SINT64:
    case FieldDescriptor::TYPE_SFIXED64:
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_UINT64:
      return "STRING";
    case FieldDescriptor::TYPE_BOOL:
      return "BOOLEAN";
    case FieldDescriptor::TYPE_STRING:
    case FieldDescriptor::TYPE_BYTES:
    case FieldDescriptor::TYPE_ENUM:
      return "STRING";
    case FieldDescriptor::TYPE_MESSAGE:
      return BigQueryMessageType(field);
    case FieldDescriptor::TYPE_GROUP:
      LOG(FATAL) << "group is not supported";
  }

  return "";
}

Json::Value ToJson(const Descriptor* desc) {
  // The top level schema is always a list of fields.
  Json::Value r(Json::arrayValue);

  for (int i = 0; i < desc->field_count(); ++i) {
    const FieldDescriptor* field = desc->field(i);
    Json::Value v(Json::objectValue);
    v["mode"] = field->is_repeated() ? "REPEATED" : "NULLABLE";
    v["name"] = FieldName(field->name());
    const std::string bigquery_type = BigQueryType(field);
    v["type"] = bigquery_type;

    if (bigquery_type == "RECORD") {
      // recurse into the child's schema.
      v["fields"] = ToJson(field->message_type());
    }

    r[i] = v;
  }

  return r;
}

std::string JsonToString(const Json::Value& json) {
  Json::StreamWriterBuilder builder;
  builder["commentStyle"] = "None";
  builder["indentation"] = "  ";
  std::unique_ptr<Json::StreamWriter> writer(
      builder.newStreamWriter());
  std::ostringstream os;
  writer->write(json, &os);
  return os.str();
}
}  // anonymous namespace

bool BigQuerySchemaGenerator::GenerateMessage(
    const FileDescriptor* file,
    const Descriptor* desc,
    GeneratorContext* ctx) const {
  const std::string bq_table = desc->options().GetExtension(pbpipes::proto::bigquery_table);
  VLOG(1) << "found bigquery extension: " << bq_table;
  CHECK(!bq_table.empty()) << "option: \"bigquery_table\" cannot be empty!"; 

  Json::Value schema = ToJson(desc);

  ZeroCopyOutputStream* out = ctx->Open(OutputLocation(file, bq_table));
  Printer p(out, '.');
  p.PrintRaw(JsonToString(schema));
  p.PrintRaw("\n");
  return true;
}
}  // namespace pbpipes

