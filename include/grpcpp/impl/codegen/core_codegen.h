/*
 *
 * Copyright 2016 gRPC authors.
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

#ifndef GRPCPP_IMPL_CODEGEN_CORE_CODEGEN_H
#define GRPCPP_IMPL_CODEGEN_CORE_CODEGEN_H

// IWYU pragma: private

// This file should be compiled as part of grpcpp.

#include <grpc/byte_buffer.h>
#include <grpc/grpc.h>
#include <grpc/impl/codegen/grpc_types.h>
#include <grpcpp/support/status.h>

namespace grpc {

class CoreCodegen final {
 public:
  static const grpc_completion_queue_factory*
  grpc_completion_queue_factory_lookup(
      const grpc_completion_queue_attributes* attributes);
  static grpc_completion_queue* grpc_completion_queue_create(
      const grpc_completion_queue_factory* factory,
      const grpc_completion_queue_attributes* attributes, void* reserved);
  static grpc_completion_queue* grpc_completion_queue_create_for_next(
      void* reserved);
  static grpc_completion_queue* grpc_completion_queue_create_for_pluck(
      void* reserved);
  static void grpc_completion_queue_shutdown(grpc_completion_queue* cq);
  static void grpc_completion_queue_destroy(grpc_completion_queue* cq);
  static grpc_event grpc_completion_queue_pluck(grpc_completion_queue* cq,
                                                void* tag,
                                                gpr_timespec deadline,
                                                void* reserved);

  static void* gpr_malloc(size_t size);
  static void gpr_free(void* p);

  static void grpc_init();
  static void grpc_shutdown();

  static void gpr_mu_init(gpr_mu* mu);
  static void gpr_mu_destroy(gpr_mu* mu);
  static void gpr_mu_lock(gpr_mu* mu);
  static void gpr_mu_unlock(gpr_mu* mu);
  static void gpr_cv_init(gpr_cv* cv);
  static void gpr_cv_destroy(gpr_cv* cv);
  static int gpr_cv_wait(gpr_cv* cv, gpr_mu* mu, gpr_timespec abs_deadline);
  static void gpr_cv_signal(gpr_cv* cv);
  static void gpr_cv_broadcast(gpr_cv* cv);

  static grpc_call_error grpc_call_start_batch(grpc_call* call,
                                               const grpc_op* ops, size_t nops,
                                               void* tag, void* reserved);
  static grpc_call_error grpc_call_cancel_with_status(grpc_call* call,
                                                      grpc_status_code status,
                                                      const char* description,
                                                      void* reserved);
  static int grpc_call_failed_before_recv_message(const grpc_call* c);
  static void grpc_call_ref(grpc_call* call);
  static void grpc_call_unref(grpc_call* call);
  static void* grpc_call_arena_alloc(grpc_call* call, size_t length);
  static const char* grpc_call_error_to_string(grpc_call_error error);

  static grpc_byte_buffer* grpc_byte_buffer_copy(grpc_byte_buffer* bb);
  static void grpc_byte_buffer_destroy(grpc_byte_buffer* bb);
  static size_t grpc_byte_buffer_length(grpc_byte_buffer* bb);

  static int grpc_byte_buffer_reader_init(grpc_byte_buffer_reader* reader,
                                          grpc_byte_buffer* buffer);
  static void grpc_byte_buffer_reader_destroy(grpc_byte_buffer_reader* reader);
  static int grpc_byte_buffer_reader_next(grpc_byte_buffer_reader* reader,
                                          grpc_slice* slice);
  static int grpc_byte_buffer_reader_peek(grpc_byte_buffer_reader* reader,
                                          grpc_slice** slice);

  static grpc_byte_buffer* grpc_raw_byte_buffer_create(grpc_slice* slice,
                                                       size_t nslices);
  static grpc_slice grpc_slice_new_with_user_data(void* p, size_t len,
                                                  void (*destroy)(void*),
                                                  void* user_data);
  static grpc_slice grpc_slice_new_with_len(void* p, size_t len,
                                            void (*destroy)(void*, size_t));
  static grpc_slice grpc_empty_slice();
  static grpc_slice grpc_slice_malloc(size_t length);
  static void grpc_slice_unref(grpc_slice slice);
  static grpc_slice grpc_slice_ref(grpc_slice slice);
  static grpc_slice grpc_slice_split_tail(grpc_slice* s, size_t split);
  static grpc_slice grpc_slice_split_head(grpc_slice* s, size_t split);
  static grpc_slice grpc_slice_sub(grpc_slice s, size_t begin, size_t end);
  static void grpc_slice_buffer_add(grpc_slice_buffer* sb, grpc_slice slice);
  static void grpc_slice_buffer_pop(grpc_slice_buffer* sb);
  static void grpc_slice_buffer_add_indexed(grpc_slice_buffer* sb,
                                            grpc_slice slice);
  static grpc_slice grpc_slice_from_static_buffer(const void* buffer,
                                                  size_t length);
  static grpc_slice grpc_slice_from_copied_buffer(const void* buffer,
                                                  size_t length);
  static void grpc_metadata_array_init(grpc_metadata_array* array);
  static void grpc_metadata_array_destroy(grpc_metadata_array* array);

  static gpr_timespec gpr_inf_future(gpr_clock_type type);
  static gpr_timespec gpr_time_0(gpr_clock_type type);

  static const Status& ok();
  static const Status& cancelled();

  static void assert_fail(const char* failed_assertion, const char* file,
                          int line);
};

/// Codegen specific version of \a GPR_ASSERT.
#define GPR_CODEGEN_ASSERT(x)                           \
  do {                                                  \
    if (GPR_UNLIKELY(!(x))) {                           \
      CoreCodegen::assert_fail(#x, __FILE__, __LINE__); \
    }                                                   \
  } while (0)

/// Codegen specific version of \a GPR_DEBUG_ASSERT.
#ifndef NDEBUG
#define GPR_CODEGEN_DEBUG_ASSERT(x) GPR_CODEGEN_ASSERT(x)
#else
#define GPR_CODEGEN_DEBUG_ASSERT(x) \
  do {                              \
  } while (0)
#endif

}  // namespace grpc

#endif  // GRPCPP_IMPL_CODEGEN_CORE_CODEGEN_H
