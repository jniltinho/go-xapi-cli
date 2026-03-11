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
#include <xen/api/xen_pusb.h>


XEN_FREE(xen_pusb)
XEN_SET_ALLOC_FREE(xen_pusb)
XEN_ALLOC(xen_pusb_record)
XEN_SET_ALLOC_FREE(xen_pusb_record)
XEN_ALLOC(xen_pusb_record_opt)
XEN_RECORD_OPT_FREE(xen_pusb)
XEN_SET_ALLOC_FREE(xen_pusb_record_opt)


static const struct_member xen_pusb_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, uuid) },
        { .key = "USB_group",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pusb_record, usb_group) },
        { .key = "host",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pusb_record, host) },
        { .key = "path",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, path) },
        { .key = "vendor_id",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, vendor_id) },
        { .key = "vendor_desc",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, vendor_desc) },
        { .key = "product_id",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, product_id) },
        { .key = "product_desc",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, product_desc) },
        { .key = "serial",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, serial) },
        { .key = "version",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, version) },
        { .key = "description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pusb_record, description) },
        { .key = "passthrough_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pusb_record, passthrough_enabled) },
        { .key = "other_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pusb_record, other_config) },
        { .key = "speed",
          .type = &abstract_type_float,
          .offset = offsetof(xen_pusb_record, speed) }
    };


const abstract_type xen_pusb_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_pusb_record),
       .member_count =
           sizeof(xen_pusb_record_struct_members) / sizeof(struct_member),
       .members = xen_pusb_record_struct_members
    };


const abstract_type xen_pusb_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_pusb_record_abstract_type_
    };


static const struct struct_member xen_pusb_xen_pusb_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_pusb_xen_pusb_record_map_contents, key)
    },
    {
        .type = &xen_pusb_record_abstract_type_,
        .offset = offsetof(xen_pusb_xen_pusb_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_pusb_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_pusb_xen_pusb_record_map_contents),
    .members = xen_pusb_xen_pusb_record_members
};


void
xen_pusb_record_free(xen_pusb_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_usb_group_record_opt_free(record->usb_group);
    xen_host_record_opt_free(record->host);
    free(record->path);
    free(record->vendor_id);
    free(record->vendor_desc);
    free(record->product_id);
    free(record->product_desc);
    free(record->serial);
    free(record->version);
    free(record->description);
    xen_string_string_map_free(record->other_config);
    free(record);
}


bool
xen_pusb_get_record(xen_session *session, xen_pusb_record **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = xen_pusb_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("PUSB.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_pusb_get_by_uuid(xen_session *session, xen_pusb *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_by_uuid");
    return session->ok;
}


bool
xen_pusb_get_uuid(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_uuid");
    return session->ok;
}


bool
xen_pusb_get_usb_group(xen_session *session, xen_usb_group *result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_USB_group");
    return session->ok;
}


bool
xen_pusb_get_host(xen_session *session, xen_host *result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_host");
    return session->ok;
}


bool
xen_pusb_get_path(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_path");
    return session->ok;
}


bool
xen_pusb_get_vendor_id(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_vendor_id");
    return session->ok;
}


bool
xen_pusb_get_vendor_desc(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_vendor_desc");
    return session->ok;
}


bool
xen_pusb_get_product_id(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_product_id");
    return session->ok;
}


bool
xen_pusb_get_product_desc(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_product_desc");
    return session->ok;
}


bool
xen_pusb_get_serial(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_serial");
    return session->ok;
}


bool
xen_pusb_get_version(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_version");
    return session->ok;
}


bool
xen_pusb_get_description(xen_session *session, char **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("PUSB.get_description");
    return session->ok;
}


bool
xen_pusb_get_passthrough_enabled(xen_session *session, bool *result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("PUSB.get_passthrough_enabled");
    return session->ok;
}


bool
xen_pusb_get_other_config(xen_session *session, xen_string_string_map **result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("PUSB.get_other_config");
    return session->ok;
}


bool
xen_pusb_get_speed(xen_session *session, double *result, xen_pusb pusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb }
        };

    abstract_type result_type = abstract_type_float;

    XEN_CALL_("PUSB.get_speed");
    return session->ok;
}


bool
xen_pusb_set_other_config(xen_session *session, xen_pusb pusb, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    xen_call_(session, "PUSB.set_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pusb_add_to_other_config(xen_session *session, xen_pusb pusb, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "PUSB.add_to_other_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pusb_remove_from_other_config(xen_session *session, xen_pusb pusb, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pusb },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "PUSB.remove_from_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pusb_scan(xen_session *session, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    xen_call_(session, "PUSB.scan", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pusb_scan_async(xen_session *session, xen_task *result, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PUSB.scan");
    return session->ok;
}


bool
xen_pusb_set_passthrough_enabled(xen_session *session, xen_pusb self, bool value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = value }
        };

    xen_call_(session, "PUSB.set_passthrough_enabled", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pusb_set_passthrough_enabled_async(xen_session *session, xen_task *result, xen_pusb self, bool value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.PUSB.set_passthrough_enabled");
    return session->ok;
}


bool
xen_pusb_get_all(xen_session *session, struct xen_pusb_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "PUSB.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pusb_get_all_records(xen_session *session, xen_pusb_xen_pusb_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_pusb_record_map;

    *result = NULL;
    xen_call_(session, "PUSB.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
