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
#include <xen/api/xen_feature.h>


XEN_FREE(xen_feature)
XEN_SET_ALLOC_FREE(xen_feature)
XEN_ALLOC(xen_feature_record)
XEN_SET_ALLOC_FREE(xen_feature_record)
XEN_ALLOC(xen_feature_record_opt)
XEN_RECORD_OPT_FREE(xen_feature)
XEN_SET_ALLOC_FREE(xen_feature_record_opt)


static const struct_member xen_feature_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_feature_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_feature_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_feature_record, name_description) },
        { .key = "enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_feature_record, enabled) },
        { .key = "experimental",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_feature_record, experimental) },
        { .key = "version",
          .type = &abstract_type_string,
          .offset = offsetof(xen_feature_record, version) },
        { .key = "host",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_feature_record, host) }
    };


const abstract_type xen_feature_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_feature_record),
       .member_count =
           sizeof(xen_feature_record_struct_members) / sizeof(struct_member),
       .members = xen_feature_record_struct_members
    };


const abstract_type xen_feature_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_feature_record_abstract_type_
    };


static const struct struct_member xen_feature_xen_feature_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_feature_xen_feature_record_map_contents, key)
    },
    {
        .type = &xen_feature_record_abstract_type_,
        .offset = offsetof(xen_feature_xen_feature_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_feature_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_feature_xen_feature_record_map_contents),
    .members = xen_feature_xen_feature_record_members
};


void
xen_feature_record_free(xen_feature_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    free(record->version);
    xen_host_record_opt_free(record->host);
    free(record);
}


bool
xen_feature_get_record(xen_session *session, xen_feature_record **result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = xen_feature_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("Feature.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_feature_get_by_uuid(xen_session *session, xen_feature *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Feature.get_by_uuid");
    return session->ok;
}


bool
xen_feature_get_by_name_label(xen_session *session, struct xen_feature_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Feature.get_by_name_label");
    return session->ok;
}


bool
xen_feature_get_uuid(xen_session *session, char **result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Feature.get_uuid");
    return session->ok;
}


bool
xen_feature_get_name_label(xen_session *session, char **result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Feature.get_name_label");
    return session->ok;
}


bool
xen_feature_get_name_description(xen_session *session, char **result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Feature.get_name_description");
    return session->ok;
}


bool
xen_feature_get_enabled(xen_session *session, bool *result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Feature.get_enabled");
    return session->ok;
}


bool
xen_feature_get_experimental(xen_session *session, bool *result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Feature.get_experimental");
    return session->ok;
}


bool
xen_feature_get_version(xen_session *session, char **result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Feature.get_version");
    return session->ok;
}


bool
xen_feature_get_host(xen_session *session, xen_host *result, xen_feature feature)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = feature }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Feature.get_host");
    return session->ok;
}


bool
xen_feature_get_all(xen_session *session, struct xen_feature_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "Feature.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_feature_get_all_records(xen_session *session, xen_feature_xen_feature_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_feature_record_map;

    *result = NULL;
    xen_call_(session, "Feature.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
