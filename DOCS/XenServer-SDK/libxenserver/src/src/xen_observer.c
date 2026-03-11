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
#include <xen/api/xen_observer.h>


XEN_FREE(xen_observer)
XEN_SET_ALLOC_FREE(xen_observer)
XEN_ALLOC(xen_observer_record)
XEN_SET_ALLOC_FREE(xen_observer_record)
XEN_ALLOC(xen_observer_record_opt)
XEN_RECORD_OPT_FREE(xen_observer)
XEN_SET_ALLOC_FREE(xen_observer_record_opt)


static const struct_member xen_observer_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_observer_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_observer_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_observer_record, name_description) },
        { .key = "hosts",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_observer_record, hosts) },
        { .key = "attributes",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_observer_record, attributes) },
        { .key = "endpoints",
          .type = &abstract_type_string_set,
          .offset = offsetof(xen_observer_record, endpoints) },
        { .key = "components",
          .type = &abstract_type_string_set,
          .offset = offsetof(xen_observer_record, components) },
        { .key = "enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_observer_record, enabled) }
    };


const abstract_type xen_observer_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_observer_record),
       .member_count =
           sizeof(xen_observer_record_struct_members) / sizeof(struct_member),
       .members = xen_observer_record_struct_members
    };


const abstract_type xen_observer_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_observer_record_abstract_type_
    };


static const struct struct_member xen_observer_xen_observer_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_observer_xen_observer_record_map_contents, key)
    },
    {
        .type = &xen_observer_record_abstract_type_,
        .offset = offsetof(xen_observer_xen_observer_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_observer_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_observer_xen_observer_record_map_contents),
    .members = xen_observer_xen_observer_record_members
};


void
xen_observer_record_free(xen_observer_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    xen_host_record_opt_set_free(record->hosts);
    xen_string_string_map_free(record->attributes);
    xen_string_set_free(record->endpoints);
    xen_string_set_free(record->components);
    free(record);
}


bool
xen_observer_get_record(xen_session *session, xen_observer_record **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = xen_observer_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("Observer.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_observer_get_by_uuid(xen_session *session, xen_observer *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Observer.get_by_uuid");
    return session->ok;
}


bool
xen_observer_create(xen_session *session, xen_observer *result, xen_observer_record *record)
{
    abstract_value param_values[] =
        {
            { .type = &xen_observer_record_abstract_type_,
              .u.struct_val = record }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Observer.create");
    return session->ok;
}


bool
xen_observer_create_async(xen_session *session, xen_task *result, xen_observer_record *record)
{
    abstract_value param_values[] =
        {
            { .type = &xen_observer_record_abstract_type_,
              .u.struct_val = record }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Observer.create");
    return session->ok;
}


bool
xen_observer_destroy(xen_session *session, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    xen_call_(session, "Observer.destroy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_observer_destroy_async(xen_session *session, xen_task *result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Observer.destroy");
    return session->ok;
}


bool
xen_observer_get_by_name_label(xen_session *session, struct xen_observer_set **result, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Observer.get_by_name_label");
    return session->ok;
}


bool
xen_observer_get_uuid(xen_session *session, char **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Observer.get_uuid");
    return session->ok;
}


bool
xen_observer_get_name_label(xen_session *session, char **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Observer.get_name_label");
    return session->ok;
}


bool
xen_observer_get_name_description(xen_session *session, char **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Observer.get_name_description");
    return session->ok;
}


bool
xen_observer_get_hosts(xen_session *session, struct xen_host_set **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Observer.get_hosts");
    return session->ok;
}


bool
xen_observer_get_attributes(xen_session *session, xen_string_string_map **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("Observer.get_attributes");
    return session->ok;
}


bool
xen_observer_get_endpoints(xen_session *session, struct xen_string_set **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Observer.get_endpoints");
    return session->ok;
}


bool
xen_observer_get_components(xen_session *session, struct xen_string_set **result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("Observer.get_components");
    return session->ok;
}


bool
xen_observer_get_enabled(xen_session *session, bool *result, xen_observer observer)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("Observer.get_enabled");
    return session->ok;
}


bool
xen_observer_set_name_label(xen_session *session, xen_observer observer, char *label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer },
            { .type = &abstract_type_string,
              .u.string_val = label }
        };

    xen_call_(session, "Observer.set_name_label", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_observer_set_name_description(xen_session *session, xen_observer observer, char *description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = observer },
            { .type = &abstract_type_string,
              .u.string_val = description }
        };

    xen_call_(session, "Observer.set_name_description", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_observer_set_hosts(xen_session *session, xen_observer self, struct xen_host_set *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)value }
        };

    xen_call_(session, "Observer.set_hosts", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_observer_set_hosts_async(xen_session *session, xen_task *result, xen_observer self, struct xen_host_set *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Observer.set_hosts");
    return session->ok;
}


bool
xen_observer_set_enabled(xen_session *session, xen_observer self, bool value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = value }
        };

    xen_call_(session, "Observer.set_enabled", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_observer_set_enabled_async(xen_session *session, xen_task *result, xen_observer self, bool value)
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
    XEN_CALL_("Async.Observer.set_enabled");
    return session->ok;
}


bool
xen_observer_set_attributes(xen_session *session, xen_observer self, xen_string_string_map *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)value }
        };

    xen_call_(session, "Observer.set_attributes", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_observer_set_attributes_async(xen_session *session, xen_task *result, xen_observer self, xen_string_string_map *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Observer.set_attributes");
    return session->ok;
}


bool
xen_observer_set_endpoints(xen_session *session, xen_observer self, struct xen_string_set *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)value }
        };

    xen_call_(session, "Observer.set_endpoints", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_observer_set_endpoints_async(xen_session *session, xen_task *result, xen_observer self, struct xen_string_set *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Observer.set_endpoints");
    return session->ok;
}


bool
xen_observer_set_components(xen_session *session, xen_observer self, struct xen_string_set *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)value }
        };

    xen_call_(session, "Observer.set_components", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_observer_set_components_async(xen_session *session, xen_task *result, xen_observer self, struct xen_string_set *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.Observer.set_components");
    return session->ok;
}


bool
xen_observer_get_all(xen_session *session, struct xen_observer_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "Observer.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_observer_get_all_records(xen_session *session, xen_observer_xen_observer_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_observer_record_map;

    *result = NULL;
    xen_call_(session, "Observer.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
