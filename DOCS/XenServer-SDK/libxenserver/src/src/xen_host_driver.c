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
#include <xen/api/xen_common.h>
#include <xen/api/xen_host_driver.h>


XEN_FREE(xen_host_driver)
XEN_SET_ALLOC_FREE(xen_host_driver)
XEN_ALLOC(xen_host_driver_record)
XEN_SET_ALLOC_FREE(xen_host_driver_record)
XEN_ALLOC(xen_host_driver_record_opt)
XEN_RECORD_OPT_FREE(xen_host_driver)
XEN_SET_ALLOC_FREE(xen_host_driver_record_opt)


static const struct_member xen_host_driver_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_host_driver_record, uuid) },
        { .key = "host",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_host_driver_record, host) },
        { .key = "name",
          .type = &abstract_type_string,
          .offset = offsetof(xen_host_driver_record, name) },
        { .key = "friendly_name",
          .type = &abstract_type_string,
          .offset = offsetof(xen_host_driver_record, friendly_name) },
        { .key = "variants",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_host_driver_record, variants) },
        { .key = "active_variant",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_host_driver_record, active_variant) },
        { .key = "selected_variant",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_host_driver_record, selected_variant) },
        { .key = "type",
          .type = &abstract_type_string,
          .offset = offsetof(xen_host_driver_record, type) },
        { .key = "description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_host_driver_record, description) },
        { .key = "info",
          .type = &abstract_type_string,
          .offset = offsetof(xen_host_driver_record, info) }
    };


const abstract_type xen_host_driver_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_host_driver_record),
       .member_count =
           sizeof(xen_host_driver_record_struct_members) / sizeof(struct_member),
       .members = xen_host_driver_record_struct_members
    };


const abstract_type xen_host_driver_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_host_driver_record_abstract_type_
    };


static const struct struct_member xen_host_driver_xen_host_driver_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_host_driver_xen_host_driver_record_map_contents, key)
    },
    {
        .type = &xen_host_driver_record_abstract_type_,
        .offset = offsetof(xen_host_driver_xen_host_driver_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_host_driver_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_host_driver_xen_host_driver_record_map_contents),
    .members = xen_host_driver_xen_host_driver_record_members
};


void
xen_host_driver_record_free(xen_host_driver_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_host_record_opt_free(record->host);
    free(record->name);
    free(record->friendly_name);
    xen_driver_variant_record_opt_set_free(record->variants);
    xen_driver_variant_record_opt_free(record->active_variant);
    xen_driver_variant_record_opt_free(record->selected_variant);
    free(record->type);
    free(record->description);
    free(record->info);
    free(record);
}


bool
xen_host_driver_get_record(xen_session *session, xen_host_driver_record **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = xen_host_driver_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("Host_driver.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_host_driver_get_by_uuid(xen_session *session, xen_host_driver *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_by_uuid");
    return session->ok;
}


bool
xen_host_driver_get_uuid(xen_session *session, char **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_uuid");
    return session->ok;
}


bool
xen_host_driver_get_host(xen_session *session, xen_host *result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_host");
    return session->ok;
}


bool
xen_host_driver_get_name(xen_session *session, char **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_name");
    return session->ok;
}


bool
xen_host_driver_get_friendly_name(xen_session *session, char **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_friendly_name");
    return session->ok;
}


bool
xen_host_driver_get_variants(xen_session *session, struct xen_driver_variant_set **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Host_driver.get_variants");
    return session->ok;
}


bool
xen_host_driver_get_active_variant(xen_session *session, xen_driver_variant *result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_active_variant");
    return session->ok;
}


bool
xen_host_driver_get_selected_variant(xen_session *session, xen_driver_variant *result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_selected_variant");
    return session->ok;
}


bool
xen_host_driver_get_type(xen_session *session, char **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_type");
    return session->ok;
}


bool
xen_host_driver_get_description(xen_session *session, char **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_description");
    return session->ok;
}


bool
xen_host_driver_get_info(xen_session *session, char **result, xen_host_driver host_driver)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host_driver }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Host_driver.get_info");
    return session->ok;
}


bool
xen_host_driver_select(xen_session *session, xen_host_driver self, xen_driver_variant variant)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = variant }
        };

    xen_call_(session, "Host_driver.select", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_host_driver_select_async(xen_session *session, xen_task *result, xen_host_driver self, xen_driver_variant variant)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = variant }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Host_driver.select");
    return session->ok;
}


bool
xen_host_driver_deselect(xen_session *session, xen_host_driver self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Host_driver.deselect", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_host_driver_deselect_async(xen_session *session, xen_task *result, xen_host_driver self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Host_driver.deselect");
    return session->ok;
}


bool
xen_host_driver_rescan(xen_session *session, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    xen_call_(session, "Host_driver.rescan", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_host_driver_rescan_async(xen_session *session, xen_task *result, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Host_driver.rescan");
    return session->ok;
}


bool
xen_host_driver_get_all(xen_session *session, struct xen_host_driver_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "Host_driver.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_host_driver_get_all_records(xen_session *session, xen_host_driver_xen_host_driver_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_host_driver_record_map;

    *result = NULL;
    xen_call_(session, "Host_driver.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
