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


#ifndef XEN_POOL_H
#define XEN_POOL_H

#include <xen/api/xen_blob_decl.h>
#include <xen/api/xen_common.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_network_decl.h>
#include <xen/api/xen_pif_decl.h>
#include <xen/api/xen_pool_allowed_operations.h>
#include <xen/api/xen_pool_decl.h>
#include <xen/api/xen_pool_guest_secureboot_readiness.h>
#include <xen/api/xen_pool_xen_pool_record_map.h>
#include <xen/api/xen_repository.h>
#include <xen/api/xen_repository_decl.h>
#include <xen/api/xen_secret.h>
#include <xen/api/xen_secret_decl.h>
#include <xen/api/xen_sr.h>
#include <xen/api/xen_sr_decl.h>
#include <xen/api/xen_string_blob_map.h>
#include <xen/api/xen_string_pool_allowed_operations_map.h>
#include <xen/api/xen_string_set.h>
#include <xen/api/xen_string_set_set.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_telemetry_frequency.h>
#include <xen/api/xen_update_sync_frequency.h>
#include <xen/api/xen_vdi.h>
#include <xen/api/xen_vdi_decl.h>
#include <xen/api/xen_vm_decl.h>
#include <xen/api/xen_vm_string_map.h>
#include <xen/api/xen_vm_string_set_map.h>
#include <xen/api/xen_vm_string_string_map_map.h>


/*
 * The pool class.
 * 
 * Pool-wide information.
 */


/**
 * Free the given xen_pool.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_pool_free(xen_pool pool);


typedef struct xen_pool_set
{
    size_t size;
    xen_pool *contents[];
} xen_pool_set;

/**
 * Allocate a xen_pool_set of the given size.
 */
extern xen_pool_set *
xen_pool_set_alloc(size_t size);

/**
 * Free the given xen_pool_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_pool_set_free(xen_pool_set *set);


typedef struct xen_pool_record
{
    xen_pool handle;
    char *uuid;
    char *name_label;
    char *name_description;
    struct xen_host_record_opt *master;
    struct xen_sr_record_opt *default_sr;
    struct xen_sr_record_opt *suspend_image_sr;
    struct xen_sr_record_opt *crash_dump_sr;
    xen_string_string_map *other_config;
    bool ha_enabled;
    xen_string_string_map *ha_configuration;
    struct xen_string_set *ha_statefiles;
    int64_t ha_host_failures_to_tolerate;
    int64_t ha_plan_exists_for;
    bool ha_allow_overcommit;
    bool ha_overcommitted;
    xen_string_blob_map *blobs;
    struct xen_string_set *tags;
    xen_string_string_map *gui_config;
    xen_string_string_map *health_check_config;
    char *wlb_url;
    char *wlb_username;
    bool wlb_enabled;
    bool wlb_verify_cert;
    bool redo_log_enabled;
    struct xen_vdi_record_opt *redo_log_vdi;
    char *vswitch_controller;
    xen_string_string_map *restrictions;
    struct xen_vdi_record_opt_set *metadata_vdis;
    char *ha_cluster_stack;
    struct xen_pool_allowed_operations_set *allowed_operations;
    xen_string_pool_allowed_operations_map *current_operations;
    xen_string_string_map *guest_agent_config;
    xen_string_string_map *cpu_info;
    bool policy_no_vendor_device;
    bool live_patching_disabled;
    bool igmp_snooping_enabled;
    char *uefi_certificates;
    char *custom_uefi_certificates;
    bool is_psr_pending;
    bool tls_verification_enabled;
    struct xen_repository_record_opt_set *repositories;
    bool client_certificate_auth_enabled;
    char *client_certificate_auth_name;
    char *repository_proxy_url;
    char *repository_proxy_username;
    struct xen_secret_record_opt *repository_proxy_password;
    bool migration_compression;
    bool coordinator_bias;
    int64_t local_auth_max_threads;
    int64_t ext_auth_max_threads;
    bool ext_auth_cache_enabled;
    int64_t ext_auth_cache_size;
    int64_t ext_auth_cache_expiry;
    struct xen_secret_record_opt *telemetry_uuid;
    enum xen_telemetry_frequency telemetry_frequency;
    time_t telemetry_next_collection;
    time_t last_update_sync;
    enum xen_update_sync_frequency update_sync_frequency;
    int64_t update_sync_day;
    bool update_sync_enabled;
    xen_string_string_map *recommendations;
    xen_string_string_map *license_server;
    bool ha_reboot_vm_on_internal_shutdown;
} xen_pool_record;

/**
 * Allocate a xen_pool_record.
 */
extern xen_pool_record *
xen_pool_record_alloc(void);

/**
 * Free the given xen_pool_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_pool_record_free(xen_pool_record *record);


typedef struct xen_pool_record_opt
{
    bool is_record;
    union
    {
        xen_pool handle;
        xen_pool_record *record;
    } u;
} xen_pool_record_opt;

/**
 * Allocate a xen_pool_record_opt.
 */
extern xen_pool_record_opt *
xen_pool_record_opt_alloc(void);

/**
 * Free the given xen_pool_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_pool_record_opt_free(xen_pool_record_opt *record_opt);


typedef struct xen_pool_record_set
{
    size_t size;
    xen_pool_record *contents[];
} xen_pool_record_set;

/**
 * Allocate a xen_pool_record_set of the given size.
 */
extern xen_pool_record_set *
xen_pool_record_set_alloc(size_t size);

/**
 * Free the given xen_pool_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_pool_record_set_free(xen_pool_record_set *set);


typedef struct xen_pool_record_opt_set
{
    size_t size;
    xen_pool_record_opt *contents[];
} xen_pool_record_opt_set;

/**
 * Allocate a xen_pool_record_opt_set of the given size.
 */
extern xen_pool_record_opt_set *
xen_pool_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_pool_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_pool_record_opt_set_free(xen_pool_record_opt_set *set);


/**
 * Get a record containing the current state of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_record(xen_session *session, xen_pool_record **result, xen_pool pool);


/**
 * Get a reference to the pool instance with the specified UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_by_uuid(xen_session *session, xen_pool *result, char *uuid);


/**
 * Get the uuid field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_uuid(xen_session *session, char **result, xen_pool pool);


/**
 * Get the name_label field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_name_label(xen_session *session, char **result, xen_pool pool);


/**
 * Get the name_description field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_name_description(xen_session *session, char **result, xen_pool pool);


/**
 * Get the master field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_master(xen_session *session, xen_host *result, xen_pool pool);


/**
 * Get the default_SR field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_default_sr(xen_session *session, xen_sr *result, xen_pool pool);


/**
 * Get the suspend_image_SR field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_suspend_image_sr(xen_session *session, xen_sr *result, xen_pool pool);


/**
 * Get the crash_dump_SR field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_crash_dump_sr(xen_session *session, xen_sr *result, xen_pool pool);


/**
 * Get the other_config field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_other_config(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the ha_enabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the ha_configuration field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_configuration(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the ha_statefiles field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_statefiles(xen_session *session, struct xen_string_set **result, xen_pool pool);


/**
 * Get the ha_host_failures_to_tolerate field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_host_failures_to_tolerate(xen_session *session, int64_t *result, xen_pool pool);


/**
 * Get the ha_plan_exists_for field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_plan_exists_for(xen_session *session, int64_t *result, xen_pool pool);


/**
 * Get the ha_allow_overcommit field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_allow_overcommit(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the ha_overcommitted field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_overcommitted(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the blobs field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_blobs(xen_session *session, xen_string_blob_map **result, xen_pool pool);


/**
 * Get the tags field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_tags(xen_session *session, struct xen_string_set **result, xen_pool pool);


/**
 * Get the gui_config field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_gui_config(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the health_check_config field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_health_check_config(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the wlb_url field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_wlb_url(xen_session *session, char **result, xen_pool pool);


/**
 * Get the wlb_username field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_wlb_username(xen_session *session, char **result, xen_pool pool);


/**
 * Get the wlb_enabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_wlb_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the wlb_verify_cert field of the given pool.
 * Minimum allowed role: read-only.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_get_wlb_verify_cert(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the redo_log_enabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_redo_log_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the redo_log_vdi field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_redo_log_vdi(xen_session *session, xen_vdi *result, xen_pool pool);


/**
 * Get the vswitch_controller field of the given pool.
 * Minimum allowed role: read-only.
 * Deprecated since XenServer 7.2.
 */
extern bool
xen_pool_get_vswitch_controller(xen_session *session, char **result, xen_pool pool);


/**
 * Get the restrictions field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_restrictions(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the metadata_VDIs field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_metadata_vdis(xen_session *session, struct xen_vdi_set **result, xen_pool pool);


/**
 * Get the ha_cluster_stack field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_cluster_stack(xen_session *session, char **result, xen_pool pool);


/**
 * Get the allowed_operations field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_allowed_operations(xen_session *session, struct xen_pool_allowed_operations_set **result, xen_pool pool);


/**
 * Get the current_operations field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_current_operations(xen_session *session, xen_string_pool_allowed_operations_map **result, xen_pool pool);


/**
 * Get the guest_agent_config field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_guest_agent_config(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the cpu_info field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_cpu_info(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the policy_no_vendor_device field of the given pool.
 * Minimum allowed role: read-only.
 * Deprecated since 24.14.0.
 */
extern bool
xen_pool_get_policy_no_vendor_device(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the live_patching_disabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_live_patching_disabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the igmp_snooping_enabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_igmp_snooping_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the uefi_certificates field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_uefi_certificates(xen_session *session, char **result, xen_pool pool);


/**
 * Get the custom_uefi_certificates field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_custom_uefi_certificates(xen_session *session, char **result, xen_pool pool);


/**
 * Get the is_psr_pending field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_is_psr_pending(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the tls_verification_enabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_tls_verification_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the repositories field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_repositories(xen_session *session, struct xen_repository_set **result, xen_pool pool);


/**
 * Get the client_certificate_auth_enabled field of the given
 * pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_client_certificate_auth_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the client_certificate_auth_name field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_client_certificate_auth_name(xen_session *session, char **result, xen_pool pool);


/**
 * Get the repository_proxy_url field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_repository_proxy_url(xen_session *session, char **result, xen_pool pool);


/**
 * Get the repository_proxy_username field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_repository_proxy_username(xen_session *session, char **result, xen_pool pool);


/**
 * Get the repository_proxy_password field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_repository_proxy_password(xen_session *session, xen_secret *result, xen_pool pool);


/**
 * Get the migration_compression field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_migration_compression(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the coordinator_bias field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_coordinator_bias(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the local_auth_max_threads field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_local_auth_max_threads(xen_session *session, int64_t *result, xen_pool pool);


/**
 * Get the ext_auth_max_threads field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ext_auth_max_threads(xen_session *session, int64_t *result, xen_pool pool);


/**
 * Get the ext_auth_cache_enabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ext_auth_cache_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the ext_auth_cache_size field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ext_auth_cache_size(xen_session *session, int64_t *result, xen_pool pool);


/**
 * Get the ext_auth_cache_expiry field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ext_auth_cache_expiry(xen_session *session, int64_t *result, xen_pool pool);


/**
 * Get the telemetry_uuid field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_telemetry_uuid(xen_session *session, xen_secret *result, xen_pool pool);


/**
 * Get the telemetry_frequency field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_telemetry_frequency(xen_session *session, enum xen_telemetry_frequency *result, xen_pool pool);


/**
 * Get the telemetry_next_collection field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_telemetry_next_collection(xen_session *session, time_t *result, xen_pool pool);


/**
 * Get the last_update_sync field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_last_update_sync(xen_session *session, time_t *result, xen_pool pool);


/**
 * Get the update_sync_frequency field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_update_sync_frequency(xen_session *session, enum xen_update_sync_frequency *result, xen_pool pool);


/**
 * Get the update_sync_day field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_update_sync_day(xen_session *session, int64_t *result, xen_pool pool);


/**
 * Get the update_sync_enabled field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_update_sync_enabled(xen_session *session, bool *result, xen_pool pool);


/**
 * Get the recommendations field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_recommendations(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the license_server field of the given pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_license_server(xen_session *session, xen_string_string_map **result, xen_pool pool);


/**
 * Get the ha_reboot_vm_on_internal_shutdown field of the given
 * pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_ha_reboot_vm_on_internal_shutdown(xen_session *session, bool *result, xen_pool pool);


/**
 * Set the name_label field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_name_label(xen_session *session, xen_pool pool, char *name_label);


/**
 * Set the name_description field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_name_description(xen_session *session, xen_pool pool, char *name_description);


/**
 * Set the default_SR field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_default_sr(xen_session *session, xen_pool pool, xen_sr default_sr);


/**
 * Set the suspend_image_SR field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_suspend_image_sr(xen_session *session, xen_pool pool, xen_sr suspend_image_sr);


/**
 * Set the crash_dump_SR field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_crash_dump_sr(xen_session *session, xen_pool pool, xen_sr crash_dump_sr);


/**
 * Set the other_config field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_other_config(xen_session *session, xen_pool pool, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_add_to_other_config(xen_session *session, xen_pool pool, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given pool.  If the key is not in that Map, then
 * do nothing.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_remove_from_other_config(xen_session *session, xen_pool pool, char *key);


/**
 * Set the ha_allow_overcommit field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_ha_allow_overcommit(xen_session *session, xen_pool pool, bool ha_allow_overcommit);


/**
 * Set the tags field of the given pool.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_pool_set_tags(xen_session *session, xen_pool pool, struct xen_string_set *tags);


/**
 * Add the given value to the tags field of the given pool.  If the
 * value is already in that Set, then do nothing.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_pool_add_tags(xen_session *session, xen_pool pool, char *value);


/**
 * Remove the given value from the tags field of the given pool.  If
 * the value is not in that Set, then do nothing.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_pool_remove_tags(xen_session *session, xen_pool pool, char *value);


/**
 * Set the gui_config field of the given pool.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_pool_set_gui_config(xen_session *session, xen_pool pool, xen_string_string_map *gui_config);


/**
 * Add the given key-value pair to the gui_config field of the given
 * pool.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_pool_add_to_gui_config(xen_session *session, xen_pool pool, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the gui_config
 * field of the given pool.  If the key is not in that Map, then do
 * nothing.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_pool_remove_from_gui_config(xen_session *session, xen_pool pool, char *key);


/**
 * Set the health_check_config field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_health_check_config(xen_session *session, xen_pool pool, xen_string_string_map *health_check_config);


/**
 * Add the given key-value pair to the health_check_config field of the
 * given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_add_to_health_check_config(xen_session *session, xen_pool pool, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * health_check_config field of the given pool.  If the key is not in that
 * Map, then do nothing.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_remove_from_health_check_config(xen_session *session, xen_pool pool, char *key);


/**
 * Set the wlb_enabled field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_wlb_enabled(xen_session *session, xen_pool pool, bool wlb_enabled);


/**
 * Set the wlb_verify_cert field of the given pool.
 * Minimum allowed role: pool-operator.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_set_wlb_verify_cert(xen_session *session, xen_pool pool, bool wlb_verify_cert);


/**
 * Set the policy_no_vendor_device field of the given pool.
 * Minimum allowed role: pool-operator.
 * Deprecated since 24.14.0.
 */
extern bool
xen_pool_set_policy_no_vendor_device(xen_session *session, xen_pool pool, bool policy_no_vendor_device);


/**
 * Set the live_patching_disabled field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_live_patching_disabled(xen_session *session, xen_pool pool, bool live_patching_disabled);


/**
 * Set the is_psr_pending field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_is_psr_pending(xen_session *session, xen_pool pool, bool is_psr_pending);


/**
 * Set the migration_compression field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_migration_compression(xen_session *session, xen_pool pool, bool migration_compression);


/**
 * Set the coordinator_bias field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_coordinator_bias(xen_session *session, xen_pool pool, bool coordinator_bias);


/**
 * Set the license_server field of the given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_license_server(xen_session *session, xen_pool pool, xen_string_string_map *license_server);


/**
 * Add the given key-value pair to the license_server field of the
 * given pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_add_to_license_server(xen_session *session, xen_pool pool, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * license_server field of the given pool.  If the key is not in that Map,
 * then do nothing.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_remove_from_license_server(xen_session *session, xen_pool pool, char *key);


/**
 * Set the ha_reboot_vm_on_internal_shutdown field of the given
 * pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_ha_reboot_vm_on_internal_shutdown(xen_session *session, xen_pool pool, bool ha_reboot_vm_on_internal_shutdown);


/**
 * Instruct host to join a new pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_join(xen_session *session, char *master_address, char *master_username, char *master_password);


/**
 * Instruct host to join a new pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_join_async(xen_session *session, xen_task *result, char *master_address, char *master_username, char *master_password);


/**
 * Instruct host to join a new pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_join_force(xen_session *session, char *master_address, char *master_username, char *master_password);


/**
 * Instruct host to join a new pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_join_force_async(xen_session *session, xen_task *result, char *master_address, char *master_username, char *master_password);


/**
 * Instruct a pool master to eject a host from the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_eject(xen_session *session, xen_host host);


/**
 * Instruct a pool master to eject a host from the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_eject_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Instruct host that's currently a slave to transition to being
 * master.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_emergency_transition_to_master(xen_session *session);


/**
 * Instruct a slave already in a pool that the master has
 * changed.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_emergency_reset_master(xen_session *session, char *master_address);


/**
 * Instruct a pool master, M, to try and contact its slaves and, if
 * slaves are in emergency mode, reset their master address to M.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_recover_slaves(xen_session *session, struct xen_host_set **result);


/**
 * Instruct a pool master, M, to try and contact its slaves and, if
 * slaves are in emergency mode, reset their master address to M.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_recover_slaves_async(xen_session *session, xen_task *result);


/**
 * Create PIFs, mapping a network to the same physical interface/VLAN
 * on each host. This call is deprecated: use Pool.create_VLAN_from_PIF
 * instead.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_create_vlan(xen_session *session, struct xen_pif_set **result, char *device, xen_network network, int64_t vlan);


/**
 * Create PIFs, mapping a network to the same physical interface/VLAN
 * on each host. This call is deprecated: use Pool.create_VLAN_from_PIF
 * instead.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_create_vlan_async(xen_session *session, xen_task *result, char *device, xen_network network, int64_t vlan);


/**
 * Reconfigure the management network interface for all Hosts in the
 * Pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_management_reconfigure(xen_session *session, xen_network network);


/**
 * Reconfigure the management network interface for all Hosts in the
 * Pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_management_reconfigure_async(xen_session *session, xen_task *result, xen_network network);


/**
 * Create a pool-wide VLAN by taking the PIF.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_create_vlan_from_pif(xen_session *session, struct xen_pif_set **result, xen_pif pif, xen_network network, int64_t vlan);


/**
 * Create a pool-wide VLAN by taking the PIF.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_create_vlan_from_pif_async(xen_session *session, xen_task *result, xen_pif pif, xen_network network, int64_t vlan);


/**
 * Turn on High Availability mode.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_ha(xen_session *session, struct xen_sr_set *heartbeat_srs, xen_string_string_map *configuration);


/**
 * Turn on High Availability mode.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_ha_async(xen_session *session, xen_task *result, struct xen_sr_set *heartbeat_srs, xen_string_string_map *configuration);


/**
 * Turn off High Availability mode.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_ha(xen_session *session);


/**
 * Turn off High Availability mode.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_ha_async(xen_session *session, xen_task *result);


/**
 * Forcibly synchronise the database now.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_sync_database(xen_session *session);


/**
 * Forcibly synchronise the database now.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_sync_database_async(xen_session *session, xen_task *result);


/**
 * Perform an orderly handover of the role of master to the referenced
 * host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_designate_new_master(xen_session *session, xen_host host);


/**
 * Perform an orderly handover of the role of master to the referenced
 * host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_designate_new_master_async(xen_session *session, xen_task *result, xen_host host);


/**
 * When this call returns the VM restart logic will not run for the
 * requested number of seconds. If the argument is zero then the restart
 * thread is immediately unblocked.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_ha_prevent_restarts_for(xen_session *session, int64_t seconds);


/**
 * Returns true if a VM failover plan exists for up to 'n' host
 * failures.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_ha_failover_plan_exists(xen_session *session, bool *result, int64_t n);


/**
 * Returns the maximum number of host failures we could tolerate before
 * we would be unable to restart configured VMs.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_ha_compute_max_host_failures_to_tolerate(xen_session *session, int64_t *result);


/**
 * Returns the maximum number of host failures we could tolerate before
 * we would be unable to restart the provided VMs.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_ha_compute_hypothetical_max_host_failures_to_tolerate(xen_session *session, int64_t *result, xen_vm_string_map *configuration);


/**
 * Return a VM failover plan assuming a given subset of hosts
 * fail.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_ha_compute_vm_failover_plan(xen_session *session, xen_vm_string_string_map_map **result, struct xen_host_set *failed_hosts, struct xen_vm_set *failed_vms);


/**
 * Set the maximum number of host failures to consider in the HA VM
 * restart planner.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_ha_host_failures_to_tolerate(xen_session *session, xen_pool self, int64_t value);


/**
 * Set the maximum number of host failures to consider in the HA VM
 * restart planner.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_ha_host_failures_to_tolerate_async(xen_session *session, xen_task *result, xen_pool self, int64_t value);


/**
 * Create a placeholder for a named binary blob of data that is
 * associated with this pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_create_new_blob(xen_session *session, xen_blob *result, xen_pool pool, char *name, char *mime_type, bool pubblic);


/**
 * Create a placeholder for a named binary blob of data that is
 * associated with this pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_create_new_blob_async(xen_session *session, xen_task *result, xen_pool pool, char *name, char *mime_type, bool pubblic);


/**
 * This call enables external authentication on all the hosts of the
 * pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_enable_external_auth(xen_session *session, xen_pool pool, xen_string_string_map *config, char *service_name, char *auth_type);


/**
 * This call disables external authentication on all the hosts of the
 * pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_disable_external_auth(xen_session *session, xen_pool pool, xen_string_string_map *config);


/**
 * This call asynchronously detects if the external authentication
 * configuration in any slave is different from that in the master and raises
 * appropriate alerts.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_detect_nonhomogeneous_external_auth(xen_session *session, xen_pool pool);


/**
 * Initializes workload balancing monitoring on this pool with the
 * specified wlb server.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_initialize_wlb(xen_session *session, char *wlb_url, char *wlb_username, char *wlb_password, char *xenserver_username, char *xenserver_password);


/**
 * Initializes workload balancing monitoring on this pool with the
 * specified wlb server.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_initialize_wlb_async(xen_session *session, xen_task *result, char *wlb_url, char *wlb_username, char *wlb_password, char *xenserver_username, char *xenserver_password);


/**
 * Permanently deconfigures workload balancing monitoring on this
 * pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_deconfigure_wlb(xen_session *session);


/**
 * Permanently deconfigures workload balancing monitoring on this
 * pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_deconfigure_wlb_async(xen_session *session, xen_task *result);


/**
 * Sets the pool optimization criteria for the workload balancing
 * server.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_send_wlb_configuration(xen_session *session, xen_string_string_map *config);


/**
 * Sets the pool optimization criteria for the workload balancing
 * server.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_send_wlb_configuration_async(xen_session *session, xen_task *result, xen_string_string_map *config);


/**
 * Retrieves the pool optimization criteria from the workload balancing
 * server.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_retrieve_wlb_configuration(xen_session *session, xen_string_string_map **result);


/**
 * Retrieves the pool optimization criteria from the workload balancing
 * server.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_retrieve_wlb_configuration_async(xen_session *session, xen_task *result);


/**
 * Retrieves vm migrate recommendations for the pool from the workload
 * balancing server.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_retrieve_wlb_recommendations(xen_session *session, xen_vm_string_set_map **result);


/**
 * Retrieves vm migrate recommendations for the pool from the workload
 * balancing server.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_retrieve_wlb_recommendations_async(xen_session *session, xen_task *result);


/**
 * Send the given body to the given host and port, using HTTPS, and
 * print the response.  This is used for debugging the SSL layer.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_send_test_post(xen_session *session, char **result, char *host, int64_t port, char *body);


/**
 * Send the given body to the given host and port, using HTTPS, and
 * print the response.  This is used for debugging the SSL layer.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_send_test_post_async(xen_session *session, xen_task *result, char *host, int64_t port, char *body);


/**
 * Install a TLS CA certificate, pool-wide.
 * Minimum allowed role: pool-operator.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_certificate_install(xen_session *session, char *name, char *cert);


/**
 * Install a TLS CA certificate, pool-wide.
 * Minimum allowed role: pool-operator.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_certificate_install_async(xen_session *session, xen_task *result, char *name, char *cert);


/**
 * Remove a pool-wide TLS CA certificate.
 * Minimum allowed role: pool-operator.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_certificate_uninstall(xen_session *session, char *name);


/**
 * Remove a pool-wide TLS CA certificate.
 * Minimum allowed role: pool-operator.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_certificate_uninstall_async(xen_session *session, xen_task *result, char *name);


/**
 * List the names of all installed TLS CA certificates.
 * Minimum allowed role: pool-operator.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_certificate_list(xen_session *session, struct xen_string_set **result);


/**
 * List the names of all installed TLS CA certificates.
 * Minimum allowed role: pool-operator.
 * Deprecated since 1.290.0.
 */
extern bool
xen_pool_certificate_list_async(xen_session *session, xen_task *result);


/**
 * Install a TLS CA certificate, pool-wide.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_install_ca_certificate(xen_session *session, char *name, char *cert);


/**
 * Install a TLS CA certificate, pool-wide.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_install_ca_certificate_async(xen_session *session, xen_task *result, char *name, char *cert);


/**
 * Remove a pool-wide TLS CA certificate.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_uninstall_ca_certificate(xen_session *session, char *name, bool force);


/**
 * Remove a pool-wide TLS CA certificate.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_uninstall_ca_certificate_async(xen_session *session, xen_task *result, char *name, bool force);


/**
 * Install a TLS CA-issued Certificate Revocation List,
 * pool-wide.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_crl_install(xen_session *session, char *name, char *cert);


/**
 * Install a TLS CA-issued Certificate Revocation List,
 * pool-wide.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_crl_install_async(xen_session *session, xen_task *result, char *name, char *cert);


/**
 * Remove a pool-wide TLS CA-issued Certificate Revocation List.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_crl_uninstall(xen_session *session, char *name);


/**
 * Remove a pool-wide TLS CA-issued Certificate Revocation List.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_crl_uninstall_async(xen_session *session, xen_task *result, char *name);


/**
 * List the names of all installed TLS CA-issued Certificate Revocation
 * Lists.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_crl_list(xen_session *session, struct xen_string_set **result);


/**
 * List the names of all installed TLS CA-issued Certificate Revocation
 * Lists.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_crl_list_async(xen_session *session, xen_task *result);


/**
 * Copy the TLS CA certificates and CRLs of the master to all
 * slaves.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_certificate_sync(xen_session *session);


/**
 * Copy the TLS CA certificates and CRLs of the master to all
 * slaves.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_certificate_sync_async(xen_session *session, xen_task *result);


/**
 * Enable TLS server certificate verification.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_enable_tls_verification(xen_session *session);


/**
 * Enable the redo log on the given SR and start using it, unless HA is
 * enabled.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_redo_log(xen_session *session, xen_sr sr);


/**
 * Enable the redo log on the given SR and start using it, unless HA is
 * enabled.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_redo_log_async(xen_session *session, xen_task *result, xen_sr sr);


/**
 * Disable the redo log if in use, unless HA is enabled.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_redo_log(xen_session *session);


/**
 * Disable the redo log if in use, unless HA is enabled.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_redo_log_async(xen_session *session, xen_task *result);


/**
 * Set the IP address of the vswitch controller.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.2.
 */
extern bool
xen_pool_set_vswitch_controller(xen_session *session, char *address);


/**
 * Set the IP address of the vswitch controller.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.2.
 */
extern bool
xen_pool_set_vswitch_controller_async(xen_session *session, xen_task *result, char *address);


/**
 * This call tests if a location is valid.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_test_archive_target(xen_session *session, char **result, xen_pool self, xen_string_string_map *config);


/**
 * This call attempts to enable pool-wide local storage caching.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_local_storage_caching(xen_session *session, xen_pool self);


/**
 * This call attempts to enable pool-wide local storage caching.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_local_storage_caching_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * This call disables pool-wide local storage caching.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_local_storage_caching(xen_session *session, xen_pool self);


/**
 * This call disables pool-wide local storage caching.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_local_storage_caching_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * This call returns the license state for the pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_license_state(xen_session *session, xen_string_string_map **result, xen_pool self);


/**
 * This call returns the license state for the pool.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_license_state_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Apply an edition to all hosts in the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_apply_edition(xen_session *session, xen_pool self, char *edition);


/**
 * Apply an edition to all hosts in the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_apply_edition_async(xen_session *session, xen_task *result, xen_pool self, char *edition);


/**
 * Sets ssl_legacy true on each host, pool-master last. See
 * Host.ssl_legacy and Host.set_ssl_legacy.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.0.
 */
extern bool
xen_pool_enable_ssl_legacy(xen_session *session, xen_pool self);


/**
 * Sets ssl_legacy true on each host, pool-master last. See
 * Host.ssl_legacy and Host.set_ssl_legacy.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.0.
 */
extern bool
xen_pool_enable_ssl_legacy_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Sets ssl_legacy false on each host, pool-master last. See
 * Host.ssl_legacy and Host.set_ssl_legacy.
 * Minimum allowed role: pool-operator.
 * Deprecated since Citrix Hypervisor 8.2.
 */
extern bool
xen_pool_disable_ssl_legacy(xen_session *session, xen_pool self);


/**
 * Sets ssl_legacy false on each host, pool-master last. See
 * Host.ssl_legacy and Host.set_ssl_legacy.
 * Minimum allowed role: pool-operator.
 * Deprecated since Citrix Hypervisor 8.2.
 */
extern bool
xen_pool_disable_ssl_legacy_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Enable or disable IGMP Snooping on the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_igmp_snooping_enabled(xen_session *session, xen_pool self, bool value);


/**
 * Enable or disable IGMP Snooping on the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_igmp_snooping_enabled_async(xen_session *session, xen_task *result, xen_pool self, bool value);


/**
 * Return true if the extension is available on the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_has_extension(xen_session *session, bool *result, xen_pool self, char *name);


/**
 * Return true if the extension is available on the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_has_extension_async(xen_session *session, xen_task *result, xen_pool self, char *name);


/**
 * Add a key-value pair to the pool-wide guest agent
 * configuration.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_add_to_guest_agent_config(xen_session *session, xen_pool self, char *key, char *value);


/**
 * Add a key-value pair to the pool-wide guest agent
 * configuration.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_add_to_guest_agent_config_async(xen_session *session, xen_task *result, xen_pool self, char *key, char *value);


/**
 * Remove a key-value pair from the pool-wide guest agent
 * configuration.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_remove_from_guest_agent_config(xen_session *session, xen_pool self, char *key);


/**
 * Remove a key-value pair from the pool-wide guest agent
 * configuration.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_remove_from_guest_agent_config_async(xen_session *session, xen_task *result, xen_pool self, char *key);


/**
 * .
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_rotate_secret(xen_session *session);


/**
 * .
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_rotate_secret_async(xen_session *session, xen_task *result);


/**
 * Set enabled set of repositories.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_repositories(xen_session *session, xen_pool self, struct xen_repository_set *value);


/**
 * Set enabled set of repositories.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_repositories_async(xen_session *session, xen_task *result, xen_pool self, struct xen_repository_set *value);


/**
 * Add a repository to the enabled set.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_add_repository(xen_session *session, xen_pool self, xen_repository value);


/**
 * Add a repository to the enabled set.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_add_repository_async(xen_session *session, xen_task *result, xen_pool self, xen_repository value);


/**
 * Remove a repository from the enabled set.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_remove_repository(xen_session *session, xen_pool self, xen_repository value);


/**
 * Remove a repository from the enabled set.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_remove_repository_async(xen_session *session, xen_task *result, xen_pool self, xen_repository value);


/**
 * Sync with the enabled repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_sync_updates(xen_session *session, char **result, xen_pool self, bool force, char *token, char *token_id, char *username, char *password);


/**
 * Sync with the enabled repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_sync_updates_async(xen_session *session, xen_task *result, xen_pool self, bool force, char *token, char *token_id, char *username, char *password);


/**
 * Check if the pool is ready to be updated. If not, report the
 * reasons.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_check_update_readiness(xen_session *session, struct xen_string_set_set **result, xen_pool self, bool requires_reboot);


/**
 * Check if the pool is ready to be updated. If not, report the
 * reasons.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_check_update_readiness_async(xen_session *session, xen_task *result, xen_pool self, bool requires_reboot);


/**
 * Enable client certificate authentication on the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_client_certificate_auth(xen_session *session, xen_pool self, char *name);


/**
 * Enable client certificate authentication on the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_enable_client_certificate_auth_async(xen_session *session, xen_task *result, xen_pool self, char *name);


/**
 * Disable client certificate authentication on the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_client_certificate_auth(xen_session *session, xen_pool self);


/**
 * Disable client certificate authentication on the pool.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_client_certificate_auth_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Configure proxy for RPM package repositories.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_configure_repository_proxy(xen_session *session, xen_pool self, char *url, char *username, char *password);


/**
 * Configure proxy for RPM package repositories.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_configure_repository_proxy_async(xen_session *session, xen_task *result, xen_pool self, char *url, char *username, char *password);


/**
 * Disable the proxy for RPM package repositories.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_repository_proxy(xen_session *session, xen_pool self);


/**
 * Disable the proxy for RPM package repositories.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_disable_repository_proxy_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Set the UEFI certificates for a pool and all its hosts. Deprecated:
 * use set_custom_uefi_certificates instead.
 * Minimum allowed role: pool-admin.
 * Deprecated since 24.0.0.
 */
extern bool
xen_pool_set_uefi_certificates(xen_session *session, xen_pool self, char *value);


/**
 * Set the UEFI certificates for a pool and all its hosts. Deprecated:
 * use set_custom_uefi_certificates instead.
 * Minimum allowed role: pool-admin.
 * Deprecated since 24.0.0.
 */
extern bool
xen_pool_set_uefi_certificates_async(xen_session *session, xen_task *result, xen_pool self, char *value);


/**
 * Set custom UEFI certificates for a pool and all its hosts. Need
 * `allow-custom-uefi-certs` set to true in conf. If empty: default back to
 * Pool.uefi_certificates.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_custom_uefi_certificates(xen_session *session, xen_pool self, char *value);


/**
 * Set custom UEFI certificates for a pool and all its hosts. Need
 * `allow-custom-uefi-certs` set to true in conf. If empty: default back to
 * Pool.uefi_certificates.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_custom_uefi_certificates_async(xen_session *session, xen_task *result, xen_pool self, char *value);


/**
 * updates all the host firewalls in the pool to open or close port 80
 * depending on the value.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_https_only(xen_session *session, xen_pool self, bool value);


/**
 * updates all the host firewalls in the pool to open or close port 80
 * depending on the value.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_https_only_async(xen_session *session, xen_task *result, xen_pool self, bool value);


/**
 * Set the timestamp for the next telemetry data collection.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_telemetry_next_collection(xen_session *session, xen_pool self, time_t value);


/**
 * Set the timestamp for the next telemetry data collection.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_telemetry_next_collection_async(xen_session *session, xen_task *result, xen_pool self, time_t value);


/**
 * Assign a new UUID to telemetry data.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_reset_telemetry_uuid(xen_session *session, xen_pool self);


/**
 * Assign a new UUID to telemetry data.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_reset_telemetry_uuid_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Configure periodic update synchronization to synchronize updates
 * from a remote CDN.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_configure_update_sync(xen_session *session, xen_pool self, enum xen_update_sync_frequency update_sync_frequency, int64_t update_sync_day);


/**
 * Configure periodic update synchronization to synchronize updates
 * from a remote CDN.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_configure_update_sync_async(xen_session *session, xen_task *result, xen_pool self, enum xen_update_sync_frequency update_sync_frequency, int64_t update_sync_day);


/**
 * enable or disable periodic update synchronization depending on the
 * value.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_update_sync_enabled(xen_session *session, xen_pool self, bool value);


/**
 * enable or disable periodic update synchronization depending on the
 * value.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_update_sync_enabled_async(xen_session *session, xen_task *result, xen_pool self, bool value);


/**
 * .
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_local_auth_max_threads(xen_session *session, xen_pool self, int64_t value);


/**
 * .
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_set_ext_auth_max_threads(xen_session *session, xen_pool self, int64_t value);


/**
 * .
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_pool_get_guest_secureboot_readiness(xen_session *session, enum xen_pool_guest_secureboot_readiness *result, xen_pool self);


/**
 * Enable SSH access on all hosts in the pool. It's a helper which
 * calls host.enable_ssh for all the hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_enable_ssh(xen_session *session, xen_pool self);


/**
 * Enable SSH access on all hosts in the pool. It's a helper which
 * calls host.enable_ssh for all the hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_enable_ssh_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Disable SSH access on all hosts in the pool. It's a helper which
 * calls host.disable_ssh for all the hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_disable_ssh(xen_session *session, xen_pool self);


/**
 * Disable SSH access on all hosts in the pool. It's a helper which
 * calls host.disable_ssh for all the hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_disable_ssh_async(xen_session *session, xen_task *result, xen_pool self);


/**
 * Set the SSH enabled timeout for all hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_ssh_enabled_timeout(xen_session *session, xen_pool self, int64_t value);


/**
 * Set the SSH enabled timeout for all hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_ssh_enabled_timeout_async(xen_session *session, xen_task *result, xen_pool self, int64_t value);


/**
 * Set the console idle timeout for all hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_console_idle_timeout(xen_session *session, xen_pool self, int64_t value);


/**
 * Set the console idle timeout for all hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_console_idle_timeout_async(xen_session *session, xen_task *result, xen_pool self, int64_t value);


/**
 * Set the SSH auto mode for all hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_ssh_auto_mode(xen_session *session, xen_pool self, bool value);


/**
 * Set the SSH auto mode for all hosts in the pool.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_pool_set_ssh_auto_mode_async(xen_session *session, xen_task *result, xen_pool self, bool value);


/**
 * Return a list of all the pools known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_all(xen_session *session, struct xen_pool_set **result);


/**
 * Return a map of pool references to pool records for all pools known
 * to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_pool_get_all_records(xen_session *session, xen_pool_xen_pool_record_map **result);


#endif
