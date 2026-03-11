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
#include "xen_persistence_backend_internal.h"
#include "xen_string_vtpm_operations_map_internal.h"
#include "xen_vtpm_operations_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_vtpm.h>


XEN_FREE(xen_vtpm)
XEN_SET_ALLOC_FREE(xen_vtpm)
XEN_ALLOC(xen_vtpm_record)
XEN_SET_ALLOC_FREE(xen_vtpm_record)
XEN_ALLOC(xen_vtpm_record_opt)
XEN_RECORD_OPT_FREE(xen_vtpm)
XEN_SET_ALLOC_FREE(xen_vtpm_record_opt)


static const struct_member xen_vtpm_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vtpm_record, uuid) },
        { .key = "allowed_operations",
          .type = &xen_vtpm_operations_set_abstract_type_,
          .offset = offsetof(xen_vtpm_record, allowed_operations) },
        { .key = "current_operations",
          .type = &string_vtpm_operations_map_abstract_type_,
          .offset = offsetof(xen_vtpm_record, current_operations) },
        { .key = "VM",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_vtpm_record, vm) },
        { .key = "backend",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_vtpm_record, backend) },
        { .key = "persistence_backend",
          .type = &xen_persistence_backend_abstract_type_,
          .offset = offsetof(xen_vtpm_record, persistence_backend) },
        { .key = "is_unique",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_vtpm_record, is_unique) },
        { .key = "is_protected",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_vtpm_record, is_protected) }
    };


const abstract_type xen_vtpm_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_vtpm_record),
       .member_count =
           sizeof(xen_vtpm_record_struct_members) / sizeof(struct_member),
       .members = xen_vtpm_record_struct_members
    };


const abstract_type xen_vtpm_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_vtpm_record_abstract_type_
    };


static const struct struct_member xen_vtpm_xen_vtpm_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_vtpm_xen_vtpm_record_map_contents, key)
    },
    {
        .type = &xen_vtpm_record_abstract_type_,
        .offset = offsetof(xen_vtpm_xen_vtpm_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_vtpm_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_vtpm_xen_vtpm_record_map_contents),
    .members = xen_vtpm_xen_vtpm_record_members
};


void
xen_vtpm_record_free(xen_vtpm_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_vtpm_operations_set_free(record->allowed_operations);
    xen_string_vtpm_operations_map_free(record->current_operations);
    xen_vm_record_opt_free(record->vm);
    xen_vm_record_opt_free(record->backend);
    free(record);
}


bool
xen_vtpm_get_record(xen_session *session, xen_vtpm_record **result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = xen_vtpm_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("VTPM.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_vtpm_get_by_uuid(xen_session *session, xen_vtpm *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VTPM.get_by_uuid");
    return session->ok;
}


bool
xen_vtpm_get_uuid(xen_session *session, char **result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VTPM.get_uuid");
    return session->ok;
}


bool
xen_vtpm_get_allowed_operations(xen_session *session, struct xen_vtpm_operations_set **result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = xen_vtpm_operations_set_abstract_type_;

    *result = NULL;
    XEN_CALL_("VTPM.get_allowed_operations");
    return session->ok;
}


bool
xen_vtpm_get_current_operations(xen_session *session, xen_string_vtpm_operations_map **result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = string_vtpm_operations_map_abstract_type_;

    *result = NULL;
    XEN_CALL_("VTPM.get_current_operations");
    return session->ok;
}


bool
xen_vtpm_get_vm(xen_session *session, xen_vm *result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VTPM.get_VM");
    return session->ok;
}


bool
xen_vtpm_get_backend(xen_session *session, xen_vm *result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VTPM.get_backend");
    return session->ok;
}


bool
xen_vtpm_get_persistence_backend(xen_session *session, enum xen_persistence_backend *result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = xen_persistence_backend_abstract_type_;

    XEN_CALL_("VTPM.get_persistence_backend");
    return session->ok;
}


bool
xen_vtpm_get_is_unique(xen_session *session, bool *result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("VTPM.get_is_unique");
    return session->ok;
}


bool
xen_vtpm_get_is_protected(xen_session *session, bool *result, xen_vtpm vtpm)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vtpm }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("VTPM.get_is_protected");
    return session->ok;
}


bool
xen_vtpm_create(xen_session *session, xen_vtpm *result, xen_vm vm, bool is_unique)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm },
            { .type = &abstract_type_bool,
              .u.bool_val = is_unique }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VTPM.create");
    return session->ok;
}


bool
xen_vtpm_create_async(xen_session *session, xen_task *result, xen_vm vm, bool is_unique)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm },
            { .type = &abstract_type_bool,
              .u.bool_val = is_unique }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VTPM.create");
    return session->ok;
}


bool
xen_vtpm_destroy(xen_session *session, xen_vtpm self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "VTPM.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_vtpm_destroy_async(xen_session *session, xen_task *result, xen_vtpm self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VTPM.destroy");
    return session->ok;
}


bool
xen_vtpm_get_all(xen_session *session, struct xen_vtpm_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "VTPM.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_vtpm_get_all_records(xen_session *session, xen_vtpm_xen_vtpm_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_vtpm_record_map;

    *result = NULL;
    xen_call_(session, "VTPM.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
