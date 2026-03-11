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
#include <xen/api/xen_pvs_site.h>


XEN_FREE(xen_pvs_site)
XEN_SET_ALLOC_FREE(xen_pvs_site)
XEN_ALLOC(xen_pvs_site_record)
XEN_SET_ALLOC_FREE(xen_pvs_site_record)
XEN_ALLOC(xen_pvs_site_record_opt)
XEN_RECORD_OPT_FREE(xen_pvs_site)
XEN_SET_ALLOC_FREE(xen_pvs_site_record_opt)


static const struct_member xen_pvs_site_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pvs_site_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pvs_site_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pvs_site_record, name_description) },
        { .key = "PVS_uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pvs_site_record, pvs_uuid) },
        { .key = "cache_storage",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_pvs_site_record, cache_storage) },
        { .key = "servers",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_pvs_site_record, servers) },
        { .key = "proxies",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_pvs_site_record, proxies) }
    };


const abstract_type xen_pvs_site_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_pvs_site_record),
       .member_count =
           sizeof(xen_pvs_site_record_struct_members) / sizeof(struct_member),
       .members = xen_pvs_site_record_struct_members
    };


const abstract_type xen_pvs_site_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_pvs_site_record_abstract_type_
    };


static const struct struct_member xen_pvs_site_xen_pvs_site_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_pvs_site_xen_pvs_site_record_map_contents, key)
    },
    {
        .type = &xen_pvs_site_record_abstract_type_,
        .offset = offsetof(xen_pvs_site_xen_pvs_site_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_pvs_site_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_pvs_site_xen_pvs_site_record_map_contents),
    .members = xen_pvs_site_xen_pvs_site_record_members
};


void
xen_pvs_site_record_free(xen_pvs_site_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    free(record->pvs_uuid);
    xen_pvs_cache_storage_record_opt_set_free(record->cache_storage);
    xen_pvs_server_record_opt_set_free(record->servers);
    xen_pvs_proxy_record_opt_set_free(record->proxies);
    free(record);
}


bool
xen_pvs_site_get_record(xen_session *session, xen_pvs_site_record **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = xen_pvs_site_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("PVS_site.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_pvs_site_get_by_uuid(xen_session *session, xen_pvs_site *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_site.get_by_uuid");
    return session->ok;
}


bool
xen_pvs_site_get_by_name_label(xen_session *session, struct xen_pvs_site_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("PVS_site.get_by_name_label");
    return session->ok;
}


bool
xen_pvs_site_get_uuid(xen_session *session, char **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_site.get_uuid");
    return session->ok;
}


bool
xen_pvs_site_get_name_label(xen_session *session, char **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_site.get_name_label");
    return session->ok;
}


bool
xen_pvs_site_get_name_description(xen_session *session, char **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_site.get_name_description");
    return session->ok;
}


bool
xen_pvs_site_get_pvs_uuid(xen_session *session, char **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_site.get_PVS_uuid");
    return session->ok;
}


bool
xen_pvs_site_get_cache_storage(xen_session *session, struct xen_pvs_cache_storage_set **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("PVS_site.get_cache_storage");
    return session->ok;
}


bool
xen_pvs_site_get_servers(xen_session *session, struct xen_pvs_server_set **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("PVS_site.get_servers");
    return session->ok;
}


bool
xen_pvs_site_get_proxies(xen_session *session, struct xen_pvs_proxy_set **result, xen_pvs_site pvs_site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("PVS_site.get_proxies");
    return session->ok;
}


bool
xen_pvs_site_set_name_label(xen_session *session, xen_pvs_site pvs_site, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site },
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    xen_call_(session, "PVS_site.set_name_label", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pvs_site_set_name_description(xen_session *session, xen_pvs_site pvs_site, char *description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_site },
            { .type = &abstract_type_string,
              .u.string_val = description }
        };

    xen_call_(session, "PVS_site.set_name_description", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pvs_site_introduce(xen_session *session, xen_pvs_site *result, char *name_label, char *name_description, char *pvs_uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string,
              .u.string_val = pvs_uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_site.introduce");
    return session->ok;
}


bool
xen_pvs_site_introduce_async(xen_session *session, xen_task *result, char *name_label, char *name_description, char *pvs_uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string,
              .u.string_val = pvs_uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PVS_site.introduce");
    return session->ok;
}


bool
xen_pvs_site_forget(xen_session *session, xen_pvs_site self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "PVS_site.forget", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pvs_site_forget_async(xen_session *session, xen_task *result, xen_pvs_site self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PVS_site.forget");
    return session->ok;
}


bool
xen_pvs_site_set_pvs_uuid(xen_session *session, xen_pvs_site self, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "PVS_site.set_PVS_uuid", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pvs_site_set_pvs_uuid_async(xen_session *session, xen_task *result, xen_pvs_site self, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PVS_site.set_PVS_uuid");
    return session->ok;
}


bool
xen_pvs_site_get_all(xen_session *session, struct xen_pvs_site_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "PVS_site.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pvs_site_get_all_records(xen_session *session, xen_pvs_site_xen_pvs_site_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_pvs_site_record_map;

    *result = NULL;
    xen_call_(session, "PVS_site.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
