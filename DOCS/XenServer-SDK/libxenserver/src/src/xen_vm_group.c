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


#include <stddef.h>
#include <stdlib.h>

#include "xen_internal.h"
#include "xen_placement_policy_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_vm_group.h>


XEN_FREE(xen_vm_group)
XEN_SET_ALLOC_FREE(xen_vm_group)
XEN_ALLOC(xen_vm_group_record)
XEN_SET_ALLOC_FREE(xen_vm_group_record)
XEN_ALLOC(xen_vm_group_record_opt)
XEN_RECORD_OPT_FREE(xen_vm_group)
XEN_SET_ALLOC_FREE(xen_vm_group_record_opt)


static const struct_member xen_vm_group_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vm_group_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vm_group_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vm_group_record, name_description) },
        { .key = "placement",
          .type = &xen_placement_policy_abstract_type_,
          .offset = offsetof(xen_vm_group_record, placement) },
        { .key = "VMs",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_vm_group_record, vms) }
    };


const abstract_type xen_vm_group_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_vm_group_record),
       .member_count =
           sizeof(xen_vm_group_record_struct_members) / sizeof(struct_member),
       .members = xen_vm_group_record_struct_members
    };


const abstract_type xen_vm_group_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_vm_group_record_abstract_type_
    };


static const struct struct_member xen_vm_group_xen_vm_group_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_vm_group_xen_vm_group_record_map_contents, key)
    },
    {
        .type = &xen_vm_group_record_abstract_type_,
        .offset = offsetof(xen_vm_group_xen_vm_group_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_vm_group_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_vm_group_xen_vm_group_record_map_contents),
    .members = xen_vm_group_xen_vm_group_record_members
};


void
xen_vm_group_record_free(xen_vm_group_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    xen_vm_record_opt_set_free(record->vms);
    free(record);
}


bool
xen_vm_group_get_record(xen_session *session, xen_vm_group_record **result, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    abstract_type result_type = xen_vm_group_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("VM_group.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_vm_group_get_by_uuid(xen_session *session, xen_vm_group *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VM_group.get_by_uuid");
    return session->ok;
}


bool
xen_vm_group_create(xen_session *session, xen_vm_group *result, xen_vm_group_record *record)
{
    abstract_value param_values[] =
        {
            { .type = &xen_vm_group_record_abstract_type_,
              .u.struct_val = record }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VM_group.create");
    return session->ok;
}


bool
xen_vm_group_create_async(xen_session *session, xen_task *result, xen_vm_group_record *record)
{
    abstract_value param_values[] =
        {
            { .type = &xen_vm_group_record_abstract_type_,
              .u.struct_val = record }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VM_group.create");
    return session->ok;
}


bool
xen_vm_group_destroy(xen_session *session, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    xen_call_(session, "VM_group.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_vm_group_destroy_async(xen_session *session, xen_task *result, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VM_group.destroy");
    return session->ok;
}


bool
xen_vm_group_get_by_name_label(xen_session *session, struct xen_vm_group_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("VM_group.get_by_name_label");
    return session->ok;
}


bool
xen_vm_group_get_uuid(xen_session *session, char **result, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VM_group.get_uuid");
    return session->ok;
}


bool
xen_vm_group_get_name_label(xen_session *session, char **result, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VM_group.get_name_label");
    return session->ok;
}


bool
xen_vm_group_get_name_description(xen_session *session, char **result, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VM_group.get_name_description");
    return session->ok;
}


bool
xen_vm_group_get_placement(xen_session *session, enum xen_placement_policy *result, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    abstract_type result_type = xen_placement_policy_abstract_type_;

    XEN_CALL_("VM_group.get_placement");
    return session->ok;
}


bool
xen_vm_group_get_vms(xen_session *session, struct xen_vm_set **result, xen_vm_group vm_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("VM_group.get_VMs");
    return session->ok;
}


bool
xen_vm_group_set_name_label(xen_session *session, xen_vm_group vm_group, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group },
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    xen_call_(session, "VM_group.set_name_label", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vm_group_set_name_description(xen_session *session, xen_vm_group vm_group, char *description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm_group },
            { .type = &abstract_type_string,
              .u.string_val = description }
        };

    xen_call_(session, "VM_group.set_name_description", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vm_group_get_all(xen_session *session, struct xen_vm_group_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "VM_group.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_vm_group_get_all_records(xen_session *session, xen_vm_group_xen_vm_group_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_vm_group_record_map;

    *result = NULL;
    xen_call_(session, "VM_group.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
