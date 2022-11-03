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

#ifndef GRPC_SLICE_H
#define GRPC_SLICE_H

#include <grpc/support/port_platform.h>

#include <stddef.h>

#include <grpc/impl/codegen/gpr_slice.h>
#include <grpc/support/sync.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct grpc_slice grpc_slice;

/** Slice API

   A slice represents a contiguous reference counted array of bytes.
   It is cheap to take references to a slice, and it is cheap to create a
   slice pointing to a subset of another slice.

   The data-structure for slices is exposed here to allow non-gpr code to
   build slices from whatever data they have available.

   When defining interfaces that handle slices, care should be taken to define
   reference ownership semantics (who should call unref?) and mutability
   constraints (is the callee allowed to modify the slice?) */

/* Inlined half of grpc_slice is allowed to expand the size of the overall type
   by this many bytes */
#define GRPC_SLICE_INLINE_EXTRA_SIZE sizeof(void*)

#define GRPC_SLICE_INLINED_SIZE \
  (sizeof(size_t) + sizeof(uint8_t*) - 1 + GRPC_SLICE_INLINE_EXTRA_SIZE)

struct grpc_slice_refcount;
/** A grpc_slice s, if initialized, represents the byte range
   s.bytes[0..s.length-1].

   It can have an associated ref count which has a destruction routine to be run
   when the ref count reaches zero (see grpc_slice_new() and grp_slice_unref()).
   Multiple grpc_slice values may share a ref count.

   If the slice does not have a refcount, it represents an inlined small piece
   of data that is copied by value.

   As a special case, a slice can be given refcount == uintptr_t(1), meaning
   that the slice represents external data that is not refcounted. */
struct grpc_slice {
  struct grpc_slice_refcount* refcount;
  union grpc_slice_data {
    struct grpc_slice_refcounted {
      size_t length;
      uint8_t* bytes;
    } refcounted;
    struct grpc_slice_inlined {
      uint8_t length;
      uint8_t bytes[GRPC_SLICE_INLINED_SIZE];
    } inlined;
  } data;
};

#define GRPC_SLICE_BUFFER_INLINE_ELEMENTS 8

/** Represents an expandable array of slices, to be interpreted as a
   single item. */
typedef struct grpc_slice_buffer {
  /** This is for internal use only. External users (i.e any code outside grpc
   * core) MUST NOT use this field */
  grpc_slice* base_slices;

  /** slices in the array (Points to the first valid grpc_slice in the array) */
  grpc_slice* slices;
  /** the number of slices in the array */
  size_t count;
  /** the number of slices allocated in the array. External users (i.e any code
   * outside grpc core) MUST NOT use this field */
  size_t capacity;
  /** the combined length of all slices in the array */
  size_t length;
  /** inlined elements to avoid allocations */
  grpc_slice inlined[GRPC_SLICE_BUFFER_INLINE_ELEMENTS];
} grpc_slice_buffer;

#define GRPC_SLICE_START_PTR(slice)                 \
  ((slice).refcount ? (slice).data.refcounted.bytes \
                    : (slice).data.inlined.bytes)
#define GRPC_SLICE_LENGTH(slice)                     \
  ((slice).refcount ? (slice).data.refcounted.length \
                    : (slice).data.inlined.length)
#define GRPC_SLICE_SET_LENGTH(slice, newlen)                              \
  ((slice).refcount ? ((slice).data.refcounted.length = (size_t)(newlen)) \
                    : ((slice).data.inlined.length = (uint8_t)(newlen)))
#define GRPC_SLICE_END_PTR(slice) \
  GRPC_SLICE_START_PTR(slice) + GRPC_SLICE_LENGTH(slice)
#define GRPC_SLICE_IS_EMPTY(slice) (GRPC_SLICE_LENGTH(slice) == 0)

#ifdef GRPC_ALLOW_GPR_SLICE_FUNCTIONS

/* Duplicate GPR_* definitions */
#define GPR_SLICE_START_PTR(slice)                  \
  ((slice).refcount ? (slice).data.refcounted.bytes \
                    : (slice).data.inlined.bytes)
#define GPR_SLICE_LENGTH(slice)                      \
  ((slice).refcount ? (slice).data.refcounted.length \
                    : (slice).data.inlined.length)
#define GPR_SLICE_SET_LENGTH(slice, newlen)                               \
  ((slice).refcount ? ((slice).data.refcounted.length = (size_t)(newlen)) \
                    : ((slice).data.inlined.length = (uint8_t)(newlen)))
#define GPR_SLICE_END_PTR(slice) \
  GRPC_SLICE_START_PTR(slice) + GRPC_SLICE_LENGTH(slice)
#define GPR_SLICE_IS_EMPTY(slice) (GRPC_SLICE_LENGTH(slice) == 0)

#endif /* GRPC_ALLOW_GPR_SLICE_FUNCTIONS */

/** Increment the refcount of s. Requires slice is initialized.
   Returns s. */
GPRAPI grpc_slice grpc_slice_ref(grpc_slice s);

/** Decrement the ref count of s.  If the ref count of s reaches zero, all
   slices sharing the ref count are destroyed, and considered no longer
   initialized.  If s is ultimately derived from a call to grpc_slice_new(start,
   len, dest) where dest!=NULL , then (*dest)(start) is called, else if s is
   ultimately derived from a call to grpc_slice_new_with_len(start, len, dest)
   where dest!=NULL , then (*dest)(start, len).  Requires s initialized.  */
GPRAPI void grpc_slice_unref(grpc_slice s);

/** Copy slice - create a new slice that contains the same data as s */
GPRAPI grpc_slice grpc_slice_copy(grpc_slice s);

/** Create a slice pointing at some data. Calls malloc to allocate a refcount
   for the object, and arranges that destroy will be called with the pointer
   passed in at destruction. */
GPRAPI grpc_slice grpc_slice_new(void* p, size_t len, void (*destroy)(void*));

/** Equivalent to grpc_slice_new, but with a separate pointer that is
   passed to the destroy function.  This function can be useful when
   the data is part of a larger structure that must be destroyed when
   the data is no longer needed. */
GPRAPI grpc_slice grpc_slice_new_with_user_data(void* p, size_t len,
                                                void (*destroy)(void*),
                                                void* user_data);

/** Equivalent to grpc_slice_new, but with a two argument destroy function that
   also takes the slice length. */
GPRAPI grpc_slice grpc_slice_new_with_len(void* p, size_t len,
                                          void (*destroy)(void*, size_t));

/** Equivalent to grpc_slice_new(malloc(len), len, free), but saves one malloc()
   call.
   Aborts if malloc() fails. */
GPRAPI grpc_slice grpc_slice_malloc(size_t length);
GPRAPI grpc_slice grpc_slice_malloc_large(size_t length);

#define GRPC_SLICE_MALLOC(len) grpc_slice_malloc(len)

/** Create a slice by copying a string.
   Does not preserve null terminators.
   Equivalent to:
     size_t len = strlen(source);
     grpc_slice slice = grpc_slice_malloc(len);
     memcpy(slice->data, source, len); */
GPRAPI grpc_slice grpc_slice_from_copied_string(const char* source);

/** Create a slice by copying a buffer.
   Equivalent to:
     grpc_slice slice = grpc_slice_malloc(len);
     memcpy(slice->data, source, len); */
GPRAPI grpc_slice grpc_slice_from_copied_buffer(const char* source, size_t len);

/** Create a slice pointing to constant memory */
GPRAPI grpc_slice grpc_slice_from_static_string(const char* source);

/** Create a slice pointing to constant memory */
GPRAPI grpc_slice grpc_slice_from_static_buffer(const void* source, size_t len);

/** Return a result slice derived from s, which shares a ref count with \a s,
   where result.data==s.data+begin, and result.length==end-begin. The ref count
   of \a s is increased by one. Do not assign result back to \a s.
   Requires s initialized, begin <= end, begin <= s.length, and
   end <= source->length. */
GPRAPI grpc_slice grpc_slice_sub(grpc_slice s, size_t begin, size_t end);

/** The same as grpc_slice_sub, but without altering the ref count */
GPRAPI grpc_slice grpc_slice_sub_no_ref(grpc_slice s, size_t begin, size_t end);

/** Splits s into two: modifies s to be s[0:split], and returns a new slice,
   sharing a refcount with s, that contains s[split:s.length].
   Requires s initialized, split <= s.length */
GPRAPI grpc_slice grpc_slice_split_tail(grpc_slice* s, size_t split);

typedef enum {
  GRPC_SLICE_REF_TAIL = 1,
  GRPC_SLICE_REF_HEAD = 2,
  GRPC_SLICE_REF_BOTH = 1 + 2
} grpc_slice_ref_whom;

/** The same as grpc_slice_split_tail, but with an option to skip altering
 * refcounts (grpc_slice_split_tail_maybe_ref(..., true) is equivalent to
 * grpc_slice_split_tail(...)) */
GPRAPI grpc_slice grpc_slice_split_tail_maybe_ref(grpc_slice* s, size_t split,
                                                  grpc_slice_ref_whom ref_whom);

/** Splits s into two: modifies s to be s[split:s.length], and returns a new
   slice, sharing a refcount with s, that contains s[0:split].
   Requires s initialized, split <= s.length */
GPRAPI grpc_slice grpc_slice_split_head(grpc_slice* s, size_t split);

GPRAPI grpc_slice grpc_empty_slice(void);

GPRAPI int grpc_slice_eq(grpc_slice a, grpc_slice b);

/** Returns <0 if a < b, ==0 if a == b, >0 if a > b
   The order is arbitrary, and is not guaranteed to be stable across different
   versions of the API. */
GPRAPI int grpc_slice_cmp(grpc_slice a, grpc_slice b);
GPRAPI int grpc_slice_str_cmp(grpc_slice a, const char* b);

/** return non-zero if the first blen bytes of a are equal to b */
GPRAPI int grpc_slice_buf_start_eq(grpc_slice a, const void* b, size_t blen);

/** return the index of the last instance of \a c in \a s, or -1 if not found */
GPRAPI int grpc_slice_rchr(grpc_slice s, char c);
GPRAPI int grpc_slice_chr(grpc_slice s, char c);

/** return the index of the first occurrence of \a needle in \a haystack, or -1
   if it's not found */
GPRAPI int grpc_slice_slice(grpc_slice haystack, grpc_slice needle);

/** Do two slices point at the same memory, with the same length
   If a or b is inlined, actually compares data */
GPRAPI int grpc_slice_is_equivalent(grpc_slice a, grpc_slice b);

/** Return a slice pointing to newly allocated memory that has the same contents
 * as \a s */
GPRAPI grpc_slice grpc_slice_dup(grpc_slice a);

/** Return a copy of slice as a C string. Offers no protection against embedded
   NULL's. Returned string must be freed with gpr_free. */
GPRAPI char* grpc_slice_to_c_string(grpc_slice s);

#ifdef __cplusplus
}
#endif

#endif /* GRPC_SLICE_H */
