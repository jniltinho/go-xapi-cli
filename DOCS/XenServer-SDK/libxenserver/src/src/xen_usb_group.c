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
#include <xen/api/xen_usb_group.h>


XEN_FREE(xen_usb_group)
XEN_SET_ALLOC_FREE(xen_usb_group)
XEN_ALLOC(xen_usb_group_record)
XEN_SET_ALLOC_FREE(xen_usb_group_record)
XEN_ALLOC(xen_usb_group_record_opt)
XEN_RECORD_OPT_FREE(xen_usb_group)
XEN_SET_ALLOC_FREE(xen_usb_group_record_opt)


static const struct_member xen_usb_group_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_usb_group_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_usb_group_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_usb_group_record, name_description) },
        { .key = "PUSBs",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_usb_group_record, pusbs) },
        { .key = "VUSBs",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_usb_group_record, vusbs) },
        { .key = "other_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_usb_group_record, other_config) }
    };


const abstract_type xen_usb_group_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_usb_group_record),
       .member_count =
           sizeof(xen_usb_group_record_struct_members) / sizeof(struct_member),
       .members = xen_usb_group_record_struct_members
    };


const abstract_type xen_usb_group_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_usb_group_record_abstract_type_
    };


static const struct struct_member xen_usb_group_xen_usb_group_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_usb_group_xen_usb_group_record_map_contents, key)
    },
    {
        .type = &xen_usb_group_record_abstract_type_,
        .offset = offsetof(xen_usb_group_xen_usb_group_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_usb_group_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_usb_group_xen_usb_group_record_map_contents),
    .members = xen_usb_group_xen_usb_group_record_members
};


void
xen_usb_group_record_free(xen_usb_group_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    xen_pusb_record_opt_set_free(record->pusbs);
    xen_vusb_record_opt_set_free(record->vusbs);
    xen_string_string_map_free(record->other_config);
    free(record);
}


bool
xen_usb_group_get_record(xen_session *session, xen_usb_group_record **result, xen_usb_group usb_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group }
        };

    abstract_type result_type = xen_usb_group_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("USB_group.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_usb_group_get_by_uuid(xen_session *session, xen_usb_group *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("USB_group.get_by_uuid");
    return session->ok;
}


bool
xen_usb_group_get_by_name_label(xen_session *session, struct xen_usb_group_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("USB_group.get_by_name_label");
    return session->ok;
}


bool
xen_usb_group_get_uuid(xen_session *session, char **result, xen_usb_group usb_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("USB_group.get_uuid");
    return session->ok;
}


bool
xen_usb_group_get_name_label(xen_session *session, char **result, xen_usb_group usb_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("USB_group.get_name_label");
    return session->ok;
}


bool
xen_usb_group_get_name_description(xen_session *session, char **result, xen_usb_group usb_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("USB_group.get_name_description");
    return session->ok;
}


bool
xen_usb_group_get_pusbs(xen_session *session, struct xen_pusb_set **result, xen_usb_group usb_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("USB_group.get_PUSBs");
    return session->ok;
}


bool
xen_usb_group_get_vusbs(xen_session *session, struct xen_vusb_set **result, xen_usb_group usb_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("USB_group.get_VUSBs");
    return session->ok;
}


bool
xen_usb_group_get_other_config(xen_session *session, xen_string_string_map **result, xen_usb_group usb_group)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("USB_group.get_other_config");
    return session->ok;
}


bool
xen_usb_group_set_name_label(xen_session *session, xen_usb_group usb_group, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group },
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    xen_call_(session, "USB_group.set_name_label", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_usb_group_set_name_description(xen_session *session, xen_usb_group usb_group, char *description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group },
            { .type = &abstract_type_string,
              .u.string_val = description }
        };

    xen_call_(session, "USB_group.set_name_description", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_usb_group_set_other_config(xen_session *session, xen_usb_group usb_group, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    xen_call_(session, "USB_group.set_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_usb_group_add_to_other_config(xen_session *session, xen_usb_group usb_group, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "USB_group.add_to_other_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_usb_group_remove_from_other_config(xen_session *session, xen_usb_group usb_group, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = usb_group },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "USB_group.remove_from_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_usb_group_create(xen_session *session, xen_usb_group *result, char *name_label, char *name_description, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("USB_group.create");
    return session->ok;
}


bool
xen_usb_group_create_async(xen_session *session, xen_task *result, char *name_label, char *name_description, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name_label },
            { .type = &abstract_type_string,
              .u.string_val = name_description },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.USB_group.create");
    return session->ok;
}


bool
xen_usb_group_destroy(xen_session *session, xen_usb_group self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "USB_group.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_usb_group_destroy_async(xen_session *session, xen_task *result, xen_usb_group self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.USB_group.destroy");
    return session->ok;
}


bool
xen_usb_group_get_all(xen_session *session, struct xen_usb_group_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "USB_group.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_usb_group_get_all_records(xen_session *session, xen_usb_group_xen_usb_group_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_usb_group_record_map;

    *result = NULL;
    xen_call_(session, "USB_group.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
