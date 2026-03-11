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
#include "xen_sdn_controller_protocol_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_sdn_controller.h>


XEN_FREE(xen_sdn_controller)
XEN_SET_ALLOC_FREE(xen_sdn_controller)
XEN_ALLOC(xen_sdn_controller_record)
XEN_SET_ALLOC_FREE(xen_sdn_controller_record)
XEN_ALLOC(xen_sdn_controller_record_opt)
XEN_RECORD_OPT_FREE(xen_sdn_controller)
XEN_SET_ALLOC_FREE(xen_sdn_controller_record_opt)


static const struct_member xen_sdn_controller_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_sdn_controller_record, uuid) },
        { .key = "protocol",
          .type = &xen_sdn_controller_protocol_abstract_type_,
          .offset = offsetof(xen_sdn_controller_record, protocol) },
        { .key = "address",
          .type = &abstract_type_string,
          .offset = offsetof(xen_sdn_controller_record, address) },
        { .key = "port",
          .type = &abstract_type_int,
          .offset = offsetof(xen_sdn_controller_record, port) }
    };


const abstract_type xen_sdn_controller_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_sdn_controller_record),
       .member_count =
           sizeof(xen_sdn_controller_record_struct_members) / sizeof(struct_member),
       .members = xen_sdn_controller_record_struct_members
    };


const abstract_type xen_sdn_controller_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_sdn_controller_record_abstract_type_
    };


static const struct struct_member xen_sdn_controller_xen_sdn_controller_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_sdn_controller_xen_sdn_controller_record_map_contents, key)
    },
    {
        .type = &xen_sdn_controller_record_abstract_type_,
        .offset = offsetof(xen_sdn_controller_xen_sdn_controller_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_sdn_controller_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_sdn_controller_xen_sdn_controller_record_map_contents),
    .members = xen_sdn_controller_xen_sdn_controller_record_members
};


void
xen_sdn_controller_record_free(xen_sdn_controller_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->address);
    free(record);
}


bool
xen_sdn_controller_get_record(xen_session *session, xen_sdn_controller_record **result, xen_sdn_controller sdn_controller)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = sdn_controller }
        };

    abstract_type result_type = xen_sdn_controller_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("SDN_controller.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_sdn_controller_get_by_uuid(xen_session *session, xen_sdn_controller *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("SDN_controller.get_by_uuid");
    return session->ok;
}


bool
xen_sdn_controller_get_uuid(xen_session *session, char **result, xen_sdn_controller sdn_controller)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = sdn_controller }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("SDN_controller.get_uuid");
    return session->ok;
}


bool
xen_sdn_controller_get_protocol(xen_session *session, enum xen_sdn_controller_protocol *result, xen_sdn_controller sdn_controller)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = sdn_controller }
        };

    abstract_type result_type = xen_sdn_controller_protocol_abstract_type_;

    XEN_CALL_("SDN_controller.get_protocol");
    return session->ok;
}


bool
xen_sdn_controller_get_address(xen_session *session, char **result, xen_sdn_controller sdn_controller)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = sdn_controller }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("SDN_controller.get_address");
    return session->ok;
}


bool
xen_sdn_controller_get_port(xen_session *session, int64_t *result, xen_sdn_controller sdn_controller)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = sdn_controller }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("SDN_controller.get_port");
    return session->ok;
}


bool
xen_sdn_controller_introduce(xen_session *session, xen_sdn_controller *result, enum xen_sdn_controller_protocol protocol, char *address, int64_t port)
{
    abstract_value param_values[] =
        {
            { .type = &xen_sdn_controller_protocol_abstract_type_,
              .u.enum_val = protocol },
            { .type = &abstract_type_string,
              .u.string_val = address },
            { .type = &abstract_type_int,
              .u.int_val = port }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("SDN_controller.introduce");
    return session->ok;
}


bool
xen_sdn_controller_introduce_async(xen_session *session, xen_task *result, enum xen_sdn_controller_protocol protocol, char *address, int64_t port)
{
    abstract_value param_values[] =
        {
            { .type = &xen_sdn_controller_protocol_abstract_type_,
              .u.enum_val = protocol },
            { .type = &abstract_type_string,
              .u.string_val = address },
            { .type = &abstract_type_int,
              .u.int_val = port }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.SDN_controller.introduce");
    return session->ok;
}


bool
xen_sdn_controller_forget(xen_session *session, xen_sdn_controller self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "SDN_controller.forget", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_sdn_controller_forget_async(xen_session *session, xen_task *result, xen_sdn_controller self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.SDN_controller.forget");
    return session->ok;
}


bool
xen_sdn_controller_get_all(xen_session *session, struct xen_sdn_controller_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "SDN_controller.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_sdn_controller_get_all_records(xen_session *session, xen_sdn_controller_xen_sdn_controller_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_sdn_controller_record_map;

    *result = NULL;
    xen_call_(session, "SDN_controller.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
