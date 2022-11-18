// Copyright 2022 The gRPC Authors
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

#ifndef GRPC_CHANNEL_CREDENTIALS_SSL_H
#define GRPC_CHANNEL_CREDENTIALS_SSL_H

#include <grpc/support/port_platform.h>

#include <grpc/grpc.h>
#include <grpc/grpc_security.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Deprecated in favor of grpc_ssl_server_credentials_create_ex. It will be
   removed after all of its call sites are migrated to
   grpc_ssl_server_credentials_create_ex. Creates an SSL credentials object.
   The security level of the resulting connection is GRPC_PRIVACY_AND_INTEGRITY.
   - pem_root_certs is the NULL-terminated string containing the PEM encoding
     of the server root certificates. If this parameter is NULL, the
     implementation will first try to dereference the file pointed by the
     GRPC_DEFAULT_SSL_ROOTS_FILE_PATH environment variable, and if that fails,
     try to get the roots set by grpc_override_ssl_default_roots. Eventually,
     if all these fail, it will try to get the roots from a well-known place on
     disk (in the grpc install directory).

     gRPC has implemented root cache if the underlying OpenSSL library supports
     it. The gRPC root certificates cache is only applicable on the default
     root certificates, which is used when this parameter is nullptr. If user
     provides their own pem_root_certs, when creating an SSL credential object,
     gRPC would not be able to cache it, and each subchannel will generate a
     copy of the root store. So it is recommended to avoid providing large room
     pem with pem_root_certs parameter to avoid excessive memory consumption,
     particularly on mobile platforms such as iOS.
   - pem_key_cert_pair is a pointer on the object containing client's private
     key and certificate chain. This parameter can be NULL if the client does
     not have such a key/cert pair.
   - verify_options is an optional verify_peer_options object which holds
     additional options controlling how peer certificates are verified. For
     example, you can supply a callback which receives the peer's certificate
     with which you can do additional verification. Can be NULL, in which
     case verification will retain default behavior. Any settings in
     verify_options are copied during this call, so the verify_options
     object can be released afterwards. */
GRPCAPI grpc_channel_credentials* grpc_ssl_credentials_create(
    const char* pem_root_certs, grpc_ssl_pem_key_cert_pair* pem_key_cert_pair,
    const verify_peer_options* verify_options, void* reserved);

/* Creates an SSL credentials object.
   The security level of the resulting connection is GRPC_PRIVACY_AND_INTEGRITY.
   - pem_root_certs is the NULL-terminated string containing the PEM encoding
     of the server root certificates. If this parameter is NULL, the
     implementation will first try to dereference the file pointed by the
     GRPC_DEFAULT_SSL_ROOTS_FILE_PATH environment variable, and if that fails,
     try to get the roots set by grpc_override_ssl_default_roots. Eventually,
     if all these fail, it will try to get the roots from a well-known place on
     disk (in the grpc install directory).

     gRPC has implemented root cache if the underlying OpenSSL library supports
     it. The gRPC root certificates cache is only applicable on the default
     root certificates, which is used when this parameter is nullptr. If user
     provides their own pem_root_certs, when creating an SSL credential object,
     gRPC would not be able to cache it, and each subchannel will generate a
     copy of the root store. So it is recommended to avoid providing large room
     pem with pem_root_certs parameter to avoid excessive memory consumption,
     particularly on mobile platforms such as iOS.
   - pem_key_cert_pair is a pointer on the object containing client's private
     key and certificate chain. This parameter can be NULL if the client does
     not have such a key/cert pair.
   - verify_options is an optional verify_peer_options object which holds
     additional options controlling how peer certificates are verified. For
     example, you can supply a callback which receives the peer's certificate
     with which you can do additional verification. Can be NULL, in which
     case verification will retain default behavior. Any settings in
     verify_options are copied during this call, so the verify_options
     object can be released afterwards. */
GRPCAPI grpc_channel_credentials* grpc_ssl_credentials_create_ex(
    const char* pem_root_certs, grpc_ssl_pem_key_cert_pair* pem_key_cert_pair,
    const grpc_ssl_verify_peer_options* verify_options, void* reserved);

/** Deprecated in favor of grpc_ssl_server_credentials_create_ex.
   Creates an SSL server_credentials object.
   - pem_roots_cert is the NULL-terminated string containing the PEM encoding of
     the client root certificates. This parameter may be NULL if the server does
     not want the client to be authenticated with SSL.
   - pem_key_cert_pairs is an array private key / certificate chains of the
     server. This parameter cannot be NULL.
   - num_key_cert_pairs indicates the number of items in the private_key_files
     and cert_chain_files parameters. It should be at least 1.
   - force_client_auth, if set to non-zero will force the client to authenticate
     with an SSL cert. Note that this option is ignored if pem_root_certs is
     NULL. */
GRPCAPI grpc_server_credentials* grpc_ssl_server_credentials_create(
    const char* pem_root_certs, grpc_ssl_pem_key_cert_pair* pem_key_cert_pairs,
    size_t num_key_cert_pairs, int force_client_auth, void* reserved);

/** Deprecated in favor of grpc_ssl_server_credentials_create_with_options.
   Same as grpc_ssl_server_credentials_create method except uses
   grpc_ssl_client_certificate_request_type enum to support more ways to
   authenticate client certificates.*/
GRPCAPI grpc_server_credentials* grpc_ssl_server_credentials_create_ex(
    const char* pem_root_certs, grpc_ssl_pem_key_cert_pair* pem_key_cert_pairs,
    size_t num_key_cert_pairs,
    grpc_ssl_client_certificate_request_type client_certificate_request,
    void* reserved);

#ifdef __cplusplus
}
#endif

#endif
