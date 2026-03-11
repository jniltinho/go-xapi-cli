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
#include "xen_origin_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_repository.h>


XEN_FREE(xen_repository)
XEN_SET_ALLOC_FREE(xen_repository)
XEN_ALLOC(xen_repository_record)
XEN_SET_ALLOC_FREE(xen_repository_record)
XEN_ALLOC(xen_repository_record_opt)
XEN_RECORD_OPT_FREE(xen_repository)
XEN_SET_ALLOC_FREE(xen_repository_record_opt)


static const struct_member xen_repository_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, name_description) },
        { .key = "binary_url",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, binary_url) },
        { .key = "source_url",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, source_url) },
        { .key = "update",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_repository_record, update) },
        { .key = "hash",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, hash) },
        { .key = "up_to_date",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_repository_record, up_to_date) },
        { .key = "gpgkey_path",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, gpgkey_path) },
        { .key = "origin",
          .type = &xen_origin_abstract_type_,
          .offset = offsetof(xen_repository_record, origin) },
        { .key = "certificate",
          .type = &abstract_type_string,
          .offset = offsetof(xen_repository_record, certificate) }
    };


const abstract_type xen_repository_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_repository_record),
       .member_count =
           sizeof(xen_repository_record_struct_members) / sizeof(struct_member),
       .members = xen_repository_record_struct_members
    };


const abstract_type xen_repository_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_repository_record_abstract_type_
    };


static const struct struct_member xen_repository_xen_repository_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_repository_xen_repository_record_map_contents, key)
    },
    {
        .type = &xen_repository_record_abstract_type_,
        .offset = offsetof(xen_repository_xen_repository_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_repository_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_repository_xen_repository_record_map_contents),
    .members = xen_repository_xen_repository_record_members
};


void
xen_repository_record_free(xen_repository_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    free(record->binary_url);
    free(record->source_url);
    free(record->hash);
    free(record->gpgkey_path);
    free(record->certificate);
    free(record);
}


bool
xen_repository_get_record(xen_session *session, xen_repository_record **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = xen_repository_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("Repository.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_repository_get_by_uuid(xen_session *session, xen_repository *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_by_uuid");
    return session->ok;
}


bool
xen_repository_get_by_name_label(xen_session *session, struct xen_repository_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Repository.get_by_name_label");
    return session->ok;
}


bool
xen_repository_get_uuid(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_uuid");
    return session->ok;
}


bool
xen_repository_get_name_label(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_name_label");
    return session->ok;
}


bool
xen_repository_get_name_description(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_name_description");
    return session->ok;
}


bool
xen_repository_get_binary_url(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_binary_url");
    return session->ok;
}


bool
xen_repository_get_source_url(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_source_url");
    return session->ok;
}


bool
xen_repository_get_update(xen_session *session, bool *result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Repository.get_update");
    return session->ok;
}


bool
xen_repository_get_hash(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_hash");
    return session->ok;
}


bool
xen_repository_get_up_to_date(xen_session *session, bool *result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Repository.get_up_to_date");
    return session->ok;
}


bool
xen_repository_get_gpgkey_path(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_gpgkey_path");
    return session->ok;
}


bool
xen_repository_get_origin(xen_session *session, enum xen_origin *result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = xen_origin_abstract_type_;

    XEN_CALL_("Repository.get_origin");
    return session->ok;
}


bool
xen_repository_get_certificate(xen_session *session, char **result, xen_repository repository)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.get_certificate");
    return session->ok;
}


bool
xen_repository_set_name_label(xen_session *session, xen_repository repository, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository },
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    xen_call_(session, "Repository.set_name_label", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_repository_set_name_description(xen_session *session, xen_repository repository, char *description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = repository },
            { .type = &abstract_type_string,
              .u.string_val = description }
        };

    xen_call_(session, "Repository.set_name_description", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_repository_introduce(xen_session *session, xen_repository *result, char *name_label, char *name_description, char *binary_url, char *source_url, bool update, char *gpgkey_path)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string,
              .u.string_val = binary_url },
            { .type = &abstract_type_string,
              .u.string_val = source_url },
            { .type = &abstract_type_bool,
              .u.bool_val = update },
            { .type = &abstract_type_string,
              .u.string_val = gpgkey_path }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.introduce");
    return session->ok;
}


bool
xen_repository_introduce_async(xen_session *session, xen_task *result, char *name_label, char *name_description, char *binary_url, char *source_url, bool update, char *gpgkey_path)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string,
              .u.string_val = binary_url },
            { .type = &abstract_type_string,
              .u.string_val = source_url },
            { .type = &abstract_type_bool,
              .u.bool_val = update },
            { .type = &abstract_type_string,
              .u.string_val = gpgkey_path }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Repository.introduce");
    return session->ok;
}


bool
xen_repository_introduce_bundle(xen_session *session, xen_repository *result, char *name_label, char *name_description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.introduce_bundle");
    return session->ok;
}


bool
xen_repository_introduce_bundle_async(xen_session *session, xen_task *result, char *name_label, char *name_description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Repository.introduce_bundle");
    return session->ok;
}


bool
xen_repository_introduce_remote_pool(xen_session *session, xen_repository *result, char *name_label, char *name_description, char *binary_url, char *certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string,
              .u.string_val = binary_url },
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Repository.introduce_remote_pool");
    return session->ok;
}


bool
xen_repository_introduce_remote_pool_async(xen_session *session, xen_task *result, char *name_label, char *name_description, char *binary_url, char *certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string,
              .u.string_val = binary_url },
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Repository.introduce_remote_pool");
    return session->ok;
}


bool
xen_repository_forget(xen_session *session, xen_repository self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "Repository.forget", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_repository_forget_async(xen_session *session, xen_task *result, xen_repository self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Repository.forget");
    return session->ok;
}


bool
xen_repository_set_gpgkey_path(xen_session *session, xen_repository self, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "Repository.set_gpgkey_path", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_repository_set_gpgkey_path_async(xen_session *session, xen_task *result, xen_repository self, char *value)
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
    XEN_CALL_("Async.Repository.set_gpgkey_path");
    return session->ok;
}


bool
xen_repository_get_all(xen_session *session, struct xen_repository_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "Repository.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_repository_get_all_records(xen_session *session, xen_repository_xen_repository_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_repository_record_map;

    *result = NULL;
    xen_call_(session, "Repository.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
