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

#include "xen_certificate_type_internal.h"
#include "xen_internal.h"
#include <xen/api/xen_certificate.h>
#include <xen/api/xen_common.h>


XEN_FREE(xen_certificate)
XEN_SET_ALLOC_FREE(xen_certificate)
XEN_ALLOC(xen_certificate_record)
XEN_SET_ALLOC_FREE(xen_certificate_record)
XEN_ALLOC(xen_certificate_record_opt)
XEN_RECORD_OPT_FREE(xen_certificate)
XEN_SET_ALLOC_FREE(xen_certificate_record_opt)


static const struct_member xen_certificate_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_certificate_record, uuid) },
        { .key = "name",
          .type = &abstract_type_string,
          .offset = offsetof(xen_certificate_record, name) },
        { .key = "type",
          .type = &xen_certificate_type_abstract_type_,
          .offset = offsetof(xen_certificate_record, type) },
        { .key = "host",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_certificate_record, host) },
        { .key = "not_before",
          .type = &abstract_type_datetime,
          .offset = offsetof(xen_certificate_record, not_before) },
        { .key = "not_after",
          .type = &abstract_type_datetime,
          .offset = offsetof(xen_certificate_record, not_after) },
        { .key = "fingerprint",
          .type = &abstract_type_string,
          .offset = offsetof(xen_certificate_record, fingerprint) },
        { .key = "fingerprint_sha256",
          .type = &abstract_type_string,
          .offset = offsetof(xen_certificate_record, fingerprint_sha256) },
        { .key = "fingerprint_sha1",
          .type = &abstract_type_string,
          .offset = offsetof(xen_certificate_record, fingerprint_sha1) }
    };


const abstract_type xen_certificate_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_certificate_record),
       .member_count =
           sizeof(xen_certificate_record_struct_members) / sizeof(struct_member),
       .members = xen_certificate_record_struct_members
    };


const abstract_type xen_certificate_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_certificate_record_abstract_type_
    };


static const struct struct_member xen_certificate_xen_certificate_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_certificate_xen_certificate_record_map_contents, key)
    },
    {
        .type = &xen_certificate_record_abstract_type_,
        .offset = offsetof(xen_certificate_xen_certificate_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_certificate_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_certificate_xen_certificate_record_map_contents),
    .members = xen_certificate_xen_certificate_record_members
};


void
xen_certificate_record_free(xen_certificate_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name);
    xen_host_record_opt_free(record->host);
    free(record->fingerprint);
    free(record->fingerprint_sha256);
    free(record->fingerprint_sha1);
    free(record);
}


bool
xen_certificate_get_record(xen_session *session, xen_certificate_record **result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = xen_certificate_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("Certificate.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_certificate_get_by_uuid(xen_session *session, xen_certificate *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Certificate.get_by_uuid");
    return session->ok;
}


bool
xen_certificate_get_uuid(xen_session *session, char **result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Certificate.get_uuid");
    return session->ok;
}


bool
xen_certificate_get_name(xen_session *session, char **result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Certificate.get_name");
    return session->ok;
}


bool
xen_certificate_get_type(xen_session *session, enum xen_certificate_type *result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = xen_certificate_type_abstract_type_;

    XEN_CALL_("Certificate.get_type");
    return session->ok;
}


bool
xen_certificate_get_host(xen_session *session, xen_host *result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Certificate.get_host");
    return session->ok;
}


bool
xen_certificate_get_not_before(xen_session *session, time_t *result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_datetime;

    XEN_CALL_("Certificate.get_not_before");
    return session->ok;
}


bool
xen_certificate_get_not_after(xen_session *session, time_t *result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_datetime;

    XEN_CALL_("Certificate.get_not_after");
    return session->ok;
}


bool
xen_certificate_get_fingerprint(xen_session *session, char **result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Certificate.get_fingerprint");
    return session->ok;
}


bool
xen_certificate_get_fingerprint_sha256(xen_session *session, char **result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Certificate.get_fingerprint_sha256");
    return session->ok;
}


bool
xen_certificate_get_fingerprint_sha1(xen_session *session, char **result, xen_certificate certificate)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = certificate }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Certificate.get_fingerprint_sha1");
    return session->ok;
}


bool
xen_certificate_get_all(xen_session *session, struct xen_certificate_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "Certificate.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_certificate_get_all_records(xen_session *session, xen_certificate_xen_certificate_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_certificate_record_map;

    *result = NULL;
    xen_call_(session, "Certificate.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
