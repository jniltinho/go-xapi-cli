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


#ifndef XEN_DRIVER_VARIANT_H
#define XEN_DRIVER_VARIANT_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_driver_variant_decl.h>
#include <xen/api/xen_driver_variant_xen_driver_variant_record_map.h>
#include <xen/api/xen_host_driver.h>
#include <xen/api/xen_host_driver_decl.h>
#include <xen/api/xen_task_decl.h>


/*
 * The Driver_variant class.
 * 
 * UNSUPPORTED. Variant of a host driver.
 */


/**
 * Free the given xen_driver_variant.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_driver_variant_free(xen_driver_variant driver_variant);


typedef struct xen_driver_variant_set
{
    size_t size;
    xen_driver_variant *contents[];
} xen_driver_variant_set;

/**
 * Allocate a xen_driver_variant_set of the given size.
 */
extern xen_driver_variant_set *
xen_driver_variant_set_alloc(size_t size);

/**
 * Free the given xen_driver_variant_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_driver_variant_set_free(xen_driver_variant_set *set);


typedef struct xen_driver_variant_record
{
    xen_driver_variant handle;
    char *uuid;
    char *name;
    struct xen_host_driver_record_opt *driver;
    char *version;
    bool hardware_present;
    double priority;
    char *status;
} xen_driver_variant_record;

/**
 * Allocate a xen_driver_variant_record.
 */
extern xen_driver_variant_record *
xen_driver_variant_record_alloc(void);

/**
 * Free the given xen_driver_variant_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_driver_variant_record_free(xen_driver_variant_record *record);


typedef struct xen_driver_variant_record_opt
{
    bool is_record;
    union
    {
        xen_driver_variant handle;
        xen_driver_variant_record *record;
    } u;
} xen_driver_variant_record_opt;

/**
 * Allocate a xen_driver_variant_record_opt.
 */
extern xen_driver_variant_record_opt *
xen_driver_variant_record_opt_alloc(void);

/**
 * Free the given xen_driver_variant_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_driver_variant_record_opt_free(xen_driver_variant_record_opt *record_opt);


typedef struct xen_driver_variant_record_set
{
    size_t size;
    xen_driver_variant_record *contents[];
} xen_driver_variant_record_set;

/**
 * Allocate a xen_driver_variant_record_set of the given size.
 */
extern xen_driver_variant_record_set *
xen_driver_variant_record_set_alloc(size_t size);

/**
 * Free the given xen_driver_variant_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_driver_variant_record_set_free(xen_driver_variant_record_set *set);


typedef struct xen_driver_variant_record_opt_set
{
    size_t size;
    xen_driver_variant_record_opt *contents[];
} xen_driver_variant_record_opt_set;

/**
 * Allocate a xen_driver_variant_record_opt_set of the given size.
 */
extern xen_driver_variant_record_opt_set *
xen_driver_variant_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_driver_variant_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_driver_variant_record_opt_set_free(xen_driver_variant_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_record(xen_session *session, xen_driver_variant_record **result, xen_driver_variant driver_variant);


/**
 * Get a reference to the Driver_variant instance with the specified
 * UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_by_uuid(xen_session *session, xen_driver_variant *result, char *uuid);


/**
 * Get the uuid field of the given Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_uuid(xen_session *session, char **result, xen_driver_variant driver_variant);


/**
 * Get the name field of the given Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_name(xen_session *session, char **result, xen_driver_variant driver_variant);


/**
 * Get the driver field of the given Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_driver(xen_session *session, xen_host_driver *result, xen_driver_variant driver_variant);


/**
 * Get the version field of the given Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_version(xen_session *session, char **result, xen_driver_variant driver_variant);


/**
 * Get the hardware_present field of the given Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_hardware_present(xen_session *session, bool *result, xen_driver_variant driver_variant);


/**
 * Get the priority field of the given Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_priority(xen_session *session, double *result, xen_driver_variant driver_variant);


/**
 * Get the status field of the given Driver_variant.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_status(xen_session *session, char **result, xen_driver_variant driver_variant);


/**
 * UNSUPPORTED Select this variant of a driver to become active after
 * reboot or immediately if currently no version is active.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_driver_variant_select(xen_session *session, xen_driver_variant self);


/**
 * UNSUPPORTED Select this variant of a driver to become active after
 * reboot or immediately if currently no version is active.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_driver_variant_select_async(xen_session *session, xen_task *result, xen_driver_variant self);


/**
 * Return a list of all the Driver_variants known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_all(xen_session *session, struct xen_driver_variant_set **result);


/**
 * Return a map of Driver_variant references to Driver_variant records
 * for all Driver_variants known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_driver_variant_get_all_records(xen_session *session, xen_driver_variant_xen_driver_variant_record_map **result);


#endif
