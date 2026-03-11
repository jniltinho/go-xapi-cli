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


#ifndef XEN_VM_GROUP_H
#define XEN_VM_GROUP_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_placement_policy.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_vm.h>
#include <xen/api/xen_vm_decl.h>
#include <xen/api/xen_vm_group_decl.h>
#include <xen/api/xen_vm_group_xen_vm_group_record_map.h>


/*
 * The VM_group class.
 * 
 * A VM group.
 */


/**
 * Free the given xen_vm_group.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_vm_group_free(xen_vm_group vm_group);


typedef struct xen_vm_group_set
{
    size_t size;
    xen_vm_group *contents[];
} xen_vm_group_set;

/**
 * Allocate a xen_vm_group_set of the given size.
 */
extern xen_vm_group_set *
xen_vm_group_set_alloc(size_t size);

/**
 * Free the given xen_vm_group_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_vm_group_set_free(xen_vm_group_set *set);


typedef struct xen_vm_group_record
{
    xen_vm_group handle;
    char *uuid;
    char *name_label;
    char *name_description;
    enum xen_placement_policy placement;
    struct xen_vm_record_opt_set *vms;
} xen_vm_group_record;

/**
 * Allocate a xen_vm_group_record.
 */
extern xen_vm_group_record *
xen_vm_group_record_alloc(void);

/**
 * Free the given xen_vm_group_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_vm_group_record_free(xen_vm_group_record *record);


typedef struct xen_vm_group_record_opt
{
    bool is_record;
    union
    {
        xen_vm_group handle;
        xen_vm_group_record *record;
    } u;
} xen_vm_group_record_opt;

/**
 * Allocate a xen_vm_group_record_opt.
 */
extern xen_vm_group_record_opt *
xen_vm_group_record_opt_alloc(void);

/**
 * Free the given xen_vm_group_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_vm_group_record_opt_free(xen_vm_group_record_opt *record_opt);


typedef struct xen_vm_group_record_set
{
    size_t size;
    xen_vm_group_record *contents[];
} xen_vm_group_record_set;

/**
 * Allocate a xen_vm_group_record_set of the given size.
 */
extern xen_vm_group_record_set *
xen_vm_group_record_set_alloc(size_t size);

/**
 * Free the given xen_vm_group_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_vm_group_record_set_free(xen_vm_group_record_set *set);


typedef struct xen_vm_group_record_opt_set
{
    size_t size;
    xen_vm_group_record_opt *contents[];
} xen_vm_group_record_opt_set;

/**
 * Allocate a xen_vm_group_record_opt_set of the given size.
 */
extern xen_vm_group_record_opt_set *
xen_vm_group_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_vm_group_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_vm_group_record_opt_set_free(xen_vm_group_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * VM_group.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_record(xen_session *session, xen_vm_group_record **result, xen_vm_group vm_group);


/**
 * Get a reference to the VM_group instance with the specified
 * UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_by_uuid(xen_session *session, xen_vm_group *result, char *uuid);


/**
 * Create a new VM_group instance, and return its handle.
 * Minimum allowed role: vm-admin.
 */
extern bool
xen_vm_group_create(xen_session *session, xen_vm_group *result, xen_vm_group_record *record);


/**
 * Create a new VM_group instance, and return its handle.
 * Minimum allowed role: vm-admin.
 */
extern bool
xen_vm_group_create_async(xen_session *session, xen_task *result, xen_vm_group_record *record);


/**
 * Destroy the specified VM_group instance.
 * Minimum allowed role: vm-admin.
 */
extern bool
xen_vm_group_destroy(xen_session *session, xen_vm_group vm_group);


/**
 * Destroy the specified VM_group instance.
 * Minimum allowed role: vm-admin.
 */
extern bool
xen_vm_group_destroy_async(xen_session *session, xen_task *result, xen_vm_group vm_group);


/**
 * Get all the VM_group instances with the given label.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_by_name_label(xen_session *session, struct xen_vm_group_set **result, char *label);


/**
 * Get the uuid field of the given VM_group.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_uuid(xen_session *session, char **result, xen_vm_group vm_group);


/**
 * Get the name/label field of the given VM_group.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_name_label(xen_session *session, char **result, xen_vm_group vm_group);


/**
 * Get the name/description field of the given VM_group.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_name_description(xen_session *session, char **result, xen_vm_group vm_group);


/**
 * Get the placement field of the given VM_group.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_placement(xen_session *session, enum xen_placement_policy *result, xen_vm_group vm_group);


/**
 * Get the VMs field of the given VM_group.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_vms(xen_session *session, struct xen_vm_set **result, xen_vm_group vm_group);


/**
 * Set the name/label field of the given VM_group.
 * Minimum allowed role: vm-admin.
 */
extern bool
xen_vm_group_set_name_label(xen_session *session, xen_vm_group vm_group, char *label);


/**
 * Set the name/description field of the given VM_group.
 * Minimum allowed role: vm-admin.
 */
extern bool
xen_vm_group_set_name_description(xen_session *session, xen_vm_group vm_group, char *description);


/**
 * Return a list of all the VM_groups known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_all(xen_session *session, struct xen_vm_group_set **result);


/**
 * Return a map of VM_group references to VM_group records for all
 * VM_groups known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_vm_group_get_all_records(xen_session *session, xen_vm_group_xen_vm_group_record_map **result);


#endif
