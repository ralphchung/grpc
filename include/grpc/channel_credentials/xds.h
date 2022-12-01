// Copyright 2022 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GRPC_CHANNEL_CREDENTIALS_XDS_H
#define GRPC_CHANNEL_CREDENTIALS_XDS_H

#include <grpc/support/port_platform.h>

#include <grpc/grpc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * EXPERIMENTAL API - Subject to change
 *
 * This method creates an xDS channel credentials object.
 *
 * Creating a channel with credentials of this type indicates that the channel
 * should get credentials configuration from the xDS control plane.
 *
 * \a fallback_credentials are used if the channel target does not have the
 * 'xds:///' scheme or if the xDS control plane does not provide information on
 * how to fetch credentials dynamically. Does NOT take ownership of the \a
 * fallback_credentials. (Internally takes a ref to the object.)
 */
GRPCAPI grpc_channel_credentials* grpc_xds_credentials_create(
    grpc_channel_credentials* fallback_credentials);

/**
 * EXPERIMENTAL API - Subject to change
 *
 * This method creates an xDS server credentials object.
 *
 * \a fallback_credentials are used if the xDS control plane does not provide
 * information on how to fetch credentials dynamically.
 *
 * Does NOT take ownership of the \a fallback_credentials. (Internally takes
 * a ref to the object.)
 */
GRPCAPI grpc_server_credentials* grpc_xds_server_credentials_create(
    grpc_server_credentials* fallback_credentials);

#ifdef __cplusplus
}
#endif

#endif /* GRPC_CHANNEL_CREDENTIALS_XDS_H */
