/*
 *
 * Copyright 2019 gRPC authors.
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

#ifndef GRPCPP_IMPL_CODEGEN_SYNC_H
#define GRPCPP_IMPL_CODEGEN_SYNC_H

// IWYU pragma: private

#include <grpc/impl/sync.h>

// Whenever possible, prefer "src/core/lib/gprpp/sync.h" over this file,
// since in core we do not rely on g_core_codegen_interface and hence do not
// pay the costs of virtual function calls.
// TODO(chengyuc): fix it when removing codegen files

namespace grpc {
namespace internal {

using Mutex = grpc_core::Mutex;
using MutexLock = grpc_core::MutexLock;
using ReleasableMutexLock = grpc_core::ReleasableMutexLock;
using CondVar = grpc_core::CondVar;

}  // namespace internal
}  // namespace grpc

#endif  // GRPCPP_IMPL_CODEGEN_SYNC_H
