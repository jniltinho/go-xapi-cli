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

#include "xen_cluster_operation_internal.h"
#include "xen_internal.h"
#include "xen_string_cluster_operation_map_internal.h"
#include <xen/api/xen_cluster.h>
#include <xen/api/xen_common.h>


XEN_FREE(xen_cluster)
XEN_SET_ALLOC_FREE(xen_cluster)
XEN_ALLOC(xen_cluster_record)
XEN_SET_ALLOC_FREE(xen_cluster_record)
XEN_ALLOC(xen_cluster_record_opt)
XEN_RECORD_OPT_FREE(xen_cluster)
XEN_SET_ALLOC_FREE(xen_cluster_record_opt)


static const struct_member xen_cluster_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_cluster_record, uuid) },
        { .key = "cluster_hosts",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_cluster_record, cluster_hosts) },
        { .key = "pending_forget",
          .type = &abstract_type_string_set,
          .offset = offsetof(xen_cluster_record, pending_forget) },
        { .key = "cluster_token",
          .type = &abstract_type_string,
          .offset = offsetof(xen_cluster_record, cluster_token) },
        { .key = "cluster_stack",
          .type = &abstract_type_string,
          .offset = offsetof(xen_cluster_record, cluster_stack) },
        { .key = "cluster_stack_version",
          .type = &abstract_type_int,
          .offset = offsetof(xen_cluster_record, cluster_stack_version) },
        { .key = "is_quorate",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_cluster_record, is_quorate) },
        { .key = "quorum",
          .type = &abstract_type_int,
          .offset = offsetof(xen_cluster_record, quorum) },
        { .key = "live_hosts",
          .type = &abstract_type_int,
          .offset = offsetof(xen_cluster_record, live_hosts) },
        { .key = "expected_hosts",
          .type = &abstract_type_int,
          .offset = offsetof(xen_cluster_record, expected_hosts) },
        { .key = "allowed_operations",
          .type = &xen_cluster_operation_set_abstract_type_,
          .offset = offsetof(xen_cluster_record, allowed_operations) },
        { .key = "current_operations",
          .type = &string_cluster_operation_map_abstract_type_,
          .offset = offsetof(xen_cluster_record, current_operations) },
        { .key = "pool_auto_join",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_cluster_record, pool_auto_join) },
        { .key = "token_timeout",
          .type = &abstract_type_float,
          .offset = offsetof(xen_cluster_record, token_timeout) },
        { .key = "token_timeout_coefficient",
          .type = &abstract_type_float,
          .offset = offsetof(xen_cluster_record, token_timeout_coefficient) },
        { .key = "cluster_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_cluster_record, cluster_config) },
        { .key = "other_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_cluster_record, other_config) }
    };


const abstract_type xen_cluster_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_cluster_record),
       .member_count =
           sizeof(xen_cluster_record_struct_members) / sizeof(struct_member),
       .members = xen_cluster_record_struct_members
    };


const abstract_type xen_cluster_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_cluster_record_abstract_type_
    };


static const struct struct_member xen_cluster_xen_cluster_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_cluster_xen_cluster_record_map_contents, key)
    },
    {
        .type = &xen_cluster_record_abstract_type_,
        .offset = offsetof(xen_cluster_xen_cluster_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_cluster_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_cluster_xen_cluster_record_map_contents),
    .members = xen_cluster_xen_cluster_record_members
};


void
xen_cluster_record_free(xen_cluster_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_cluster_host_record_opt_set_free(record->cluster_hosts);
    xen_string_set_free(record->pending_forget);
    free(record->cluster_token);
    free(record->cluster_stack);
    xen_cluster_operation_set_free(record->allowed_operations);
    xen_string_cluster_operation_map_free(record->current_operations);
    xen_string_string_map_free(record->cluster_config);
    xen_string_string_map_free(record->other_config);
    free(record);
}


bool
xen_cluster_get_record(xen_session *session, xen_cluster_record **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = xen_cluster_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("Cluster.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_cluster_get_by_uuid(xen_session *session, xen_cluster *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster.get_by_uuid");
    return session->ok;
}


bool
xen_cluster_get_uuid(xen_session *session, char **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster.get_uuid");
    return session->ok;
}


bool
xen_cluster_get_cluster_hosts(xen_session *session, struct xen_cluster_host_set **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Cluster.get_cluster_hosts");
    return session->ok;
}


bool
xen_cluster_get_pending_forget(xen_session *session, struct xen_string_set **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Cluster.get_pending_forget");
    return session->ok;
}


bool
xen_cluster_get_cluster_token(xen_session *session, char **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster.get_cluster_token");
    return session->ok;
}


bool
xen_cluster_get_cluster_stack(xen_session *session, char **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster.get_cluster_stack");
    return session->ok;
}


bool
xen_cluster_get_cluster_stack_version(xen_session *session, int64_t *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("Cluster.get_cluster_stack_version");
    return session->ok;
}


bool
xen_cluster_get_is_quorate(xen_session *session, bool *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Cluster.get_is_quorate");
    return session->ok;
}


bool
xen_cluster_get_quorum(xen_session *session, int64_t *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("Cluster.get_quorum");
    return session->ok;
}


bool
xen_cluster_get_live_hosts(xen_session *session, int64_t *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("Cluster.get_live_hosts");
    return session->ok;
}


bool
xen_cluster_get_expected_hosts(xen_session *session, int64_t *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("Cluster.get_expected_hosts");
    return session->ok;
}


bool
xen_cluster_get_allowed_operations(xen_session *session, struct xen_cluster_operation_set **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = xen_cluster_operation_set_abstract_type_;

    *result = NULL;
    XEN_CALL_("Cluster.get_allowed_operations");
    return session->ok;
}


bool
xen_cluster_get_current_operations(xen_session *session, xen_string_cluster_operation_map **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = string_cluster_operation_map_abstract_type_;

    *result = NULL;
    XEN_CALL_("Cluster.get_current_operations");
    return session->ok;
}


bool
xen_cluster_get_pool_auto_join(xen_session *session, bool *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Cluster.get_pool_auto_join");
    return session->ok;
}


bool
xen_cluster_get_token_timeout(xen_session *session, double *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_float;

    XEN_CALL_("Cluster.get_token_timeout");
    return session->ok;
}


bool
xen_cluster_get_token_timeout_coefficient(xen_session *session, double *result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_float;

    XEN_CALL_("Cluster.get_token_timeout_coefficient");
    return session->ok;
}


bool
xen_cluster_get_cluster_config(xen_session *session, xen_string_string_map **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("Cluster.get_cluster_config");
    return session->ok;
}


bool
xen_cluster_get_other_config(xen_session *session, xen_string_string_map **result, xen_cluster cluster)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("Cluster.get_other_config");
    return session->ok;
}


bool
xen_cluster_set_other_config(xen_session *session, xen_cluster cluster, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    xen_call_(session, "Cluster.set_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_add_to_other_config(xen_session *session, xen_cluster cluster, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "Cluster.add_to_other_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_remove_from_other_config(xen_session *session, xen_cluster cluster, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = cluster },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "Cluster.remove_from_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_create(xen_session *session, xen_cluster *result, xen_pif pif, char *cluster_stack, bool pool_auto_join, double token_timeout, double token_timeout_coefficient)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pif },
            { .type = &abstract_type_string,
              .u.string_val = cluster_stack },
            { .type = &abstract_type_bool,
              .u.bool_val = pool_auto_join },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout_coefficient }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster.create");
    return session->ok;
}


bool
xen_cluster_create_async(xen_session *session, xen_task *result, xen_pif pif, char *cluster_stack, bool pool_auto_join, double token_timeout, double token_timeout_coefficient)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pif },
            { .type = &abstract_type_string,
              .u.string_val = cluster_stack },
            { .type = &abstract_type_bool,
              .u.bool_val = pool_auto_join },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout_coefficient }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster.create");
    return session->ok;
}


bool
xen_cluster_destroy(xen_session *session, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_destroy_async(xen_session *session, xen_task *result, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster.destroy");
    return session->ok;
}


bool
xen_cluster_get_network(xen_session *session, xen_network *result, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster.get_network");
    return session->ok;
}


bool
xen_cluster_get_network_async(xen_session *session, xen_task *result, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster.get_network");
    return session->ok;
}


bool
xen_cluster_pool_create(xen_session *session, xen_cluster *result, xen_network network, char *cluster_stack, double token_timeout, double token_timeout_coefficient)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network },
            { .type = &abstract_type_string,
              .u.string_val = cluster_stack },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout_coefficient }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Cluster.pool_create");
    return session->ok;
}


bool
xen_cluster_pool_create_async(xen_session *session, xen_task *result, xen_network network, char *cluster_stack, double token_timeout, double token_timeout_coefficient)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network },
            { .type = &abstract_type_string,
              .u.string_val = cluster_stack },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout },
            { .type = &abstract_type_float,
              .u.float_val = token_timeout_coefficient }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster.pool_create");
    return session->ok;
}


bool
xen_cluster_pool_force_destroy(xen_session *session, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster.pool_force_destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_pool_force_destroy_async(xen_session *session, xen_task *result, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster.pool_force_destroy");
    return session->ok;
}


bool
xen_cluster_pool_destroy(xen_session *session, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster.pool_destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_pool_destroy_async(xen_session *session, xen_task *result, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster.pool_destroy");
    return session->ok;
}


bool
xen_cluster_pool_resync(xen_session *session, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Cluster.pool_resync", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_cluster_pool_resync_async(xen_session *session, xen_task *result, xen_cluster self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Cluster.pool_resync");
    return session->ok;
}


bool
xen_cluster_get_all(xen_session *session, struct xen_cluster_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "Cluster.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_cluster_get_all_records(xen_session *session, xen_cluster_xen_cluster_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_cluster_record_map;

    *result = NULL;
    xen_call_(session, "Cluster.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
