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
#include <xen/api/xen_lvhd.h>


XEN_FREE(xen_lvhd)
XEN_SET_ALLOC_FREE(xen_lvhd)
XEN_ALLOC(xen_lvhd_record)
XEN_SET_ALLOC_FREE(xen_lvhd_record)
XEN_ALLOC(xen_lvhd_record_opt)
XEN_RECORD_OPT_FREE(xen_lvhd)
XEN_SET_ALLOC_FREE(xen_lvhd_record_opt)


static const struct_member xen_lvhd_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_lvhd_record, uuid) }
    };


const abstract_type xen_lvhd_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_lvhd_record),
       .member_count =
           sizeof(xen_lvhd_record_struct_members) / sizeof(struct_member),
       .members = xen_lvhd_record_struct_members
    };


const abstract_type xen_lvhd_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_lvhd_record_abstract_type_
    };


void
xen_lvhd_record_free(xen_lvhd_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record);
}


bool
xen_lvhd_get_record(xen_session *session, xen_lvhd_record **result, xen_lvhd lvhd)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = lvhd }
        };

    abstract_type result_type = xen_lvhd_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("LVHD.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_lvhd_get_by_uuid(xen_session *session, xen_lvhd *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("LVHD.get_by_uuid");
    return session->ok;
}


bool
xen_lvhd_get_uuid(xen_session *session, char **result, xen_lvhd lvhd)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = lvhd }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("LVHD.get_uuid");
    return session->ok;
}


bool
xen_lvhd_enable_thin_provisioning(xen_session *session, char **result, xen_host host, xen_sr sr, int64_t initial_allocation, int64_t allocation_quantum)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host },
            { .type = &abstract_type_string,
              .u.string_val = sr },
            { .type = &abstract_type_int,
              .u.int_val = initial_allocation },
            { .type = &abstract_type_int,
              .u.int_val = allocation_quantum }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("LVHD.enable_thin_provisioning");
    return session->ok;
}


bool
xen_lvhd_enable_thin_provisioning_async(xen_session *session, xen_task *result, xen_host host, xen_sr sr, int64_t initial_allocation, int64_t allocation_quantum)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host },
            { .type = &abstract_type_string,
              .u.string_val = sr },
            { .type = &abstract_type_int,
              .u.int_val = initial_allocation },
            { .type = &abstract_type_int,
              .u.int_val = allocation_quantum }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.LVHD.enable_thin_provisioning");
    return session->ok;
}
