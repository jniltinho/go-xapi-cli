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
#include "xen_pvs_proxy_status_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_pvs_proxy.h>


XEN_FREE(xen_pvs_proxy)
XEN_SET_ALLOC_FREE(xen_pvs_proxy)
XEN_ALLOC(xen_pvs_proxy_record)
XEN_SET_ALLOC_FREE(xen_pvs_proxy_record)
XEN_ALLOC(xen_pvs_proxy_record_opt)
XEN_RECORD_OPT_FREE(xen_pvs_proxy)
XEN_SET_ALLOC_FREE(xen_pvs_proxy_record_opt)


static const struct_member xen_pvs_proxy_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pvs_proxy_record, uuid) },
        { .key = "site",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pvs_proxy_record, site) },
        { .key = "VIF",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pvs_proxy_record, vif) },
        { .key = "currently_attached",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pvs_proxy_record, currently_attached) },
        { .key = "status",
          .type = &xen_pvs_proxy_status_abstract_type_,
          .offset = offsetof(xen_pvs_proxy_record, status) }
    };


const abstract_type xen_pvs_proxy_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_pvs_proxy_record),
       .member_count =
           sizeof(xen_pvs_proxy_record_struct_members) / sizeof(struct_member),
       .members = xen_pvs_proxy_record_struct_members
    };


const abstract_type xen_pvs_proxy_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_pvs_proxy_record_abstract_type_
    };


static const struct struct_member xen_pvs_proxy_xen_pvs_proxy_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_pvs_proxy_xen_pvs_proxy_record_map_contents, key)
    },
    {
        .type = &xen_pvs_proxy_record_abstract_type_,
        .offset = offsetof(xen_pvs_proxy_xen_pvs_proxy_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_pvs_proxy_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_pvs_proxy_xen_pvs_proxy_record_map_contents),
    .members = xen_pvs_proxy_xen_pvs_proxy_record_members
};


void
xen_pvs_proxy_record_free(xen_pvs_proxy_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_pvs_site_record_opt_free(record->site);
    xen_vif_record_opt_free(record->vif);
    free(record);
}


bool
xen_pvs_proxy_get_record(xen_session *session, xen_pvs_proxy_record **result, xen_pvs_proxy pvs_proxy)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_proxy }
        };

    abstract_type result_type = xen_pvs_proxy_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("PVS_proxy.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_pvs_proxy_get_by_uuid(xen_session *session, xen_pvs_proxy *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_proxy.get_by_uuid");
    return session->ok;
}


bool
xen_pvs_proxy_get_uuid(xen_session *session, char **result, xen_pvs_proxy pvs_proxy)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_proxy }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_proxy.get_uuid");
    return session->ok;
}


bool
xen_pvs_proxy_get_site(xen_session *session, xen_pvs_site *result, xen_pvs_proxy pvs_proxy)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_proxy }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_proxy.get_site");
    return session->ok;
}


bool
xen_pvs_proxy_get_vif(xen_session *session, xen_vif *result, xen_pvs_proxy pvs_proxy)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_proxy }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_proxy.get_VIF");
    return session->ok;
}


bool
xen_pvs_proxy_get_currently_attached(xen_session *session, bool *result, xen_pvs_proxy pvs_proxy)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_proxy }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("PVS_proxy.get_currently_attached");
    return session->ok;
}


bool
xen_pvs_proxy_get_status(xen_session *session, enum xen_pvs_proxy_status *result, xen_pvs_proxy pvs_proxy)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_proxy }
        };

    abstract_type result_type = xen_pvs_proxy_status_abstract_type_;

    XEN_CALL_("PVS_proxy.get_status");
    return session->ok;
}


bool
xen_pvs_proxy_create(xen_session *session, xen_pvs_proxy *result, xen_pvs_site site, xen_vif vif)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = site },
            { .type = &abstract_type_string,
              .u.string_val = vif }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_proxy.create");
    return session->ok;
}


bool
xen_pvs_proxy_create_async(xen_session *session, xen_task *result, xen_pvs_site site, xen_vif vif)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = site },
            { .type = &abstract_type_string,
              .u.string_val = vif }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PVS_proxy.create");
    return session->ok;
}


bool
xen_pvs_proxy_destroy(xen_session *session, xen_pvs_proxy self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "PVS_proxy.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pvs_proxy_destroy_async(xen_session *session, xen_task *result, xen_pvs_proxy self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PVS_proxy.destroy");
    return session->ok;
}


bool
xen_pvs_proxy_get_all(xen_session *session, struct xen_pvs_proxy_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "PVS_proxy.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pvs_proxy_get_all_records(xen_session *session, xen_pvs_proxy_xen_pvs_proxy_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_pvs_proxy_record_map;

    *result = NULL;
    xen_call_(session, "PVS_proxy.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
