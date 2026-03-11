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
#include "xen_pool_allowed_operations_internal.h"
#include "xen_pool_guest_secureboot_readiness_internal.h"
#include "xen_string_pool_allowed_operations_map_internal.h"
#include "xen_telemetry_frequency_internal.h"
#include "xen_update_sync_frequency_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_pool.h>


XEN_FREE(xen_pool)
XEN_SET_ALLOC_FREE(xen_pool)
XEN_ALLOC(xen_pool_record)
XEN_SET_ALLOC_FREE(xen_pool_record)
XEN_ALLOC(xen_pool_record_opt)
XEN_RECORD_OPT_FREE(xen_pool)
XEN_SET_ALLOC_FREE(xen_pool_record_opt)


static const struct_member xen_pool_record_struct_members[] =
    {
        { .key = "uuid",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, uuid) },
        { .key = "name_label",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, name_label) },
        { .key = "name_description",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, name_description) },
        { .key = "master",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_record, master) },
        { .key = "default_SR",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_record, default_sr) },
        { .key = "suspend_image_SR",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_record, suspend_image_sr) },
        { .key = "crash_dump_SR",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_record, crash_dump_sr) },
        { .key = "other_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, other_config) },
        { .key = "ha_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, ha_enabled) },
        { .key = "ha_configuration",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, ha_configuration) },
        { .key = "ha_statefiles",
          .type = &abstract_type_string_set,
          .offset = offsetof(xen_pool_record, ha_statefiles) },
        { .key = "ha_host_failures_to_tolerate",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_record, ha_host_failures_to_tolerate) },
        { .key = "ha_plan_exists_for",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_record, ha_plan_exists_for) },
        { .key = "ha_allow_overcommit",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, ha_allow_overcommit) },
        { .key = "ha_overcommitted",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, ha_overcommitted) },
        { .key = "blobs",
          .type = &abstract_type_string_ref_map,
          .offset = offsetof(xen_pool_record, blobs) },
        { .key = "tags",
          .type = &abstract_type_string_set,
          .offset = offsetof(xen_pool_record, tags) },
        { .key = "gui_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, gui_config) },
        { .key = "health_check_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, health_check_config) },
        { .key = "wlb_url",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, wlb_url) },
        { .key = "wlb_username",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, wlb_username) },
        { .key = "wlb_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, wlb_enabled) },
        { .key = "wlb_verify_cert",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, wlb_verify_cert) },
        { .key = "redo_log_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, redo_log_enabled) },
        { .key = "redo_log_vdi",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_record, redo_log_vdi) },
        { .key = "vswitch_controller",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, vswitch_controller) },
        { .key = "restrictions",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, restrictions) },
        { .key = "metadata_VDIs",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_pool_record, metadata_vdis) },
        { .key = "ha_cluster_stack",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, ha_cluster_stack) },
        { .key = "allowed_operations",
          .type = &xen_pool_allowed_operations_set_abstract_type_,
          .offset = offsetof(xen_pool_record, allowed_operations) },
        { .key = "current_operations",
          .type = &string_pool_allowed_operations_map_abstract_type_,
          .offset = offsetof(xen_pool_record, current_operations) },
        { .key = "guest_agent_config",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, guest_agent_config) },
        { .key = "cpu_info",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, cpu_info) },
        { .key = "policy_no_vendor_device",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, policy_no_vendor_device) },
        { .key = "live_patching_disabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, live_patching_disabled) },
        { .key = "igmp_snooping_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, igmp_snooping_enabled) },
        { .key = "uefi_certificates",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, uefi_certificates) },
        { .key = "custom_uefi_certificates",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, custom_uefi_certificates) },
        { .key = "is_psr_pending",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, is_psr_pending) },
        { .key = "tls_verification_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, tls_verification_enabled) },
        { .key = "repositories",
          .type = &abstract_type_ref_set,
          .offset = offsetof(xen_pool_record, repositories) },
        { .key = "client_certificate_auth_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, client_certificate_auth_enabled) },
        { .key = "client_certificate_auth_name",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, client_certificate_auth_name) },
        { .key = "repository_proxy_url",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, repository_proxy_url) },
        { .key = "repository_proxy_username",
          .type = &abstract_type_string,
          .offset = offsetof(xen_pool_record, repository_proxy_username) },
        { .key = "repository_proxy_password",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_record, repository_proxy_password) },
        { .key = "migration_compression",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, migration_compression) },
        { .key = "coordinator_bias",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, coordinator_bias) },
        { .key = "local_auth_max_threads",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_record, local_auth_max_threads) },
        { .key = "ext_auth_max_threads",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_record, ext_auth_max_threads) },
        { .key = "ext_auth_cache_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, ext_auth_cache_enabled) },
        { .key = "ext_auth_cache_size",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_record, ext_auth_cache_size) },
        { .key = "ext_auth_cache_expiry",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_record, ext_auth_cache_expiry) },
        { .key = "telemetry_uuid",
          .type = &abstract_type_ref,
          .offset = offsetof(xen_pool_record, telemetry_uuid) },
        { .key = "telemetry_frequency",
          .type = &xen_telemetry_frequency_abstract_type_,
          .offset = offsetof(xen_pool_record, telemetry_frequency) },
        { .key = "telemetry_next_collection",
          .type = &abstract_type_datetime,
          .offset = offsetof(xen_pool_record, telemetry_next_collection) },
        { .key = "last_update_sync",
          .type = &abstract_type_datetime,
          .offset = offsetof(xen_pool_record, last_update_sync) },
        { .key = "update_sync_frequency",
          .type = &xen_update_sync_frequency_abstract_type_,
          .offset = offsetof(xen_pool_record, update_sync_frequency) },
        { .key = "update_sync_day",
          .type = &abstract_type_int,
          .offset = offsetof(xen_pool_record, update_sync_day) },
        { .key = "update_sync_enabled",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, update_sync_enabled) },
        { .key = "recommendations",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, recommendations) },
        { .key = "license_server",
          .type = &abstract_type_string_string_map,
          .offset = offsetof(xen_pool_record, license_server) },
        { .key = "ha_reboot_vm_on_internal_shutdown",
          .type = &abstract_type_bool,
          .offset = offsetof(xen_pool_record, ha_reboot_vm_on_internal_shutdown) }
    };


const abstract_type xen_pool_record_abstract_type_ =
    {
       .XEN_API_TYPE = STRUCT,
       .struct_size = sizeof(xen_pool_record),
       .member_count =
           sizeof(xen_pool_record_struct_members) / sizeof(struct_member),
       .members = xen_pool_record_struct_members
    };


const abstract_type xen_pool_record_set_abstract_type_ =
    {
       .XEN_API_TYPE = SET,
        .child = &xen_pool_record_abstract_type_
    };


static const struct struct_member xen_pool_xen_pool_record_members[] =
{
    {
        .type = &abstract_type_string,
        .offset = offsetof(xen_pool_xen_pool_record_map_contents, key)
    },
    {
        .type = &xen_pool_record_abstract_type_,
        .offset = offsetof(xen_pool_xen_pool_record_map_contents, val)
    }
};


const abstract_type abstract_type_string_xen_pool_record_map =
{
    .XEN_API_TYPE = MAP,
    .struct_size = sizeof(xen_pool_xen_pool_record_map_contents),
    .members = xen_pool_xen_pool_record_members
};


void
xen_pool_record_free(xen_pool_record *record)
{
    if (record == NULL)
        return;

    free(record->handle);
    free(record->uuid);
    free(record->name_label);
    free(record->name_description);
    xen_host_record_opt_free(record->master);
    xen_sr_record_opt_free(record->default_sr);
    xen_sr_record_opt_free(record->suspend_image_sr);
    xen_sr_record_opt_free(record->crash_dump_sr);
    xen_string_string_map_free(record->other_config);
    xen_string_string_map_free(record->ha_configuration);
    xen_string_set_free(record->ha_statefiles);
    xen_string_blob_map_free(record->blobs);
    xen_string_set_free(record->tags);
    xen_string_string_map_free(record->gui_config);
    xen_string_string_map_free(record->health_check_config);
    free(record->wlb_url);
    free(record->wlb_username);
    xen_vdi_record_opt_free(record->redo_log_vdi);
    free(record->vswitch_controller);
    xen_string_string_map_free(record->restrictions);
    xen_vdi_record_opt_set_free(record->metadata_vdis);
    free(record->ha_cluster_stack);
    xen_pool_allowed_operations_set_free(record->allowed_operations);
    xen_string_pool_allowed_operations_map_free(record->current_operations);
    xen_string_string_map_free(record->guest_agent_config);
    xen_string_string_map_free(record->cpu_info);
    free(record->uefi_certificates);
    free(record->custom_uefi_certificates);
    xen_repository_record_opt_set_free(record->repositories);
    free(record->client_certificate_auth_name);
    free(record->repository_proxy_url);
    free(record->repository_proxy_username);
    xen_secret_record_opt_free(record->repository_proxy_password);
    xen_secret_record_opt_free(record->telemetry_uuid);
    xen_string_string_map_free(record->recommendations);
    xen_string_string_map_free(record->license_server);
    free(record);
}


bool
xen_pool_get_record(xen_session *session, xen_pool_record **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = xen_pool_record_abstract_type_;

    *result = NULL;
    XEN_CALL_("pool.get_record");

    if (session->ok)
       (*result)->handle = xen_strdup_((*result)->uuid);

    return session->ok;
}


bool
xen_pool_get_by_uuid(xen_session *session, xen_pool *result, char *uuid)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = uuid }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_by_uuid");
    return session->ok;
}


bool
xen_pool_get_uuid(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_uuid");
    return session->ok;
}


bool
xen_pool_get_name_label(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_name_label");
    return session->ok;
}


bool
xen_pool_get_name_description(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_name_description");
    return session->ok;
}


bool
xen_pool_get_master(xen_session *session, xen_host *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_master");
    return session->ok;
}


bool
xen_pool_get_default_sr(xen_session *session, xen_sr *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_default_SR");
    return session->ok;
}


bool
xen_pool_get_suspend_image_sr(xen_session *session, xen_sr *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_suspend_image_SR");
    return session->ok;
}


bool
xen_pool_get_crash_dump_sr(xen_session *session, xen_sr *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_crash_dump_SR");
    return session->ok;
}


bool
xen_pool_get_other_config(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_other_config");
    return session->ok;
}


bool
xen_pool_get_ha_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_ha_enabled");
    return session->ok;
}


bool
xen_pool_get_ha_configuration(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_ha_configuration");
    return session->ok;
}


bool
xen_pool_get_ha_statefiles(xen_session *session, struct xen_string_set **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool.get_ha_statefiles");
    return session->ok;
}


bool
xen_pool_get_ha_host_failures_to_tolerate(xen_session *session, int64_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.get_ha_host_failures_to_tolerate");
    return session->ok;
}


bool
xen_pool_get_ha_plan_exists_for(xen_session *session, int64_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.get_ha_plan_exists_for");
    return session->ok;
}


bool
xen_pool_get_ha_allow_overcommit(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_ha_allow_overcommit");
    return session->ok;
}


bool
xen_pool_get_ha_overcommitted(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_ha_overcommitted");
    return session->ok;
}


bool
xen_pool_get_blobs(xen_session *session, xen_string_blob_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_blobs");
    return session->ok;
}


bool
xen_pool_get_tags(xen_session *session, struct xen_string_set **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool.get_tags");
    return session->ok;
}


bool
xen_pool_get_gui_config(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_gui_config");
    return session->ok;
}


bool
xen_pool_get_health_check_config(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_health_check_config");
    return session->ok;
}


bool
xen_pool_get_wlb_url(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_wlb_url");
    return session->ok;
}


bool
xen_pool_get_wlb_username(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_wlb_username");
    return session->ok;
}


bool
xen_pool_get_wlb_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_wlb_enabled");
    return session->ok;
}


bool
xen_pool_get_wlb_verify_cert(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_wlb_verify_cert");
    return session->ok;
}


bool
xen_pool_get_redo_log_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_redo_log_enabled");
    return session->ok;
}


bool
xen_pool_get_redo_log_vdi(xen_session *session, xen_vdi *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_redo_log_vdi");
    return session->ok;
}


bool
xen_pool_get_vswitch_controller(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_vswitch_controller");
    return session->ok;
}


bool
xen_pool_get_restrictions(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_restrictions");
    return session->ok;
}


bool
xen_pool_get_metadata_vdis(xen_session *session, struct xen_vdi_set **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool.get_metadata_VDIs");
    return session->ok;
}


bool
xen_pool_get_ha_cluster_stack(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_ha_cluster_stack");
    return session->ok;
}


bool
xen_pool_get_allowed_operations(xen_session *session, struct xen_pool_allowed_operations_set **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = xen_pool_allowed_operations_set_abstract_type_;

    *result = NULL;
    XEN_CALL_("pool.get_allowed_operations");
    return session->ok;
}


bool
xen_pool_get_current_operations(xen_session *session, xen_string_pool_allowed_operations_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = string_pool_allowed_operations_map_abstract_type_;

    *result = NULL;
    XEN_CALL_("pool.get_current_operations");
    return session->ok;
}


bool
xen_pool_get_guest_agent_config(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_guest_agent_config");
    return session->ok;
}


bool
xen_pool_get_cpu_info(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_cpu_info");
    return session->ok;
}


bool
xen_pool_get_policy_no_vendor_device(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_policy_no_vendor_device");
    return session->ok;
}


bool
xen_pool_get_live_patching_disabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_live_patching_disabled");
    return session->ok;
}


bool
xen_pool_get_igmp_snooping_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_igmp_snooping_enabled");
    return session->ok;
}


bool
xen_pool_get_uefi_certificates(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_uefi_certificates");
    return session->ok;
}


bool
xen_pool_get_custom_uefi_certificates(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_custom_uefi_certificates");
    return session->ok;
}


bool
xen_pool_get_is_psr_pending(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_is_psr_pending");
    return session->ok;
}


bool
xen_pool_get_tls_verification_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_tls_verification_enabled");
    return session->ok;
}


bool
xen_pool_get_repositories(xen_session *session, struct xen_repository_set **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool.get_repositories");
    return session->ok;
}


bool
xen_pool_get_client_certificate_auth_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_client_certificate_auth_enabled");
    return session->ok;
}


bool
xen_pool_get_client_certificate_auth_name(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_client_certificate_auth_name");
    return session->ok;
}


bool
xen_pool_get_repository_proxy_url(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_repository_proxy_url");
    return session->ok;
}


bool
xen_pool_get_repository_proxy_username(xen_session *session, char **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_repository_proxy_username");
    return session->ok;
}


bool
xen_pool_get_repository_proxy_password(xen_session *session, xen_secret *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_repository_proxy_password");
    return session->ok;
}


bool
xen_pool_get_migration_compression(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_migration_compression");
    return session->ok;
}


bool
xen_pool_get_coordinator_bias(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_coordinator_bias");
    return session->ok;
}


bool
xen_pool_get_local_auth_max_threads(xen_session *session, int64_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.get_local_auth_max_threads");
    return session->ok;
}


bool
xen_pool_get_ext_auth_max_threads(xen_session *session, int64_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.get_ext_auth_max_threads");
    return session->ok;
}


bool
xen_pool_get_ext_auth_cache_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_ext_auth_cache_enabled");
    return session->ok;
}


bool
xen_pool_get_ext_auth_cache_size(xen_session *session, int64_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.get_ext_auth_cache_size");
    return session->ok;
}


bool
xen_pool_get_ext_auth_cache_expiry(xen_session *session, int64_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.get_ext_auth_cache_expiry");
    return session->ok;
}


bool
xen_pool_get_telemetry_uuid(xen_session *session, xen_secret *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.get_telemetry_uuid");
    return session->ok;
}


bool
xen_pool_get_telemetry_frequency(xen_session *session, enum xen_telemetry_frequency *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = xen_telemetry_frequency_abstract_type_;

    XEN_CALL_("pool.get_telemetry_frequency");
    return session->ok;
}


bool
xen_pool_get_telemetry_next_collection(xen_session *session, time_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_datetime;

    XEN_CALL_("pool.get_telemetry_next_collection");
    return session->ok;
}


bool
xen_pool_get_last_update_sync(xen_session *session, time_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_datetime;

    XEN_CALL_("pool.get_last_update_sync");
    return session->ok;
}


bool
xen_pool_get_update_sync_frequency(xen_session *session, enum xen_update_sync_frequency *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = xen_update_sync_frequency_abstract_type_;

    XEN_CALL_("pool.get_update_sync_frequency");
    return session->ok;
}


bool
xen_pool_get_update_sync_day(xen_session *session, int64_t *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.get_update_sync_day");
    return session->ok;
}


bool
xen_pool_get_update_sync_enabled(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_update_sync_enabled");
    return session->ok;
}


bool
xen_pool_get_recommendations(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_recommendations");
    return session->ok;
}


bool
xen_pool_get_license_server(xen_session *session, xen_string_string_map **result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_license_server");
    return session->ok;
}


bool
xen_pool_get_ha_reboot_vm_on_internal_shutdown(xen_session *session, bool *result, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.get_ha_reboot_vm_on_internal_shutdown");
    return session->ok;
}


bool
xen_pool_set_name_label(xen_session *session, xen_pool pool, char *name_label)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = name_label }
        };

    xen_call_(session, "pool.set_name_label", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_name_description(xen_session *session, xen_pool pool, char *name_description)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = name_description }
        };

    xen_call_(session, "pool.set_name_description", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_default_sr(xen_session *session, xen_pool pool, xen_sr default_sr)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = default_sr }
        };

    xen_call_(session, "pool.set_default_SR", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_suspend_image_sr(xen_session *session, xen_pool pool, xen_sr suspend_image_sr)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = suspend_image_sr }
        };

    xen_call_(session, "pool.set_suspend_image_SR", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_crash_dump_sr(xen_session *session, xen_pool pool, xen_sr crash_dump_sr)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = crash_dump_sr }
        };

    xen_call_(session, "pool.set_crash_dump_SR", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_other_config(xen_session *session, xen_pool pool, xen_string_string_map *other_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)other_config }
        };

    xen_call_(session, "pool.set_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_add_to_other_config(xen_session *session, xen_pool pool, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.add_to_other_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_remove_from_other_config(xen_session *session, xen_pool pool, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "pool.remove_from_other_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_ha_allow_overcommit(xen_session *session, xen_pool pool, bool ha_allow_overcommit)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = ha_allow_overcommit }
        };

    xen_call_(session, "pool.set_ha_allow_overcommit", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_tags(xen_session *session, xen_pool pool, struct xen_string_set *tags)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)tags }
        };

    xen_call_(session, "pool.set_tags", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_add_tags(xen_session *session, xen_pool pool, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.add_tags", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_remove_tags(xen_session *session, xen_pool pool, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.remove_tags", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_gui_config(xen_session *session, xen_pool pool, xen_string_string_map *gui_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)gui_config }
        };

    xen_call_(session, "pool.set_gui_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_add_to_gui_config(xen_session *session, xen_pool pool, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.add_to_gui_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_remove_from_gui_config(xen_session *session, xen_pool pool, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "pool.remove_from_gui_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_health_check_config(xen_session *session, xen_pool pool, xen_string_string_map *health_check_config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)health_check_config }
        };

    xen_call_(session, "pool.set_health_check_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_add_to_health_check_config(xen_session *session, xen_pool pool, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.add_to_health_check_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_remove_from_health_check_config(xen_session *session, xen_pool pool, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "pool.remove_from_health_check_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_wlb_enabled(xen_session *session, xen_pool pool, bool wlb_enabled)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = wlb_enabled }
        };

    xen_call_(session, "pool.set_wlb_enabled", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_wlb_verify_cert(xen_session *session, xen_pool pool, bool wlb_verify_cert)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = wlb_verify_cert }
        };

    xen_call_(session, "pool.set_wlb_verify_cert", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_policy_no_vendor_device(xen_session *session, xen_pool pool, bool policy_no_vendor_device)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = policy_no_vendor_device }
        };

    xen_call_(session, "pool.set_policy_no_vendor_device", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_live_patching_disabled(xen_session *session, xen_pool pool, bool live_patching_disabled)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = live_patching_disabled }
        };

    xen_call_(session, "pool.set_live_patching_disabled", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_is_psr_pending(xen_session *session, xen_pool pool, bool is_psr_pending)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = is_psr_pending }
        };

    xen_call_(session, "pool.set_is_psr_pending", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_migration_compression(xen_session *session, xen_pool pool, bool migration_compression)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = migration_compression }
        };

    xen_call_(session, "pool.set_migration_compression", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_coordinator_bias(xen_session *session, xen_pool pool, bool coordinator_bias)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = coordinator_bias }
        };

    xen_call_(session, "pool.set_coordinator_bias", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_license_server(xen_session *session, xen_pool pool, xen_string_string_map *license_server)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)license_server }
        };

    xen_call_(session, "pool.set_license_server", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_add_to_license_server(xen_session *session, xen_pool pool, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.add_to_license_server", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_remove_from_license_server(xen_session *session, xen_pool pool, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "pool.remove_from_license_server", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_ha_reboot_vm_on_internal_shutdown(xen_session *session, xen_pool pool, bool ha_reboot_vm_on_internal_shutdown)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_bool,
              .u.bool_val = ha_reboot_vm_on_internal_shutdown }
        };

    xen_call_(session, "pool.set_ha_reboot_vm_on_internal_shutdown", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_join(xen_session *session, char *master_address, char *master_username, char *master_password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = master_address },
            { .type = &abstract_type_string,
              .u.string_val = master_username },
            { .type = &abstract_type_string,
              .u.string_val = master_password }
        };

    xen_call_(session, "pool.join", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_join_async(xen_session *session, xen_task *result, char *master_address, char *master_username, char *master_password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = master_address },
            { .type = &abstract_type_string,
              .u.string_val = master_username },
            { .type = &abstract_type_string,
              .u.string_val = master_password }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.join");
    return session->ok;
}


bool
xen_pool_join_force(xen_session *session, char *master_address, char *master_username, char *master_password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = master_address },
            { .type = &abstract_type_string,
              .u.string_val = master_username },
            { .type = &abstract_type_string,
              .u.string_val = master_password }
        };

    xen_call_(session, "pool.join_force", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_join_force_async(xen_session *session, xen_task *result, char *master_address, char *master_username, char *master_password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = master_address },
            { .type = &abstract_type_string,
              .u.string_val = master_username },
            { .type = &abstract_type_string,
              .u.string_val = master_password }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.join_force");
    return session->ok;
}


bool
xen_pool_eject(xen_session *session, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    xen_call_(session, "pool.eject", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_eject_async(xen_session *session, xen_task *result, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.eject");
    return session->ok;
}


bool
xen_pool_emergency_transition_to_master(xen_session *session)
{

    xen_call_(session, "pool.emergency_transition_to_master", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_emergency_reset_master(xen_session *session, char *master_address)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = master_address }
        };

    xen_call_(session, "pool.emergency_reset_master", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_recover_slaves(xen_session *session, struct xen_host_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "pool.recover_slaves", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_recover_slaves_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.recover_slaves", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_create_vlan(xen_session *session, struct xen_pif_set **result, char *device, xen_network network, int64_t vlan)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = device },
            { .type = &abstract_type_string,
              .u.string_val = network },
            { .type = &abstract_type_int,
              .u.int_val = vlan }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool.create_VLAN");
    return session->ok;
}


bool
xen_pool_create_vlan_async(xen_session *session, xen_task *result, char *device, xen_network network, int64_t vlan)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = device },
            { .type = &abstract_type_string,
              .u.string_val = network },
            { .type = &abstract_type_int,
              .u.int_val = vlan }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.create_VLAN");
    return session->ok;
}


bool
xen_pool_management_reconfigure(xen_session *session, xen_network network)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network }
        };

    xen_call_(session, "pool.management_reconfigure", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_management_reconfigure_async(xen_session *session, xen_task *result, xen_network network)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = network }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.management_reconfigure");
    return session->ok;
}


bool
xen_pool_create_vlan_from_pif(xen_session *session, struct xen_pif_set **result, xen_pif pif, xen_network network, int64_t vlan)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pif },
            { .type = &abstract_type_string,
              .u.string_val = network },
            { .type = &abstract_type_int,
              .u.int_val = vlan }
        };

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    XEN_CALL_("pool.create_VLAN_from_PIF");
    return session->ok;
}


bool
xen_pool_create_vlan_from_pif_async(xen_session *session, xen_task *result, xen_pif pif, xen_network network, int64_t vlan)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pif },
            { .type = &abstract_type_string,
              .u.string_val = network },
            { .type = &abstract_type_int,
              .u.int_val = vlan }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.create_VLAN_from_PIF");
    return session->ok;
}


bool
xen_pool_enable_ha(xen_session *session, struct xen_sr_set *heartbeat_srs, xen_string_string_map *configuration)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)heartbeat_srs },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)configuration }
        };

    xen_call_(session, "pool.enable_ha", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_enable_ha_async(xen_session *session, xen_task *result, struct xen_sr_set *heartbeat_srs, xen_string_string_map *configuration)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)heartbeat_srs },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)configuration }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.enable_ha");
    return session->ok;
}


bool
xen_pool_disable_ha(xen_session *session)
{

    xen_call_(session, "pool.disable_ha", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_ha_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.disable_ha", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_sync_database(xen_session *session)
{

    xen_call_(session, "pool.sync_database", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_sync_database_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.sync_database", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_designate_new_master(xen_session *session, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    xen_call_(session, "pool.designate_new_master", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_designate_new_master_async(xen_session *session, xen_task *result, xen_host host)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.designate_new_master");
    return session->ok;
}


bool
xen_pool_ha_prevent_restarts_for(xen_session *session, int64_t seconds)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_int,
              .u.int_val = seconds }
        };

    xen_call_(session, "pool.ha_prevent_restarts_for", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_ha_failover_plan_exists(xen_session *session, bool *result, int64_t n)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_int,
              .u.int_val = n }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.ha_failover_plan_exists");
    return session->ok;
}


bool
xen_pool_ha_compute_max_host_failures_to_tolerate(xen_session *session, int64_t *result)
{

    abstract_type result_type = abstract_type_int;

    xen_call_(session, "pool.ha_compute_max_host_failures_to_tolerate", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_ha_compute_hypothetical_max_host_failures_to_tolerate(xen_session *session, int64_t *result, xen_vm_string_map *configuration)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)configuration }
        };

    abstract_type result_type = abstract_type_int;

    XEN_CALL_("pool.ha_compute_hypothetical_max_host_failures_to_tolerate");
    return session->ok;
}


bool
xen_pool_ha_compute_vm_failover_plan(xen_session *session, xen_vm_string_string_map_map **result, struct xen_host_set *failed_hosts, struct xen_vm_set *failed_vms)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)failed_hosts },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)failed_vms }
        };

    abstract_type result_type = abstract_type_string_string_string_map_map;

    *result = NULL;
    XEN_CALL_("pool.ha_compute_vm_failover_plan");
    return session->ok;
}


bool
xen_pool_set_ha_host_failures_to_tolerate(xen_session *session, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    xen_call_(session, "pool.set_ha_host_failures_to_tolerate", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_ha_host_failures_to_tolerate_async(xen_session *session, xen_task *result, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.set_ha_host_failures_to_tolerate");
    return session->ok;
}


bool
xen_pool_create_new_blob(xen_session *session, xen_blob *result, xen_pool pool, char *name, char *mime_type, bool pubblic)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = mime_type },
            { .type = &abstract_type_bool,
              .u.bool_val = pubblic }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.create_new_blob");
    return session->ok;
}


bool
xen_pool_create_new_blob_async(xen_session *session, xen_task *result, xen_pool pool, char *name, char *mime_type, bool pubblic)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = mime_type },
            { .type = &abstract_type_bool,
              .u.bool_val = pubblic }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.create_new_blob");
    return session->ok;
}


bool
xen_pool_enable_external_auth(xen_session *session, xen_pool pool, xen_string_string_map *config, char *service_name, char *auth_type)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)config },
            { .type = &abstract_type_string,
              .u.string_val = service_name },
            { .type = &abstract_type_string,
              .u.string_val = auth_type }
        };

    xen_call_(session, "pool.enable_external_auth", param_values, 4, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_external_auth(xen_session *session, xen_pool pool, xen_string_string_map *config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)config }
        };

    xen_call_(session, "pool.disable_external_auth", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_detect_nonhomogeneous_external_auth(xen_session *session, xen_pool pool)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = pool }
        };

    xen_call_(session, "pool.detect_nonhomogeneous_external_auth", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_initialize_wlb(xen_session *session, char *wlb_url, char *wlb_username, char *wlb_password, char *xenserver_username, char *xenserver_password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = wlb_url },
            { .type = &abstract_type_string,
              .u.string_val = wlb_username },
            { .type = &abstract_type_string,
              .u.string_val = wlb_password },
            { .type = &abstract_type_string,
              .u.string_val = xenserver_username },
            { .type = &abstract_type_string,
              .u.string_val = xenserver_password }
        };

    xen_call_(session, "pool.initialize_wlb", param_values, 5, NULL, NULL);
    return session->ok;
}


bool
xen_pool_initialize_wlb_async(xen_session *session, xen_task *result, char *wlb_url, char *wlb_username, char *wlb_password, char *xenserver_username, char *xenserver_password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = wlb_url },
            { .type = &abstract_type_string,
              .u.string_val = wlb_username },
            { .type = &abstract_type_string,
              .u.string_val = wlb_password },
            { .type = &abstract_type_string,
              .u.string_val = xenserver_username },
            { .type = &abstract_type_string,
              .u.string_val = xenserver_password }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.initialize_wlb");
    return session->ok;
}


bool
xen_pool_deconfigure_wlb(xen_session *session)
{

    xen_call_(session, "pool.deconfigure_wlb", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_deconfigure_wlb_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.deconfigure_wlb", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_send_wlb_configuration(xen_session *session, xen_string_string_map *config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)config }
        };

    xen_call_(session, "pool.send_wlb_configuration", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_send_wlb_configuration_async(xen_session *session, xen_task *result, xen_string_string_map *config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)config }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.send_wlb_configuration");
    return session->ok;
}


bool
xen_pool_retrieve_wlb_configuration(xen_session *session, xen_string_string_map **result)
{

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    xen_call_(session, "pool.retrieve_wlb_configuration", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_retrieve_wlb_configuration_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.retrieve_wlb_configuration", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_retrieve_wlb_recommendations(xen_session *session, xen_vm_string_set_map **result)
{

    abstract_type result_type = abstract_type_string_string_set_map;

    *result = NULL;
    xen_call_(session, "pool.retrieve_wlb_recommendations", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_retrieve_wlb_recommendations_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.retrieve_wlb_recommendations", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_send_test_post(xen_session *session, char **result, char *host, int64_t port, char *body)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host },
            { .type = &abstract_type_int,
              .u.int_val = port },
            { .type = &abstract_type_string,
              .u.string_val = body }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.send_test_post");
    return session->ok;
}


bool
xen_pool_send_test_post_async(xen_session *session, xen_task *result, char *host, int64_t port, char *body)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = host },
            { .type = &abstract_type_int,
              .u.int_val = port },
            { .type = &abstract_type_string,
              .u.string_val = body }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.send_test_post");
    return session->ok;
}


bool
xen_pool_certificate_install(xen_session *session, char *name, char *cert)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = cert }
        };

    xen_call_(session, "pool.certificate_install", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_certificate_install_async(xen_session *session, xen_task *result, char *name, char *cert)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = cert }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.certificate_install");
    return session->ok;
}


bool
xen_pool_certificate_uninstall(xen_session *session, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    xen_call_(session, "pool.certificate_uninstall", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_certificate_uninstall_async(xen_session *session, xen_task *result, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.certificate_uninstall");
    return session->ok;
}


bool
xen_pool_certificate_list(xen_session *session, struct xen_string_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "pool.certificate_list", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_certificate_list_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.certificate_list", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_install_ca_certificate(xen_session *session, char *name, char *cert)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = cert }
        };

    xen_call_(session, "pool.install_ca_certificate", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_install_ca_certificate_async(xen_session *session, xen_task *result, char *name, char *cert)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = cert }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.install_ca_certificate");
    return session->ok;
}


bool
xen_pool_uninstall_ca_certificate(xen_session *session, char *name, bool force)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_bool,
              .u.bool_val = force }
        };

    xen_call_(session, "pool.uninstall_ca_certificate", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_uninstall_ca_certificate_async(xen_session *session, xen_task *result, char *name, bool force)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_bool,
              .u.bool_val = force }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.uninstall_ca_certificate");
    return session->ok;
}


bool
xen_pool_crl_install(xen_session *session, char *name, char *cert)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = cert }
        };

    xen_call_(session, "pool.crl_install", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_crl_install_async(xen_session *session, xen_task *result, char *name, char *cert)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name },
            { .type = &abstract_type_string,
              .u.string_val = cert }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.crl_install");
    return session->ok;
}


bool
xen_pool_crl_uninstall(xen_session *session, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    xen_call_(session, "pool.crl_uninstall", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_crl_uninstall_async(xen_session *session, xen_task *result, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.crl_uninstall");
    return session->ok;
}


bool
xen_pool_crl_list(xen_session *session, struct xen_string_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "pool.crl_list", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_crl_list_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.crl_list", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_certificate_sync(xen_session *session)
{

    xen_call_(session, "pool.certificate_sync", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_certificate_sync_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.certificate_sync", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_enable_tls_verification(xen_session *session)
{

    xen_call_(session, "pool.enable_tls_verification", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_enable_redo_log(xen_session *session, xen_sr sr)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = sr }
        };

    xen_call_(session, "pool.enable_redo_log", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_enable_redo_log_async(xen_session *session, xen_task *result, xen_sr sr)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = sr }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.enable_redo_log");
    return session->ok;
}


bool
xen_pool_disable_redo_log(xen_session *session)
{

    xen_call_(session, "pool.disable_redo_log", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_redo_log_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.disable_redo_log", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_set_vswitch_controller(xen_session *session, char *address)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = address }
        };

    xen_call_(session, "pool.set_vswitch_controller", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_vswitch_controller_async(xen_session *session, xen_task *result, char *address)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = address }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.set_vswitch_controller");
    return session->ok;
}


bool
xen_pool_test_archive_target(xen_session *session, char **result, xen_pool self, xen_string_string_map *config)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_string_map,
              .u.set_val = (arbitrary_set *)config }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.test_archive_target");
    return session->ok;
}


bool
xen_pool_enable_local_storage_caching(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.enable_local_storage_caching", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_enable_local_storage_caching_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.enable_local_storage_caching");
    return session->ok;
}


bool
xen_pool_disable_local_storage_caching(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.disable_local_storage_caching", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_local_storage_caching_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.disable_local_storage_caching");
    return session->ok;
}


bool
xen_pool_get_license_state(xen_session *session, xen_string_string_map **result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string_string_map;

    *result = NULL;
    XEN_CALL_("pool.get_license_state");
    return session->ok;
}


bool
xen_pool_get_license_state_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.get_license_state");
    return session->ok;
}


bool
xen_pool_apply_edition(xen_session *session, xen_pool self, char *edition)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = edition }
        };

    xen_call_(session, "pool.apply_edition", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_apply_edition_async(xen_session *session, xen_task *result, xen_pool self, char *edition)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = edition }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.apply_edition");
    return session->ok;
}


bool
xen_pool_enable_ssl_legacy(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.enable_ssl_legacy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_enable_ssl_legacy_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.enable_ssl_legacy");
    return session->ok;
}


bool
xen_pool_disable_ssl_legacy(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.disable_ssl_legacy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_ssl_legacy_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.disable_ssl_legacy");
    return session->ok;
}


bool
xen_pool_set_igmp_snooping_enabled(xen_session *session, xen_pool self, bool value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = value }
        };

    xen_call_(session, "pool.set_igmp_snooping_enabled", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_igmp_snooping_enabled_async(xen_session *session, xen_task *result, xen_pool self, bool value)
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
    XEN_CALL_("Async.pool.set_igmp_snooping_enabled");
    return session->ok;
}


bool
xen_pool_has_extension(xen_session *session, bool *result, xen_pool self, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    abstract_type result_type = abstract_type_bool;

    XEN_CALL_("pool.has_extension");
    return session->ok;
}


bool
xen_pool_has_extension_async(xen_session *session, xen_task *result, xen_pool self, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.has_extension");
    return session->ok;
}


bool
xen_pool_add_to_guest_agent_config(xen_session *session, xen_pool self, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.add_to_guest_agent_config", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_add_to_guest_agent_config_async(xen_session *session, xen_task *result, xen_pool self, char *key, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = key },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.add_to_guest_agent_config");
    return session->ok;
}


bool
xen_pool_remove_from_guest_agent_config(xen_session *session, xen_pool self, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    xen_call_(session, "pool.remove_from_guest_agent_config", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_remove_from_guest_agent_config_async(xen_session *session, xen_task *result, xen_pool self, char *key)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = key }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.remove_from_guest_agent_config");
    return session->ok;
}


bool
xen_pool_rotate_secret(xen_session *session)
{

    xen_call_(session, "pool.rotate_secret", NULL, 0, NULL, NULL);
    return session->ok;
}


bool
xen_pool_rotate_secret_async(xen_session *session, xen_task *result)
{

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    xen_call_(session, "Async.pool.rotate_secret", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_set_repositories(xen_session *session, xen_pool self, struct xen_repository_set *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string_set,
              .u.set_val = (arbitrary_set *)value }
        };

    xen_call_(session, "pool.set_repositories", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_repositories_async(xen_session *session, xen_task *result, xen_pool self, struct xen_repository_set *value)
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
    XEN_CALL_("Async.pool.set_repositories");
    return session->ok;
}


bool
xen_pool_add_repository(xen_session *session, xen_pool self, xen_repository value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.add_repository", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_add_repository_async(xen_session *session, xen_task *result, xen_pool self, xen_repository value)
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
    XEN_CALL_("Async.pool.add_repository");
    return session->ok;
}


bool
xen_pool_remove_repository(xen_session *session, xen_pool self, xen_repository value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.remove_repository", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_remove_repository_async(xen_session *session, xen_task *result, xen_pool self, xen_repository value)
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
    XEN_CALL_("Async.pool.remove_repository");
    return session->ok;
}


bool
xen_pool_sync_updates(xen_session *session, char **result, xen_pool self, bool force, char *token, char *token_id, char *username, char *password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = force },
            { .type = &abstract_type_string,
              .u.string_val = token },
            { .type = &abstract_type_string,
              .u.string_val = token_id },
            { .type = &abstract_type_string,
              .u.string_val = username },
            { .type = &abstract_type_string,
              .u.string_val = password }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("pool.sync_updates");
    return session->ok;
}


bool
xen_pool_sync_updates_async(xen_session *session, xen_task *result, xen_pool self, bool force, char *token, char *token_id, char *username, char *password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = force },
            { .type = &abstract_type_string,
              .u.string_val = token },
            { .type = &abstract_type_string,
              .u.string_val = token_id },
            { .type = &abstract_type_string,
              .u.string_val = username },
            { .type = &abstract_type_string,
              .u.string_val = password }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.sync_updates");
    return session->ok;
}


bool
xen_pool_check_update_readiness(xen_session *session, struct xen_string_set_set **result, xen_pool self, bool requires_reboot)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = requires_reboot }
        };

    abstract_type result_type = abstract_type_string_set_set;

    *result = NULL;
    XEN_CALL_("pool.check_update_readiness");
    return session->ok;
}


bool
xen_pool_check_update_readiness_async(xen_session *session, xen_task *result, xen_pool self, bool requires_reboot)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = requires_reboot }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.check_update_readiness");
    return session->ok;
}


bool
xen_pool_enable_client_certificate_auth(xen_session *session, xen_pool self, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    xen_call_(session, "pool.enable_client_certificate_auth", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_enable_client_certificate_auth_async(xen_session *session, xen_task *result, xen_pool self, char *name)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = name }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.enable_client_certificate_auth");
    return session->ok;
}


bool
xen_pool_disable_client_certificate_auth(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.disable_client_certificate_auth", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_client_certificate_auth_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.disable_client_certificate_auth");
    return session->ok;
}


bool
xen_pool_configure_repository_proxy(xen_session *session, xen_pool self, char *url, char *username, char *password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = url },
            { .type = &abstract_type_string,
              .u.string_val = username },
            { .type = &abstract_type_string,
              .u.string_val = password }
        };

    xen_call_(session, "pool.configure_repository_proxy", param_values, 4, NULL, NULL);
    return session->ok;
}


bool
xen_pool_configure_repository_proxy_async(xen_session *session, xen_task *result, xen_pool self, char *url, char *username, char *password)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = url },
            { .type = &abstract_type_string,
              .u.string_val = username },
            { .type = &abstract_type_string,
              .u.string_val = password }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.configure_repository_proxy");
    return session->ok;
}


bool
xen_pool_disable_repository_proxy(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.disable_repository_proxy", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_repository_proxy_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.disable_repository_proxy");
    return session->ok;
}


bool
xen_pool_set_uefi_certificates(xen_session *session, xen_pool self, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.set_uefi_certificates", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_uefi_certificates_async(xen_session *session, xen_task *result, xen_pool self, char *value)
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
    XEN_CALL_("Async.pool.set_uefi_certificates");
    return session->ok;
}


bool
xen_pool_set_custom_uefi_certificates(xen_session *session, xen_pool self, char *value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_string,
              .u.string_val = value }
        };

    xen_call_(session, "pool.set_custom_uefi_certificates", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_custom_uefi_certificates_async(xen_session *session, xen_task *result, xen_pool self, char *value)
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
    XEN_CALL_("Async.pool.set_custom_uefi_certificates");
    return session->ok;
}


bool
xen_pool_set_https_only(xen_session *session, xen_pool self, bool value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = value }
        };

    xen_call_(session, "pool.set_https_only", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_https_only_async(xen_session *session, xen_task *result, xen_pool self, bool value)
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
    XEN_CALL_("Async.pool.set_https_only");
    return session->ok;
}


bool
xen_pool_set_telemetry_next_collection(xen_session *session, xen_pool self, time_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_datetime,
              .u.datetime_val = value }
        };

    xen_call_(session, "pool.set_telemetry_next_collection", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_telemetry_next_collection_async(xen_session *session, xen_task *result, xen_pool self, time_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_datetime,
              .u.datetime_val = value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.set_telemetry_next_collection");
    return session->ok;
}


bool
xen_pool_reset_telemetry_uuid(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.reset_telemetry_uuid", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_reset_telemetry_uuid_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.reset_telemetry_uuid");
    return session->ok;
}


bool
xen_pool_configure_update_sync(xen_session *session, xen_pool self, enum xen_update_sync_frequency update_sync_frequency, int64_t update_sync_day)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &xen_update_sync_frequency_abstract_type_,
              .u.enum_val = update_sync_frequency },
            { .type = &abstract_type_int,
              .u.int_val = update_sync_day }
        };

    xen_call_(session, "pool.configure_update_sync", param_values, 3, NULL, NULL);
    return session->ok;
}


bool
xen_pool_configure_update_sync_async(xen_session *session, xen_task *result, xen_pool self, enum xen_update_sync_frequency update_sync_frequency, int64_t update_sync_day)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &xen_update_sync_frequency_abstract_type_,
              .u.enum_val = update_sync_frequency },
            { .type = &abstract_type_int,
              .u.int_val = update_sync_day }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.configure_update_sync");
    return session->ok;
}


bool
xen_pool_set_update_sync_enabled(xen_session *session, xen_pool self, bool value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = value }
        };

    xen_call_(session, "pool.set_update_sync_enabled", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_update_sync_enabled_async(xen_session *session, xen_task *result, xen_pool self, bool value)
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
    XEN_CALL_("Async.pool.set_update_sync_enabled");
    return session->ok;
}


bool
xen_pool_set_local_auth_max_threads(xen_session *session, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    xen_call_(session, "pool.set_local_auth_max_threads", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_ext_auth_max_threads(xen_session *session, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    xen_call_(session, "pool.set_ext_auth_max_threads", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_get_guest_secureboot_readiness(xen_session *session, enum xen_pool_guest_secureboot_readiness *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = xen_pool_guest_secureboot_readiness_abstract_type_;

    XEN_CALL_("pool.get_guest_secureboot_readiness");
    return session->ok;
}


bool
xen_pool_enable_ssh(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.enable_ssh", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_enable_ssh_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.enable_ssh");
    return session->ok;
}


bool
xen_pool_disable_ssh(xen_session *session, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    xen_call_(session, "pool.disable_ssh", param_values, 1, NULL, NULL);
    return session->ok;
}


bool
xen_pool_disable_ssh_async(xen_session *session, xen_task *result, xen_pool self)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.disable_ssh");
    return session->ok;
}


bool
xen_pool_set_ssh_enabled_timeout(xen_session *session, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    xen_call_(session, "pool.set_ssh_enabled_timeout", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_ssh_enabled_timeout_async(xen_session *session, xen_task *result, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.set_ssh_enabled_timeout");
    return session->ok;
}


bool
xen_pool_set_console_idle_timeout(xen_session *session, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    xen_call_(session, "pool.set_console_idle_timeout", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_console_idle_timeout_async(xen_session *session, xen_task *result, xen_pool self, int64_t value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_int,
              .u.int_val = value }
        };

    abstract_type result_type = abstract_type_string;

    *result = NULL;
    XEN_CALL_("Async.pool.set_console_idle_timeout");
    return session->ok;
}


bool
xen_pool_set_ssh_auto_mode(xen_session *session, xen_pool self, bool value)
{
    abstract_value param_values[] =
        {
            { .type = &abstract_type_string,
              .u.string_val = self },
            { .type = &abstract_type_bool,
              .u.bool_val = value }
        };

    xen_call_(session, "pool.set_ssh_auto_mode", param_values, 2, NULL, NULL);
    return session->ok;
}


bool
xen_pool_set_ssh_auto_mode_async(xen_session *session, xen_task *result, xen_pool self, bool value)
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
    XEN_CALL_("Async.pool.set_ssh_auto_mode");
    return session->ok;
}


bool
xen_pool_get_all(xen_session *session, struct xen_pool_set **result)
{

    abstract_type result_type = abstract_type_string_set;

    *result = NULL;
    xen_call_(session, "pool.get_all", NULL, 0, &result_type, result);
    return session->ok;
}


bool
xen_pool_get_all_records(xen_session *session, xen_pool_xen_pool_record_map **result)
{

    abstract_type result_type = abstract_type_string_xen_pool_record_map;

    *result = NULL;
    xen_call_(session, "pool.get_all_records", NULL, 0, &result_type, result);
    return session->ok;
}
