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
#include "xen_livepatch_status_internal.h"
#include "xen_update_after_apply_guidance_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_pool_update.h>


XEN_FREE(xen_pool_update)
XEN_SET_ALLOC_FREE(xen_pool_update)
XEN_ALLOC(xen_pool_update_record)
XEN_SET_ALLOC_FREE(xen_pool_update_record)
XEN_ALLOC(xen_pool_update_record_opt)
XEN_RECORD_OPT_FREE(xen_pool_update)
XEN_SET_ALLOC_FREE(xen_pool_update_record_opt)


static const struct_member xen_pool_update_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_update_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_update_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_update_record, name_description) },
        { .key = "version",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_update_record, version) },
        { .key = "installation_size",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_update_record, installation_size) },
        { .key = "key",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_update_record, key) },
        { .key = "after_apply_guidance",
          .type = &xen_update_after_apply_guidance_set_abstract_type_,
          .offset = offsetof(xen_pool_update_record, after_apply_guidance) },
        { .key = "vdi",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_update_record, vdi) },
        { .key = "hosts",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_pool_update_record, hosts) },
        { .key = "other_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_update_record, other_config) },
        { .key = "enforce_homogeneity",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_update_record, enforce_homogeneity) }
    };


const abstract_type xen_pool_update_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_pool_update_record),
       .member_count =
           sizeof(xen_pool_update_record_struct_members) / sizeof(struct_member),
       .members = xen_pool_update_record_struct_members
    };


const abstract_type xen_pool_update_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_pool_update_record_abstract_type_
    };


static const struct struct_member xen_pool_update_xen_pool_update_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_pool_update_xen_pool_update_record_map_contents, key)
    },
    {
        .type = &xen_pool_update_record_abstract_type_,
        .offset = offsetof(xen_pool_update_xen_pool_update_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_pool_update_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_pool_update_xen_pool_update_record_map_contents),
    .members = xen_pool_update_xen_pool_update_record_members
};


void
xen_pool_update_record_free(xen_pool_update_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    free(record->version);
    free(record->key);
    xen_update_after_apply_guidance_set_free(record->after_apply_guidance);
    xen_vdi_record_opt_free(record->vdi);
    xen_host_record_opt_set_free(record->hosts);
    xen_string_string_map_free(record->other_config);
    free(record);
}


bool
xen_pool_update_get_record(xen_session *session, xen_pool_update_record **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = xen_pool_update_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("pool_update.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_pool_update_get_by_uuid(xen_session *session, xen_pool_update *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.get_by_uuid");
    return session->ok;
}


bool
xen_pool_update_get_by_name_label(xen_session *session, struct xen_pool_update_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool_update.get_by_name_label");
    return session->ok;
}


bool
xen_pool_update_get_uuid(xen_session *session, char **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.get_uuid");
    return session->ok;
}


bool
xen_pool_update_get_name_label(xen_session *session, char **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.get_name_label");
    return session->ok;
}


bool
xen_pool_update_get_name_description(xen_session *session, char **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.get_name_description");
    return session->ok;
}


bool
xen_pool_update_get_version(xen_session *session, char **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.get_version");
    return session->ok;
}


bool
xen_pool_update_get_installation_size(xen_session *session, int64_t *result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool_update.get_installation_size");
    return session->ok;
}


bool
xen_pool_update_get_key(xen_session *session, char **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.get_key");
    return session->ok;
}


bool
xen_pool_update_get_after_apply_guidance(xen_session *session, struct xen_update_after_apply_guidance_set **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = xen_update_after_apply_guidance_set_abstract_type_;

    *result = NULL;
    XEN_CALL_("pool_update.get_after_apply_guidance");
    return session->ok;
}


bool
xen_pool_update_get_vdi(xen_session *session, xen_vdi *result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.get_vdi");
    return session->ok;
}


bool
xen_pool_update_get_hosts(xen_session *session, struct xen_host_set **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool_update.get_hosts");
    return session->ok;
}


bool
xen_pool_update_get_other_config(xen_session *session, xen_string_string_map **result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool_update.get_other_config");
    return session->ok;
}


bool
xen_pool_update_get_enforce_homogeneity(xen_session *session, bool *result, xen_pool_update pool_update)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool_update.get_enforce_homogeneity");
    return session->ok;
}


bool
xen_pool_update_set_other_config(xen_session *session, xen_pool_update pool_update, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    xen_call_(session, "pool_update.set_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_update_add_to_other_config(xen_session *session, xen_pool_update pool_update, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool_update.add_to_other_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_update_remove_from_other_config(xen_session *session, xen_pool_update pool_update, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool_update },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "pool_update.remove_from_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_update_introduce(xen_session *session, xen_pool_update *result, xen_vdi vdi)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vdi }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool_update.introduce");
    return session->ok;
}


bool
xen_pool_update_introduce_async(xen_session *session, xen_task *result, xen_vdi vdi)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vdi }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool_update.introduce");
    return session->ok;
}


bool
xen_pool_update_precheck(xen_session *session, enum xen_livepatch_status *result, xen_pool_update self, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    abstract_type result_type = xen_livepatch_status_abstract_type_;

    XEN_CALL_("pool_update.precheck");
    return session->ok;
}


bool
xen_pool_update_precheck_async(xen_session *session, xen_task *result, xen_pool_update self, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool_update.precheck");
    return session->ok;
}


bool
xen_pool_update_apply(xen_session *session, xen_pool_update self, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    xen_call_(session, "pool_update.apply", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_update_apply_async(xen_session *session, xen_task *result, xen_pool_update self, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool_update.apply");
    return session->ok;
}


bool
xen_pool_update_pool_apply(xen_session *session, xen_pool_update self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool_update.pool_apply", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_update_pool_apply_async(xen_session *session, xen_task *result, xen_pool_update self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool_update.pool_apply");
    return session->ok;
}


bool
xen_pool_update_pool_clean(xen_session *session, xen_pool_update self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool_update.pool_clean", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_update_pool_clean_async(xen_session *session, xen_task *result, xen_pool_update self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool_update.pool_clean");
    return session->ok;
}


bool
xen_pool_update_destroy(xen_session *session, xen_pool_update self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool_update.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_update_destroy_async(xen_session *session, xen_task *result, xen_pool_update self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool_update.destroy");
    return session->ok;
}


bool
xen_pool_update_get_all(xen_session *session, struct xen_pool_update_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "pool_update.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_update_get_all_records(xen_session *session, xen_pool_update_xen_pool_update_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_pool_update_record_map;

    *result = NULL;
    xen_call_(session, "pool_update.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
