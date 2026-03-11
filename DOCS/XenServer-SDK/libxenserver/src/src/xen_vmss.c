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
#include "xen_vmss_frequency_internal.h"
#include "xen_vmss_type_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_vmss.h>


XEN_FREE(xen_vmss)
XEN_SET_ALLOC_FREE(xen_vmss)
XEN_ALLOC(xen_vmss_record)
XEN_SET_ALLOC_FREE(xen_vmss_record)
XEN_ALLOC(xen_vmss_record_opt)
XEN_RECORD_OPT_FREE(xen_vmss)
XEN_SET_ALLOC_FREE(xen_vmss_record_opt)


static const struct_member xen_vmss_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vmss_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vmss_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vmss_record, name_description) },
        { .key = "enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_vmss_record, enabled) },
        { .key = "type",
          .type = &xen_vmss_type_abstract_type_,
          .offset = offsetof(xen_vmss_record, type) },
        { .key = "retained_snapshots",
          .type = &abstract_type_int,
          .offset = offsetof(xen_vmss_record, retained_snapshots) },
        { .key = "frequency",
          .type = &xen_vmss_frequency_abstract_type_,
          .offset = offsetof(xen_vmss_record, frequency) },
        { .key = "schedule",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_vmss_record, schedule) },
        { .key = "last_run_time",
          .type = &abstract_type_datetime,
          .offset = offsetof(xen_vmss_record, last_run_time) },
        { .key = "VMs",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_vmss_record, vms) }
    };


const abstract_type xen_vmss_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_vmss_record),
       .member_count =
           sizeof(xen_vmss_record_struct_members) / sizeof(struct_member),
       .members = xen_vmss_record_struct_members
    };


const abstract_type xen_vmss_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_vmss_record_abstract_type_
    };


static const struct struct_member xen_vmss_xen_vmss_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_vmss_xen_vmss_record_map_contents, key)
    },
    {
        .type = &xen_vmss_record_abstract_type_,
        .offset = offsetof(xen_vmss_xen_vmss_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_vmss_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_vmss_xen_vmss_record_map_contents),
    .members = xen_vmss_xen_vmss_record_members
};


void
xen_vmss_record_free(xen_vmss_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    xen_string_string_map_free(record->schedule);
    xen_vm_record_opt_set_free(record->vms);
    free(record);
}


bool
xen_vmss_get_record(xen_session *session, xen_vmss_record **result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = xen_vmss_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("VMSS.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_vmss_get_by_uuid(xen_session *session, xen_vmss *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VMSS.get_by_uuid");
    return session->ok;
}


bool
xen_vmss_create(xen_session *session, xen_vmss *result, xen_vmss_record *record)
{
    abstract_value param_values[] =
        {
            { .type = &xen_vmss_record_abstract_type_,
              .u.struct_val = record }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VMSS.create");
    return session->ok;
}


bool
xen_vmss_create_async(xen_session *session, xen_task *result, xen_vmss_record *record)
{
    abstract_value param_values[] =
        {
            { .type = &xen_vmss_record_abstract_type_,
              .u.struct_val = record }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VMSS.create");
    return session->ok;
}


bool
xen_vmss_destroy(xen_session *session, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    xen_call_(session, "VMSS.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_destroy_async(xen_session *session, xen_task *result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VMSS.destroy");
    return session->ok;
}


bool
xen_vmss_get_by_name_label(xen_session *session, struct xen_vmss_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("VMSS.get_by_name_label");
    return session->ok;
}


bool
xen_vmss_get_uuid(xen_session *session, char **result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VMSS.get_uuid");
    return session->ok;
}


bool
xen_vmss_get_name_label(xen_session *session, char **result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VMSS.get_name_label");
    return session->ok;
}


bool
xen_vmss_get_name_description(xen_session *session, char **result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VMSS.get_name_description");
    return session->ok;
}


bool
xen_vmss_get_enabled(xen_session *session, bool *result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("VMSS.get_enabled");
    return session->ok;
}


bool
xen_vmss_get_type(xen_session *session, enum xen_vmss_type *result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = xen_vmss_type_abstract_type_;

    XEN_CALL_("VMSS.get_type");
    return session->ok;
}


bool
xen_vmss_get_retained_snapshots(xen_session *session, int64_t *result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("VMSS.get_retained_snapshots");
    return session->ok;
}


bool
xen_vmss_get_frequency(xen_session *session, enum xen_vmss_frequency *result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = xen_vmss_frequency_abstract_type_;

    XEN_CALL_("VMSS.get_frequency");
    return session->ok;
}


bool
xen_vmss_get_schedule(xen_session *session, xen_string_string_map **result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("VMSS.get_schedule");
    return session->ok;
}


bool
xen_vmss_get_last_run_time(xen_session *session, time_t *result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_datetime;

    XEN_CALL_("VMSS.get_last_run_time");
    return session->ok;
}


bool
xen_vmss_get_vms(xen_session *session, struct xen_vm_set **result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("VMSS.get_VMs");
    return session->ok;
}


bool
xen_vmss_set_name_label(xen_session *session, xen_vmss vmss, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss },
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    xen_call_(session, "VMSS.set_name_label", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_set_name_description(xen_session *session, xen_vmss vmss, char *description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss },
            { .type = &abstract_type_string,
              .u.string_val = description }
        };

    xen_call_(session, "VMSS.set_name_description", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_set_enabled(xen_session *session, xen_vmss vmss, bool enabled)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss },
            { .type = &abstract_type_bool,
              .u.bool_val = enabled }
        };

    xen_call_(session, "VMSS.set_enabled", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_snapshot_now(xen_session *session, char **result, xen_vmss vmss)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vmss }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VMSS.snapshot_now");
    return session->ok;
}


bool
xen_vmss_set_retained_snapshots(xen_session *session, xen_vmss self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    xen_call_(session, "VMSS.set_retained_snapshots", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_set_frequency(xen_session *session, xen_vmss self, enum xen_vmss_frequency value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &xen_vmss_frequency_abstract_type_,
              .u.enum_val = value }
        };

    xen_call_(session, "VMSS.set_frequency", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_set_schedule(xen_session *session, xen_vmss self, xen_string_string_map *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)value }
        };

    xen_call_(session, "VMSS.set_schedule", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_add_to_schedule(xen_session *session, xen_vmss self, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "VMSS.add_to_schedule", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_remove_from_schedule(xen_session *session, xen_vmss self, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "VMSS.remove_from_schedule", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_set_last_run_time(xen_session *session, xen_vmss self, time_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_datetime,
              .u.datetime_val = value }
        };

    xen_call_(session, "VMSS.set_last_run_time", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_set_type(xen_session *session, xen_vmss self, enum xen_vmss_type value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &xen_vmss_type_abstract_type_,
              .u.enum_val = value }
        };

    xen_call_(session, "VMSS.set_type", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vmss_get_all(xen_session *session, struct xen_vmss_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "VMSS.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_vmss_get_all_records(xen_session *session, xen_vmss_xen_vmss_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_vmss_record_map;

    *result = NULL;
    xen_call_(session, "VMSS.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
