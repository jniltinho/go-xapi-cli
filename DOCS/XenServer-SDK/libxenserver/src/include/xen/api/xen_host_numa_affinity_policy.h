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


#ifndef XEN_HOST_NUMA_AFFINITY_POLICY_H
#define XEN_HOST_NUMA_AFFINITY_POLICY_H


#include <xen/api/xen_common.h>


enum xen_host_numa_affinity_policy
{
    /**
     * VMs are spread across all available NUMA nodes
     */
    XEN_HOST_NUMA_AFFINITY_POLICY_ANY,

    /**
     * VMs are placed on the smallest number of NUMA nodes that
     * they fit using soft-pinning, but the policy doesn't guarantee a balanced
     * placement, falling back to the 'any' policy.
     */
    XEN_HOST_NUMA_AFFINITY_POLICY_BEST_EFFORT,

    /**
     * Use the NUMA affinity policy that is the default for the
     * current version
     */
    XEN_HOST_NUMA_AFFINITY_POLICY_DEFAULT_POLICY,

    /**
     * Unknown to this SDK version.
     */
    XEN_HOST_NUMA_AFFINITY_POLICY_UNDEFINED
};


typedef struct xen_host_numa_affinity_policy_set
{
    size_t size;
    enum xen_host_numa_affinity_policy contents[];
} xen_host_numa_affinity_policy_set;

/**
 * Allocate a xen_host_numa_affinity_policy_set of the given size.
 */
extern xen_host_numa_affinity_policy_set *
xen_host_numa_affinity_policy_set_alloc(size_t size);

/**
 * Free the given xen_host_numa_affinity_policy_set.  The given set must
 * have been allocated by this library.
 */
extern void
xen_host_numa_affinity_policy_set_free(xen_host_numa_affinity_policy_set *set);


/**
 * Return the name corresponding to the given code.  This string must
 * not be modified or freed.
 */
extern const char *
xen_host_numa_affinity_policy_to_string(enum xen_host_numa_affinity_policy val);


/**
 * Return the correct code for the given string, or set the session
 * object to failure and return an undefined value if the given string does
 * not match a known code.
 */
extern enum xen_host_numa_affinity_policy
xen_host_numa_affinity_policy_from_string(xen_session *session, const char *str);


#endif
