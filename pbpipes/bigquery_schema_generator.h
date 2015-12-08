#ifndef PBPIPES_BIGQUERY_SCHEMA_GENERATOR_H_
#define PBPIPES_BIGQUERY_SCHEMA_GENERATOR_H_

#include <string>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>

namespace pbpipes {
using google::protobuf::Descriptor;
using google::protobuf::FileDescriptor;
using google::protobuf::compiler::CodeGenerator;
using google::protobuf::compiler::GeneratorContext;

class BigQuerySchemaGenerator : public CodeGenerator {
 public:
  BigQuerySchemaGenerator();

  ~BigQuerySchemaGenerator() override;

  // implementation of CodeGenerator.
  bool Generate(const FileDescriptor* file,
                const std::string& parameter,
                GeneratorContext* ctx,
                std::string* error) const override;

 private:
  bool GenerateMessage(const FileDescriptor* file,
                       const Descriptor* message, 
                       GeneratorContext* ctx) const;
};
}  // namespace pbpipes

#endif  // PBPIPES_BIGQUERY_SCHEMA_GENERATOR_H_

