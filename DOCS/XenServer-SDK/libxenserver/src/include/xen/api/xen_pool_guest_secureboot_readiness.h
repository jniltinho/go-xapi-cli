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


#ifndef XEN_POOL_GUEST_SECUREBOOT_READINESS_H
#define XEN_POOL_GUEST_SECUREBOOT_READINESS_H


#include <xen/api/xen_common.h>


enum xen_pool_guest_secureboot_readiness
{
    /**
     * Pool is ready for SecureBoot, all auth files are present
     */
    XEN_POOL_GUEST_SECUREBOOT_READINESS_READY,

    /**
     * Pool is ready for SecureBoot, but there is no dbx auth file
     */
    XEN_POOL_GUEST_SECUREBOOT_READINESS_READY_NO_DBX,

    /**
     * Pool is not ready for SecureBoot, mandatory auth files are
     * missing
     */
    XEN_POOL_GUEST_SECUREBOOT_READINESS_NOT_READY,

    /**
     * Unknown to this SDK version.
     */
    XEN_POOL_GUEST_SECUREBOOT_READINESS_UNDEFINED
};


typedef struct xen_pool_guest_secureboot_readiness_set
{
    size_t size;
    enum xen_pool_guest_secureboot_readiness contents[];
} xen_pool_guest_secureboot_readiness_set;

/**
 * Allocate a xen_pool_guest_secureboot_readiness_set of the given size.
 */
extern xen_pool_guest_secureboot_readiness_set *
xen_pool_guest_secureboot_readiness_set_alloc(size_t size);

/**
 * Free the given xen_pool_guest_secureboot_readiness_set.  The given set must
 * have been allocated by this library.
 */
extern void
xen_pool_guest_secureboot_readiness_set_free(xen_pool_guest_secureboot_readiness_set *set);


/**
 * Return the name corresponding to the given code.  This string must
 * not be modified or freed.
 */
extern const char *
xen_pool_guest_secureboot_readiness_to_string(enum xen_pool_guest_secureboot_readiness val);


/**
 * Return the correct code for the given string, or set the session
 * object to failure and return an undefined value if the given string does
 * not match a known code.
 */
extern enum xen_pool_guest_secureboot_readiness
xen_pool_guest_secureboot_readiness_from_string(xen_session *session, const char *str);


#endif
