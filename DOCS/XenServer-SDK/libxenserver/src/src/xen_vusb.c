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
#include "xen_string_vusb_operations_map_internal.h"
#include "xen_vusb_operations_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_vusb.h>


XEN_FREE(xen_vusb)
XEN_SET_ALLOC_FREE(xen_vusb)
XEN_ALLOC(xen_vusb_record)
XEN_SET_ALLOC_FREE(xen_vusb_record)
XEN_ALLOC(xen_vusb_record_opt)
XEN_RECORD_OPT_FREE(xen_vusb)
XEN_SET_ALLOC_FREE(xen_vusb_record_opt)


static const struct_member xen_vusb_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_vusb_record, uuid) },
        { .key = "allowed_operations",
          .type = &xen_vusb_operations_set_abstract_type_,
          .offset = offsetof(xen_vusb_record, allowed_operations) },
        { .key = "current_operations",
          .type = &string_vusb_operations_map_abstract_type_,
          .offset = offsetof(xen_vusb_record, current_operations) },
        { .key = "VM",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_vusb_record, vm) },
        { .key = "USB_group",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_vusb_record, usb_group) },
        { .key = "other_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_vusb_record, other_config) },
        { .key = "currently_attached",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_vusb_record, currently_attached) }
    };


const abstract_type xen_vusb_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_vusb_record),
       .member_count =
           sizeof(xen_vusb_record_struct_members) / sizeof(struct_member),
       .members = xen_vusb_record_struct_members
    };


const abstract_type xen_vusb_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_vusb_record_abstract_type_
    };


static const struct struct_member xen_vusb_xen_vusb_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_vusb_xen_vusb_record_map_contents, key)
    },
    {
        .type = &xen_vusb_record_abstract_type_,
        .offset = offsetof(xen_vusb_xen_vusb_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_vusb_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_vusb_xen_vusb_record_map_contents),
    .members = xen_vusb_xen_vusb_record_members
};


void
xen_vusb_record_free(xen_vusb_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    xen_vusb_operations_set_free(record->allowed_operations);
    xen_string_vusb_operations_map_free(record->current_operations);
    xen_vm_record_opt_free(record->vm);
    xen_usb_group_record_opt_free(record->usb_group);
    xen_string_string_map_free(record->other_config);
    free(record);
}


bool
xen_vusb_get_record(xen_session *session, xen_vusb_record **result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = xen_vusb_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("VUSB.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_vusb_get_by_uuid(xen_session *session, xen_vusb *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VUSB.get_by_uuid");
    return session->ok;
}


bool
xen_vusb_get_uuid(xen_session *session, char **result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VUSB.get_uuid");
    return session->ok;
}


bool
xen_vusb_get_allowed_operations(xen_session *session, struct xen_vusb_operations_set **result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = xen_vusb_operations_set_abstract_type_;

    *result = NULL;
    XEN_CALL_("VUSB.get_allowed_operations");
    return session->ok;
}


bool
xen_vusb_get_current_operations(xen_session *session, xen_string_vusb_operations_map **result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = string_vusb_operations_map_abstract_type_;

    *result = NULL;
    XEN_CALL_("VUSB.get_current_operations");
    return session->ok;
}


bool
xen_vusb_get_vm(xen_session *session, xen_vm *result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VUSB.get_VM");
    return session->ok;
}


bool
xen_vusb_get_usb_group(xen_session *session, xen_usb_group *result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VUSB.get_USB_group");
    return session->ok;
}


bool
xen_vusb_get_other_config(xen_session *session, xen_string_string_map **result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("VUSB.get_other_config");
    return session->ok;
}


bool
xen_vusb_get_currently_attached(xen_session *session, bool *result, xen_vusb vusb)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("VUSB.get_currently_attached");
    return session->ok;
}


bool
xen_vusb_set_other_config(xen_session *session, xen_vusb vusb, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    xen_call_(session, "VUSB.set_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vusb_add_to_other_config(xen_session *session, xen_vusb vusb, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "VUSB.add_to_other_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_vusb_remove_from_other_config(xen_session *session, xen_vusb vusb, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vusb },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "VUSB.remove_from_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_vusb_create(xen_session *session, xen_vusb *result, xen_vm vm, xen_usb_group usb_group, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm },
            { .type = &abstract_type_string,
              .u.string_val = usb_group },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("VUSB.create");
    return session->ok;
}


bool
xen_vusb_create_async(xen_session *session, xen_task *result, xen_vm vm, xen_usb_group usb_group, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = vm },
            { .type = &abstract_type_string,
              .u.string_val = usb_group },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VUSB.create");
    return session->ok;
}


bool
xen_vusb_unplug(xen_session *session, xen_vusb self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "VUSB.unplug", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_vusb_unplug_async(xen_session *session, xen_task *result, xen_vusb self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VUSB.unplug");
    return session->ok;
}


bool
xen_vusb_destroy(xen_session *session, xen_vusb self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "VUSB.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_vusb_destroy_async(xen_session *session, xen_task *result, xen_vusb self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.VUSB.destroy");
    return session->ok;
}


bool
xen_vusb_get_all(xen_session *session, struct xen_vusb_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "VUSB.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_vusb_get_all_records(xen_session *session, xen_vusb_xen_vusb_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_vusb_record_map;

    *result = NULL;
    xen_call_(session, "VUSB.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
