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


#ifndef XEN_LATEST_SYNCED_UPDATES_APPLIED_STATE_H
#define XEN_LATEST_SYNCED_UPDATES_APPLIED_STATE_H


#include <xen/api/xen_common.h>


enum xen_latest_synced_updates_applied_state
{
    /**
     * The host is up to date with the latest updates synced from
     * remote CDN
     */
    XEN_LATEST_SYNCED_UPDATES_APPLIED_STATE_YES,

    /**
     * The host is outdated with the latest updates synced from
     * remote CDN
     */
    XEN_LATEST_SYNCED_UPDATES_APPLIED_STATE_NO,

    /**
     * If the host is up to date with the latest updates synced
     * from remote CDN is unknown
     */
    XEN_LATEST_SYNCED_UPDATES_APPLIED_STATE_UNKNOWN,

    /**
     * Unknown to this SDK version.
     */
    XEN_LATEST_SYNCED_UPDATES_APPLIED_STATE_UNDEFINED
};


typedef struct xen_latest_synced_updates_applied_state_set
{
    size_t size;
    enum xen_latest_synced_updates_applied_state contents[];
} xen_latest_synced_updates_applied_state_set;

/**
 * Allocate a xen_latest_synced_updates_applied_state_set of the given size.
 */
extern xen_latest_synced_updates_applied_state_set *
xen_latest_synced_updates_applied_state_set_alloc(size_t size);

/**
 * Free the given xen_latest_synced_updates_applied_state_set.  The given set must
 * have been allocated by this library.
 */
extern void
xen_latest_synced_updates_applied_state_set_free(xen_latest_synced_updates_applied_state_set *set);


/**
 * Return the name corresponding to the given code.  This string must
 * not be modified or freed.
 */
extern const char *
xen_latest_synced_updates_applied_state_to_string(enum xen_latest_synced_updates_applied_state val);


/**
 * Return the correct code for the given string, or set the session
 * object to failure and return an undefined value if the given string does
 * not match a known code.
 */
extern enum xen_latest_synced_updates_applied_state
xen_latest_synced_updates_applied_state_from_string(xen_session *session, const char *str);


#endif
