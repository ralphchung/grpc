#ifndef GRPC_GRPC_INSECURE_CREDENTIALS_H
#define GRPC_GRPC_INSECURE_CREDENTIALS_H

#include <grpc/support/port_platform.h>

#include <grpc/grpc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * EXPERIMENTAL API - Subject to change
 *
 * This method creates an insecure channel credentials object.
 */
GRPCAPI grpc_channel_credentials* grpc_insecure_credentials_create();

/**
 * EXPERIMENTAL API - Subject to change
 *
 * This method creates an insecure server credentials object.
 */
GRPCAPI grpc_server_credentials* grpc_insecure_server_credentials_create();

#ifdef __cplusplus
}
#endif

#endif
