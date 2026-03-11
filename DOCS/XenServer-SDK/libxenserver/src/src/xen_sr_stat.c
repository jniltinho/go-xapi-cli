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
#include "xen_sr_health_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_sr_stat.h>


XEN_FREE(xen_sr_stat)
XEN_SET_ALLOC_FREE(xen_sr_stat)
XEN_ALLOC(xen_sr_stat_record)
XEN_SET_ALLOC_FREE(xen_sr_stat_record)
XEN_ALLOC(xen_sr_stat_record_opt)
XEN_RECORD_OPT_FREE(xen_sr_stat)
XEN_SET_ALLOC_FREE(xen_sr_stat_record_opt)


static const struct_member xen_sr_stat_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_sr_stat_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_sr_stat_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_sr_stat_record, name_description) },
        { .key = "free_space",
          .type = &abstract_type_int,
          .offset = offsetof(xen_sr_stat_record, free_space) },
        { .key = "total_space",
          .type = &abstract_type_int,
          .offset = offsetof(xen_sr_stat_record, total_space) },
        { .key = "clustered",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_sr_stat_record, clustered) },
        { .key = "health",
          .type = &xen_sr_health_abstract_type_,
          .offset = offsetof(xen_sr_stat_record, health) }
    };


const abstract_type xen_sr_stat_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_sr_stat_record),
       .member_count =
           sizeof(xen_sr_stat_record_struct_members) / sizeof(struct_member),
       .members = xen_sr_stat_record_struct_members
    };


const abstract_type xen_sr_stat_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_sr_stat_record_abstract_type_
    };


void
xen_sr_stat_record_free(xen_sr_stat_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    free(record);
}
