// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.
//
// Generates C++ code for a given .proto file.

#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_GENERATOR_H__

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "google/protobuf/compiler/code_generator.h"
#include "absl/status/status.h"
#include "google/protobuf/cpp_features.pb.h"

// Must be included last.
#include "google/protobuf/port_def.inc"

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {
// CodeGenerator implementation which generates a C++ source file and
// header.  If you create your own protocol compiler binary and you want
// it to support C++ output, you can do so by registering an instance of this
// CodeGenerator with the CommandLineInterface in your main() function.
class PROTOC_EXPORT CppGenerator : public CodeGenerator {
 public:
  CppGenerator() = default;
  CppGenerator(const CppGenerator&) = delete;
  CppGenerator& operator=(const CppGenerator&) = delete;
  ~CppGenerator() override = default;

  enum class Runtime {
    kGoogle3,     // Use the internal google3 runtime.
    kOpensource,  // Use the open-source runtime.

    // Use the open-source runtime with google3 #include paths.  We make these
    // absolute to avoid ambiguity, so the runtime will be #included like:
    //   #include "third_party/protobuf/.../google/protobuf/message.h"
    kOpensourceGoogle3
  };

  void set_opensource_runtime(bool opensource) {
    opensource_runtime_ = opensource;
  }

  // If set to a non-empty string, generated code will do:
  //   #include "<BASE>/google/protobuf/message.h"
  // instead of:
  //   #include "google/protobuf/message.h"
  // This has no effect if opensource_runtime = false.
  void set_runtime_include_base(std::string base) {
    runtime_include_base_ = std::move(base);
  }

  bool Generate(const FileDescriptor* file, const std::string& parameter,
                GeneratorContext* generator_context,
                std::string* error) const override;

  uint64_t GetSupportedFeatures() const override {
    return FEATURE_PROTO3_OPTIONAL | FEATURE_SUPPORTS_EDITIONS;
  }

  std::vector<const FieldDescriptor*> GetFeatureExtensions() const override {
    return {GetExtensionReflection(pb::cpp)};
  }

 private:
  bool opensource_runtime_ = PROTO2_IS_OSS;
  std::string runtime_include_base_;

  absl::Status ValidateFeatures(const FileDescriptor* file) const;
};
}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_COMPILER_CPP_GENERATOR_H__
