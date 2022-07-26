/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef GRPCPP_IMPL_GRPC_LIBRARY_H
#define GRPCPP_IMPL_GRPC_LIBRARY_H

#include <iostream>

#include <grpcpp/impl/codegen/config.h>
#include <grpcpp/impl/codegen/core_codegen.h>

namespace grpc {

class GrpcLibraryInterface {
 public:
  virtual ~GrpcLibraryInterface() = default;
  virtual void init() = 0;
  virtual void shutdown() = 0;
};

/// Initialized by \a grpc::GrpcLibraryInitializer from
/// <grpcpp/impl/grpc_library.h>
extern GrpcLibraryInterface* g_glip;

/// Classes that require gRPC to be initialized should inherit from this class.
class GrpcLibraryCodegen {
 public:
  explicit GrpcLibraryCodegen(bool call_grpc_init = true)
      : grpc_init_called_(false) {
    if (call_grpc_init) {
      GPR_CODEGEN_ASSERT(g_glip &&
                         "gRPC library not initialized. See "
                         "grpc::internal::GrpcLibraryInitializer.");
      g_glip->init();
      grpc_init_called_ = true;
    }
  }
  virtual ~GrpcLibraryCodegen() {
    if (grpc_init_called_) {
      GPR_CODEGEN_ASSERT(g_glip &&
                         "gRPC library not initialized. See "
                         "grpc::internal::GrpcLibraryInitializer.");
      g_glip->shutdown();
    }
  }

 private:
  bool grpc_init_called_;
};

}  // namespace grpc

namespace grpc {

namespace internal {
class GrpcLibrary final : public GrpcLibraryInterface {
 public:
  void init() override { grpc_init(); }
  void shutdown() override { grpc_shutdown(); }
};

extern GrpcLibrary g_grpc_library;

/// Instantiating this class ensures the proper initialization of gRPC.
class GrpcLibraryInitializer final {
 public:
  GrpcLibraryInitializer() {
    if (grpc::g_glip == nullptr) {
      grpc::g_glip = &g_grpc_library;
    }
    if (grpc::g_core_codegen_interface == nullptr) {
      static auto* const g_core_codegen = new CoreCodegen();
      grpc::g_core_codegen_interface = g_core_codegen;
    }
  }

  /// A no-op method to force the linker to reference this class, which will
  /// take care of initializing and shutting down the gRPC runtime.
  int summon() { return 0; }
};

}  // namespace internal
}  // namespace grpc

#endif  // GRPCPP_IMPL_GRPC_LIBRARY_H
