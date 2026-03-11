/*
 * Copyright (c) Cloud Software Group, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2) Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef XEN_POOL_ALLOWED_OPERATIONS_H
#define XEN_POOL_ALLOWED_OPERATIONS_H


#include <xen/api/xen_common.h>


enum xen_pool_allowed_operations
{
    /**
     * Indicates this pool is in the process of enabling HA
     */
    XEN_POOL_ALLOWED_OPERATIONS_HA_ENABLE,

    /**
     * Indicates this pool is in the process of disabling HA
     */
    XEN_POOL_ALLOWED_OPERATIONS_HA_DISABLE,

    /**
     * Indicates this pool is in the process of creating a cluster
     */
    XEN_POOL_ALLOWED_OPERATIONS_CLUSTER_CREATE,

    /**
     * Indicates this pool is in the process of changing master
     */
    XEN_POOL_ALLOWED_OPERATIONS_DESIGNATE_NEW_MASTER,

    /**
     * Indicates this pool is in the process of configuring
     * repositories
     */
    XEN_POOL_ALLOWED_OPERATIONS_CONFIGURE_REPOSITORIES,

    /**
     * Indicates this pool is in the process of syncing updates
     */
    XEN_POOL_ALLOWED_OPERATIONS_SYNC_UPDATES,

    /**
     * Indicates this pool is in the process of syncing bundle
     */
    XEN_POOL_ALLOWED_OPERATIONS_SYNC_BUNDLE,

    /**
     * Indicates this pool is in the process of getting updates
     */
    XEN_POOL_ALLOWED_OPERATIONS_GET_UPDATES,

    /**
     * Indicates this pool is in the process of applying updates
     */
    XEN_POOL_ALLOWED_OPERATIONS_APPLY_UPDATES,

    /**
     * Indicates this pool is in the process of enabling TLS
     * verification
     */
    XEN_POOL_ALLOWED_OPERATIONS_TLS_VERIFICATION_ENABLE,

    /**
     * A certificate refresh and distribution is in progress
     */
    XEN_POOL_ALLOWED_OPERATIONS_CERT_REFRESH,

    /**
     * Indicates this pool is exchanging internal certificates with
     * a new joiner
     */
    XEN_POOL_ALLOWED_OPERATIONS_EXCHANGE_CERTIFICATES_ON_JOIN,

    /**
     * Indicates this pool is exchanging ca certificates with a new
     * joiner
     */
    XEN_POOL_ALLOWED_OPERATIONS_EXCHANGE_CA_CERTIFICATES_ON_JOIN,

    /**
     * Indicates the primary host is sending its certificates to
     * another host
     */
    XEN_POOL_ALLOWED_OPERATIONS_COPY_PRIMARY_HOST_CERTS,

    /**
     * Ejection of a host from the pool is under way
     */
    XEN_POOL_ALLOWED_OPERATIONS_EJECT,

    /**
     * Unknown to this SDK version.
     */
    XEN_POOL_ALLOWED_OPERATIONS_UNDEFINED
};


typedef struct xen_pool_allowed_operations_set
{
    size_t size;
    enum xen_pool_allowed_operations contents[];
} xen_pool_allowed_operations_set;

/**
 * Allocate a xen_pool_allowed_operations_set of the given size.
 */
extern xen_pool_allowed_operations_set *
xen_pool_allowed_operations_set_alloc(size_t size);

/**
 * Free the given xen_pool_allowed_operations_set.  The given set must
 * have been allocated by this library.
 */
extern void
xen_pool_allowed_operations_set_free(xen_pool_allowed_operations_set *set);


/**
 * Return the name corresponding to the given code.  This string must
 * not be modified or freed.
 */
extern const char *
xen_pool_allowed_operations_to_string(enum xen_pool_allowed_operations val);


/**
 * Return the correct code for the given string, or set the session
 * object to failure and return an undefined value if the given string does
 * not match a known code.
 */
extern enum xen_pool_allowed_operations
xen_pool_allowed_operations_from_string(xen_session *session, const char *str);


#endif
