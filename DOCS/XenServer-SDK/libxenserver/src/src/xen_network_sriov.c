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
#include "xen_sriov_configuration_mode_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_network_sriov.h>


XEN_FREE(xen_network_sriov)
XEN_SET_ALLOC_FREE(xen_network_sriov)
XEN_ALLOC(xen_network_sriov_record)
XEN_SET_ALLOC_FREE(xen_network_sriov_record)
XEN_ALLOC(xen_network_sriov_record_opt)
XEN_RECORD_OPT_FREE(xen_network_sriov)
XEN_SET_ALLOC_FREE(xen_network_sriov_record_opt)


static const struct_member xen_network_sriov_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_network_sriov_record, uuid) },
        { .key = "physical_PIF",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_network_sriov_record, physical_pif) },
        { .key = "logical_PIF",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_network_sriov_record, logical_pif) },
        { .key = "requires_reboot",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_network_sriov_record, requires_reboot) },
        { .key = "configuration_mode",
          .type = &xen_sriov_configuration_mode_abstract_type_,
          .offset = offsetof(xen_network_sriov_record, configuration_mode) }
    };


const abstract_type xen_network_sriov_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_network_sriov_record),
       .member_count =
           sizeof(xen_network_sriov_record_struct_members) / sizeof(struct_member),
       .members = xen_network_sriov_record_struct_members
    };


const abstract_type xen_network_sriov_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_network_sriov_record_abstract_type_
    };


static const struct struct_member xen_network_sriov_xen_network_sriov_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_network_sriov_xen_network_sriov_record_map_contents, key)
    },
    {
        .type = &xen_network_sriov_record_abstract_type_,
        .offset = offsetof(xen_network_sriov_xen_network_sriov_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_network_sriov_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_network_sriov_xen_network_sriov_record_map_contents),
    .members = xen_network_sriov_xen_network_sriov_record_members
};


void
xen_network_sriov_record_free(xen_network_sriov_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_pif_record_opt_free(record->physical_pif);
    xen_pif_record_opt_free(record->logical_pif);
    free(record);
}


bool
xen_network_sriov_get_record(xen_session *session, xen_network_sriov_record **result, xen_network_sriov network_sriov)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network_sriov }
        };

    abstract_type result_type = xen_network_sriov_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("network_sriov.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_network_sriov_get_by_uuid(xen_session *session, xen_network_sriov *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("network_sriov.get_by_uuid");
    return session->ok;
}


bool
xen_network_sriov_get_uuid(xen_session *session, char **result, xen_network_sriov network_sriov)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network_sriov }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("network_sriov.get_uuid");
    return session->ok;
}


bool
xen_network_sriov_get_physical_pif(xen_session *session, xen_pif *result, xen_network_sriov network_sriov)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network_sriov }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("network_sriov.get_physical_PIF");
    return session->ok;
}


bool
xen_network_sriov_get_logical_pif(xen_session *session, xen_pif *result, xen_network_sriov network_sriov)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network_sriov }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("network_sriov.get_logical_PIF");
    return session->ok;
}


bool
xen_network_sriov_get_requires_reboot(xen_session *session, bool *result, xen_network_sriov network_sriov)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network_sriov }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("network_sriov.get_requires_reboot");
    return session->ok;
}


bool
xen_network_sriov_get_configuration_mode(xen_session *session, enum xen_sriov_configuration_mode *result, xen_network_sriov network_sriov)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network_sriov }
        };

    abstract_type result_type = xen_sriov_configuration_mode_abstract_type_;

    XEN_CALL_("network_sriov.get_configuration_mode");
    return session->ok;
}


bool
xen_network_sriov_create(xen_session *session, xen_network_sriov *result, xen_pif pif, xen_network network)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pif },
            { .type = &abstract_type_string,
              .u.string_val = network }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("network_sriov.create");
    return session->ok;
}


bool
xen_network_sriov_create_async(xen_session *session, xen_task *result, xen_pif pif, xen_network network)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pif },
            { .type = &abstract_type_string,
              .u.string_val = network }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.network_sriov.create");
    return session->ok;
}


bool
xen_network_sriov_destroy(xen_session *session, xen_network_sriov self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "network_sriov.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_network_sriov_destroy_async(xen_session *session, xen_task *result, xen_network_sriov self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.network_sriov.destroy");
    return session->ok;
}


bool
xen_network_sriov_get_remaining_capacity(xen_session *session, int64_t *result, xen_network_sriov self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("network_sriov.get_remaining_capacity");
    return session->ok;
}


bool
xen_network_sriov_get_remaining_capacity_async(xen_session *session, xen_task *result, xen_network_sriov self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.network_sriov.get_remaining_capacity");
    return session->ok;
}


bool
xen_network_sriov_get_all(xen_session *session, struct xen_network_sriov_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "network_sriov.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_network_sriov_get_all_records(xen_session *session, xen_network_sriov_xen_network_sriov_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_network_sriov_record_map;

    *result = NULL;
    xen_call_(session, "network_sriov.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
