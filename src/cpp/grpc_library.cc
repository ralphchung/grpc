/*
 *
 * Copyright 2022 gRPC authors.
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

#include <grpc/grpc.h>
#include <grpcpp/grpc_library.h>
#include <grpcpp/impl/grpc_library.h>

namespace grpc {

GrpcLibrary::GrpcLibrary(bool call_grpc_init) : grpc_init_called_(false) {
  if (call_grpc_init) {
    internal_grpc_library.init();
    grpc_init_called_ = true;
  }
}
GrpcLibrary::~GrpcLibrary() {
  if (grpc_init_called_) {
    internal_grpc_library.shutdown();
  }
}

internal::GrpcLibrary GrpcLibrary::internal_grpc_library;

namespace internal {

void GrpcLibrary::init() { grpc_init(); }

void GrpcLibrary::shutdown() { grpc_shutdown(); }

}  // namespace internal

}  // namespace grpc
