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


#ifndef XEN_HOST_DRIVER_H
#define XEN_HOST_DRIVER_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_driver_variant.h>
#include <xen/api/xen_driver_variant_decl.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_host_driver_decl.h>
#include <xen/api/xen_host_driver_xen_host_driver_record_map.h>
#include <xen/api/xen_task_decl.h>


/*
 * The Host_driver class.
 * 
 * UNSUPPORTED. A multi-version driver on a host.
 */


/**
 * Free the given xen_host_driver.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_host_driver_free(xen_host_driver host_driver);


typedef struct xen_host_driver_set
{
    size_t size;
    xen_host_driver *contents[];
} xen_host_driver_set;

/**
 * Allocate a xen_host_driver_set of the given size.
 */
extern xen_host_driver_set *
xen_host_driver_set_alloc(size_t size);

/**
 * Free the given xen_host_driver_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_host_driver_set_free(xen_host_driver_set *set);


typedef struct xen_host_driver_record
{
    xen_host_driver handle;
    char *uuid;
    struct xen_host_record_opt *host;
    char *name;
    char *friendly_name;
    struct xen_driver_variant_record_opt_set *variants;
    struct xen_driver_variant_record_opt *active_variant;
    struct xen_driver_variant_record_opt *selected_variant;
    char *type;
    char *description;
    char *info;
} xen_host_driver_record;

/**
 * Allocate a xen_host_driver_record.
 */
extern xen_host_driver_record *
xen_host_driver_record_alloc(void);

/**
 * Free the given xen_host_driver_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_host_driver_record_free(xen_host_driver_record *record);


typedef struct xen_host_driver_record_opt
{
    bool is_record;
    union
    {
        xen_host_driver handle;
        xen_host_driver_record *record;
    } u;
} xen_host_driver_record_opt;

/**
 * Allocate a xen_host_driver_record_opt.
 */
extern xen_host_driver_record_opt *
xen_host_driver_record_opt_alloc(void);

/**
 * Free the given xen_host_driver_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_host_driver_record_opt_free(xen_host_driver_record_opt *record_opt);


typedef struct xen_host_driver_record_set
{
    size_t size;
    xen_host_driver_record *contents[];
} xen_host_driver_record_set;

/**
 * Allocate a xen_host_driver_record_set of the given size.
 */
extern xen_host_driver_record_set *
xen_host_driver_record_set_alloc(size_t size);

/**
 * Free the given xen_host_driver_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_host_driver_record_set_free(xen_host_driver_record_set *set);


typedef struct xen_host_driver_record_opt_set
{
    size_t size;
    xen_host_driver_record_opt *contents[];
} xen_host_driver_record_opt_set;

/**
 * Allocate a xen_host_driver_record_opt_set of the given size.
 */
extern xen_host_driver_record_opt_set *
xen_host_driver_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_host_driver_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_host_driver_record_opt_set_free(xen_host_driver_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_record(xen_session *session, xen_host_driver_record **result, xen_host_driver host_driver);


/**
 * Get a reference to the Host_driver instance with the specified
 * UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_by_uuid(xen_session *session, xen_host_driver *result, char *uuid);


/**
 * Get the uuid field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_uuid(xen_session *session, char **result, xen_host_driver host_driver);


/**
 * Get the host field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_host(xen_session *session, xen_host *result, xen_host_driver host_driver);


/**
 * Get the name field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_name(xen_session *session, char **result, xen_host_driver host_driver);


/**
 * Get the friendly_name field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_friendly_name(xen_session *session, char **result, xen_host_driver host_driver);


/**
 * Get the variants field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_variants(xen_session *session, struct xen_driver_variant_set **result, xen_host_driver host_driver);


/**
 * Get the active_variant field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_active_variant(xen_session *session, xen_driver_variant *result, xen_host_driver host_driver);


/**
 * Get the selected_variant field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_selected_variant(xen_session *session, xen_driver_variant *result, xen_host_driver host_driver);


/**
 * Get the type field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_type(xen_session *session, char **result, xen_host_driver host_driver);


/**
 * Get the description field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_description(xen_session *session, char **result, xen_host_driver host_driver);


/**
 * Get the info field of the given Host_driver.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_info(xen_session *session, char **result, xen_host_driver host_driver);


/**
 * UNSUPPORTED. Select a variant of the driver to become active after
 * reboot or immediately if currently no version is active.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_driver_select(xen_session *session, xen_host_driver self, xen_driver_variant variant);


/**
 * UNSUPPORTED. Select a variant of the driver to become active after
 * reboot or immediately if currently no version is active.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_driver_select_async(xen_session *session, xen_task *result, xen_host_driver self, xen_driver_variant variant);


/**
 * UNSUPPORTED. Deselect the currently active variant of this driver
 * after reboot. No action will be taken if no variant is currently
 * active.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_driver_deselect(xen_session *session, xen_host_driver self);


/**
 * UNSUPPORTED. Deselect the currently active variant of this driver
 * after reboot. No action will be taken if no variant is currently
 * active.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_driver_deselect_async(xen_session *session, xen_task *result, xen_host_driver self);


/**
 * UNSUPPORTED. Re-scan a host's drivers and update information about
 * them. This is mostly  for trouble shooting.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_driver_rescan(xen_session *session, xen_host host);


/**
 * UNSUPPORTED. Re-scan a host's drivers and update information about
 * them. This is mostly  for trouble shooting.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_driver_rescan_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Return a list of all the Host_drivers known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_all(xen_session *session, struct xen_host_driver_set **result);


/**
 * Return a map of Host_driver references to Host_driver records for
 * all Host_drivers known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_driver_get_all_records(xen_session *session, xen_host_driver_xen_host_driver_record_map **result);


#endif
