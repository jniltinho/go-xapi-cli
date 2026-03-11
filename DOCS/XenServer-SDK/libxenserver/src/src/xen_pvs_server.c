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
#include <xen/api/xen_pvs_server.h>


XEN_FREE(xen_pvs_server)
XEN_SET_ALLOC_FREE(xen_pvs_server)
XEN_ALLOC(xen_pvs_server_record)
XEN_SET_ALLOC_FREE(xen_pvs_server_record)
XEN_ALLOC(xen_pvs_server_record_opt)
XEN_RECORD_OPT_FREE(xen_pvs_server)
XEN_SET_ALLOC_FREE(xen_pvs_server_record_opt)


static const struct_member xen_pvs_server_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pvs_server_record, uuid) },
        { .key = "addresses",
          .type = &abstract_type_string_set,
          .offset = offsetof(xen_pvs_server_record, addresses) },
        { .key = "first_port",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pvs_server_record, first_port) },
        { .key = "last_port",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pvs_server_record, last_port) },
        { .key = "site",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pvs_server_record, site) }
    };


const abstract_type xen_pvs_server_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_pvs_server_record),
       .member_count =
           sizeof(xen_pvs_server_record_struct_members) / sizeof(struct_member),
       .members = xen_pvs_server_record_struct_members
    };


const abstract_type xen_pvs_server_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_pvs_server_record_abstract_type_
    };


static const struct struct_member xen_pvs_server_xen_pvs_server_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_pvs_server_xen_pvs_server_record_map_contents, key)
    },
    {
        .type = &xen_pvs_server_record_abstract_type_,
        .offset = offsetof(xen_pvs_server_xen_pvs_server_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_pvs_server_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_pvs_server_xen_pvs_server_record_map_contents),
    .members = xen_pvs_server_xen_pvs_server_record_members
};


void
xen_pvs_server_record_free(xen_pvs_server_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_string_set_free(record->addresses);
    xen_pvs_site_record_opt_free(record->site);
    free(record);
}


bool
xen_pvs_server_get_record(xen_session *session, xen_pvs_server_record **result, xen_pvs_server pvs_server)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_server }
        };

    abstract_type result_type = xen_pvs_server_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("PVS_server.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_pvs_server_get_by_uuid(xen_session *session, xen_pvs_server *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_server.get_by_uuid");
    return session->ok;
}


bool
xen_pvs_server_get_uuid(xen_session *session, char **result, xen_pvs_server pvs_server)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_server }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_server.get_uuid");
    return session->ok;
}


bool
xen_pvs_server_get_addresses(xen_session *session, struct xen_string_set **result, xen_pvs_server pvs_server)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_server }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("PVS_server.get_addresses");
    return session->ok;
}


bool
xen_pvs_server_get_first_port(xen_session *session, int64_t *result, xen_pvs_server pvs_server)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_server }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("PVS_server.get_first_port");
    return session->ok;
}


bool
xen_pvs_server_get_last_port(xen_session *session, int64_t *result, xen_pvs_server pvs_server)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_server }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("PVS_server.get_last_port");
    return session->ok;
}


bool
xen_pvs_server_get_site(xen_session *session, xen_pvs_site *result, xen_pvs_server pvs_server)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pvs_server }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_server.get_site");
    return session->ok;
}


bool
xen_pvs_server_introduce(xen_session *session, xen_pvs_server *result, struct xen_string_set *addresses, int64_t first_port, int64_t last_port, xen_pvs_site site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)addresses },
            { .type = &abstract_type_int,
              .u.int_val = first_port },
            { .type = &abstract_type_int,
              .u.int_val = last_port },
            { .type = &abstract_type_string,
              .u.string_val = site }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PVS_server.introduce");
    return session->ok;
}


bool
xen_pvs_server_introduce_async(xen_session *session, xen_task *result, struct xen_string_set *addresses, int64_t first_port, int64_t last_port, xen_pvs_site site)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)addresses },
            { .type = &abstract_type_int,
              .u.int_val = first_port },
            { .type = &abstract_type_int,
              .u.int_val = last_port },
            { .type = &abstract_type_string,
              .u.string_val = site }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PVS_server.introduce");
    return session->ok;
}


bool
xen_pvs_server_forget(xen_session *session, xen_pvs_server self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "PVS_server.forget", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pvs_server_forget_async(xen_session *session, xen_task *result, xen_pvs_server self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PVS_server.forget");
    return session->ok;
}


bool
xen_pvs_server_get_all(xen_session *session, struct xen_pvs_server_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "PVS_server.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pvs_server_get_all_records(xen_session *session, xen_pvs_server_xen_pvs_server_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_pvs_server_record_map;

    *result = NULL;
    xen_call_(session, "PVS_server.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
