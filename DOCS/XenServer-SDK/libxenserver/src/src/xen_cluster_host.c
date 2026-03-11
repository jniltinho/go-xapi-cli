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

#include "xen_cluster_host_operation_internal.h"
#include "xen_internal.h"
#include "xen_string_cluster_host_operation_map_internal.h"
#include <xen/api/xen_cluster_host.h>
#include <xen/api/xen_common.h>


XEN_FREE(xen_cluster_host)
XEN_SET_ALLOC_FREE(xen_cluster_host)
XEN_ALLOC(xen_cluster_host_record)
XEN_SET_ALLOC_FREE(xen_cluster_host_record)
XEN_ALLOC(xen_cluster_host_record_opt)
XEN_RECORD_OPT_FREE(xen_cluster_host)
XEN_SET_ALLOC_FREE(xen_cluster_host_record_opt)


static const struct_member xen_cluster_host_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_cluster_host_record, uuid) },
        { .key = "cluster",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_cluster_host_record, cluster) },
        { .key = "host",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_cluster_host_record, host) },
        { .key = "enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_cluster_host_record, enabled) },
        { .key = "PIF",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_cluster_host_record, pif) },
        { .key = "joined",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_cluster_host_record, joined) },
        { .key = "live",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_cluster_host_record, live) },
        { .key = "last_update_live",
          .type = &abstract_type_datetime,
          .offset = offsetof(xen_cluster_host_record, last_update_live) },
        { .key = "allowed_operations",
          .type = &xen_cluster_host_operation_set_abstract_type_,
          .offset = offsetof(xen_cluster_host_record, allowed_operations) },
        { .key = "current_operations",
          .type = &string_cluster_host_operation_map_abstract_type_,
          .offset = offsetof(xen_cluster_host_record, current_operations) },
        { .key = "other_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_cluster_host_record, other_config) }
    };


const abstract_type xen_cluster_host_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_cluster_host_record),
       .member_count =
           sizeof(xen_cluster_host_record_struct_members) / sizeof(struct_member),
       .members = xen_cluster_host_record_struct_members
    };


const abstract_type xen_cluster_host_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_cluster_host_record_abstract_type_
    };


static const struct struct_member xen_cluster_host_xen_cluster_host_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_cluster_host_xen_cluster_host_record_map_contents, key)
    },
    {
        .type = &xen_cluster_host_record_abstract_type_,
        .offset = offsetof(xen_cluster_host_xen_cluster_host_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_cluster_host_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_cluster_host_xen_cluster_host_record_map_contents),
    .members = xen_cluster_host_xen_cluster_host_record_members
};


void
xen_cluster_host_record_free(xen_cluster_host_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_cluster_record_opt_free(record->cluster);
    xen_host_record_opt_free(record->host);
    xen_pif_record_opt_free(record->pif);
    xen_cluster_host_operation_set_free(record->allowed_operations);
    xen_string_cluster_host_operation_map_free(record->current_operations);
    xen_string_string_map_free(record->other_config);
    free(record);
}


bool
xen_cluster_host_get_record(xen_session *session, xen_cluster_host_record **result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = xen_cluster_host_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_cluster_host_get_by_uuid(xen_session *session, xen_cluster_host *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_by_uuid");
    return session->ok;
}


bool
xen_cluster_host_get_uuid(xen_session *session, char **result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_uuid");
    return session->ok;
}


bool
xen_cluster_host_get_cluster(xen_session *session, xen_cluster *result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_cluster");
    return session->ok;
}


bool
xen_cluster_host_get_host(xen_session *session, xen_host *result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_host");
    return session->ok;
}


bool
xen_cluster_host_get_enabled(xen_session *session, bool *result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Cluster_host.get_enabled");
    return session->ok;
}


bool
xen_cluster_host_get_pif(xen_session *session, xen_pif *result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_PIF");
    return session->ok;
}


bool
xen_cluster_host_get_joined(xen_session *session, bool *result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Cluster_host.get_joined");
    return session->ok;
}


bool
xen_cluster_host_get_live(xen_session *session, bool *result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Cluster_host.get_live");
    return session->ok;
}


bool
xen_cluster_host_get_last_update_live(xen_session *session, time_t *result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_datetime;

    XEN_CALL_("Cluster_host.get_last_update_live");
    return session->ok;
}


bool
xen_cluster_host_get_allowed_operations(xen_session *session, struct xen_cluster_host_operation_set **result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = xen_cluster_host_operation_set_abstract_type_;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_allowed_operations");
    return session->ok;
}


bool
xen_cluster_host_get_current_operations(xen_session *session, xen_string_cluster_host_operation_map **result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = string_cluster_host_operation_map_abstract_type_;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_current_operations");
    return session->ok;
}


bool
xen_cluster_host_get_other_config(xen_session *session, xen_string_string_map **result, xen_cluster_host cluster_host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster_host }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("Cluster_host.get_other_config");
    return session->ok;
}


bool
xen_cluster_host_create(xen_session *session, xen_cluster_host *result, xen_cluster cluster, xen_host host, xen_pif pif)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster },
            { .type = &abstract_type_string,
              .u.string_val = host },
            { .type = &abstract_type_string,
              .u.string_val = pif }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster_host.create");
    return session->ok;
}


bool
xen_cluster_host_create_async(xen_session *session, xen_task *result, xen_cluster cluster, xen_host host, xen_pif pif)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster },
            { .type = &abstract_type_string,
              .u.string_val = host },
            { .type = &abstract_type_string,
              .u.string_val = pif }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster_host.create");
    return session->ok;
}


bool
xen_cluster_host_destroy(xen_session *session, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster_host.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_host_destroy_async(xen_session *session, xen_task *result, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster_host.destroy");
    return session->ok;
}


bool
xen_cluster_host_enable(xen_session *session, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster_host.enable", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_host_enable_async(xen_session *session, xen_task *result, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster_host.enable");
    return session->ok;
}


bool
xen_cluster_host_force_destroy(xen_session *session, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster_host.force_destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_host_force_destroy_async(xen_session *session, xen_task *result, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster_host.force_destroy");
    return session->ok;
}


bool
xen_cluster_host_disable(xen_session *session, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster_host.disable", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_host_disable_async(xen_session *session, xen_task *result, xen_cluster_host self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster_host.disable");
    return session->ok;
}


bool
xen_cluster_host_get_all(xen_session *session, struct xen_cluster_host_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "Cluster_host.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_cluster_host_get_all_records(xen_session *session, xen_cluster_host_xen_cluster_host_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_cluster_host_record_map;

    *result = NULL;
    xen_call_(session, "Cluster_host.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
