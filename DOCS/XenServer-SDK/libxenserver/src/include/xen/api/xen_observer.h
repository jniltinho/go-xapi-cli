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


#ifndef XEN_OBSERVER_H
#define XEN_OBSERVER_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_observer_decl.h>
#include <xen/api/xen_observer_xen_observer_record_map.h>
#include <xen/api/xen_string_set.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>


/*
 * The Observer class.
 * 
 * Describes an observer which will control observability activity in the
 * Toolstack.
 */


/**
 * Free the given xen_observer.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_observer_free(xen_observer observer);


typedef struct xen_observer_set
{
    size_t size;
    xen_observer *contents[];
} xen_observer_set;

/**
 * Allocate a xen_observer_set of the given size.
 */
extern xen_observer_set *
xen_observer_set_alloc(size_t size);

/**
 * Free the given xen_observer_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_observer_set_free(xen_observer_set *set);


typedef struct xen_observer_record
{
    xen_observer handle;
    char *uuid;
    char *name_label;
    char *name_description;
    struct xen_host_record_opt_set *hosts;
    xen_string_string_map *attributes;
    struct xen_string_set *endpoints;
    struct xen_string_set *components;
    bool enabled;
} xen_observer_record;

/**
 * Allocate a xen_observer_record.
 */
extern xen_observer_record *
xen_observer_record_alloc(void);

/**
 * Free the given xen_observer_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_observer_record_free(xen_observer_record *record);


typedef struct xen_observer_record_opt
{
    bool is_record;
    union
    {
        xen_observer handle;
        xen_observer_record *record;
    } u;
} xen_observer_record_opt;

/**
 * Allocate a xen_observer_record_opt.
 */
extern xen_observer_record_opt *
xen_observer_record_opt_alloc(void);

/**
 * Free the given xen_observer_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_observer_record_opt_free(xen_observer_record_opt *record_opt);


typedef struct xen_observer_record_set
{
    size_t size;
    xen_observer_record *contents[];
} xen_observer_record_set;

/**
 * Allocate a xen_observer_record_set of the given size.
 */
extern xen_observer_record_set *
xen_observer_record_set_alloc(size_t size);

/**
 * Free the given xen_observer_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_observer_record_set_free(xen_observer_record_set *set);


typedef struct xen_observer_record_opt_set
{
    size_t size;
    xen_observer_record_opt *contents[];
} xen_observer_record_opt_set;

/**
 * Allocate a xen_observer_record_opt_set of the given size.
 */
extern xen_observer_record_opt_set *
xen_observer_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_observer_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_observer_record_opt_set_free(xen_observer_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_record(xen_session *session, xen_observer_record **result, xen_observer observer);


/**
 * Get a reference to the Observer instance with the specified
 * UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_by_uuid(xen_session *session, xen_observer *result, char *uuid);


/**
 * Create a new Observer instance, and return its handle.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_create(xen_session *session, xen_observer *result, xen_observer_record *record);


/**
 * Create a new Observer instance, and return its handle.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_create_async(xen_session *session, xen_task *result, xen_observer_record *record);


/**
 * Destroy the specified Observer instance.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_destroy(xen_session *session, xen_observer observer);


/**
 * Destroy the specified Observer instance.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_destroy_async(xen_session *session, xen_task *result, xen_observer observer);


/**
 * Get all the Observer instances with the given label.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_by_name_label(xen_session *session, struct xen_observer_set **result, char *label);


/**
 * Get the uuid field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_uuid(xen_session *session, char **result, xen_observer observer);


/**
 * Get the name/label field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_name_label(xen_session *session, char **result, xen_observer observer);


/**
 * Get the name/description field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_name_description(xen_session *session, char **result, xen_observer observer);


/**
 * Get the hosts field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_hosts(xen_session *session, struct xen_host_set **result, xen_observer observer);


/**
 * Get the attributes field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_attributes(xen_session *session, xen_string_string_map **result, xen_observer observer);


/**
 * Get the endpoints field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_endpoints(xen_session *session, struct xen_string_set **result, xen_observer observer);


/**
 * Get the components field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_components(xen_session *session, struct xen_string_set **result, xen_observer observer);


/**
 * Get the enabled field of the given Observer.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_enabled(xen_session *session, bool *result, xen_observer observer);


/**
 * Set the name/label field of the given Observer.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_name_label(xen_session *session, xen_observer observer, char *label);


/**
 * Set the name/description field of the given Observer.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_name_description(xen_session *session, xen_observer observer, char *description);


/**
 * Sets the hosts that the observer is to be registered on.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_hosts(xen_session *session, xen_observer self, struct xen_host_set *value);


/**
 * Sets the hosts that the observer is to be registered on.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_hosts_async(xen_session *session, xen_task *result, xen_observer self, struct xen_host_set *value);


/**
 * Enable / disable this observer which will stop the observer from
 * producing observability information.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_enabled(xen_session *session, xen_observer self, bool value);


/**
 * Enable / disable this observer which will stop the observer from
 * producing observability information.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_enabled_async(xen_session *session, xen_task *result, xen_observer self, bool value);


/**
 * Set the attributes of an observer. These are used to emit metadata
 * by the observer.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_attributes(xen_session *session, xen_observer self, xen_string_string_map *value);


/**
 * Set the attributes of an observer. These are used to emit metadata
 * by the observer.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_attributes_async(xen_session *session, xen_task *result, xen_observer self, xen_string_string_map *value);


/**
 * Set the file/HTTP endpoints the observer sends data to.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_endpoints(xen_session *session, xen_observer self, struct xen_string_set *value);


/**
 * Set the file/HTTP endpoints the observer sends data to.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_endpoints_async(xen_session *session, xen_task *result, xen_observer self, struct xen_string_set *value);


/**
 * Set the components on which the observer will broadcast to. i.e.
 * xapi, xenopsd, networkd, etc.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_components(xen_session *session, xen_observer self, struct xen_string_set *value);


/**
 * Set the components on which the observer will broadcast to. i.e.
 * xapi, xenopsd, networkd, etc.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_observer_set_components_async(xen_session *session, xen_task *result, xen_observer self, struct xen_string_set *value);


/**
 * Return a list of all the Observers known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_all(xen_session *session, struct xen_observer_set **result);


/**
 * Return a map of Observer references to Observer records for all
 * Observers known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_observer_get_all_records(xen_session *session, xen_observer_xen_observer_record_map **result);


#endif
