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


#ifndef XEN_HOST_H
#define XEN_HOST_H

#include <xen/api/xen_blob_decl.h>
#include <xen/api/xen_certificate.h>
#include <xen/api/xen_certificate_decl.h>
#include <xen/api/xen_common.h>
#include <xen/api/xen_feature.h>
#include <xen/api/xen_feature_decl.h>
#include <xen/api/xen_host_allowed_operations.h>
#include <xen/api/xen_host_cpu.h>
#include <xen/api/xen_host_cpu_decl.h>
#include <xen/api/xen_host_crashdump.h>
#include <xen/api/xen_host_crashdump_decl.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_host_display.h>
#include <xen/api/xen_host_metrics.h>
#include <xen/api/xen_host_metrics_decl.h>
#include <xen/api/xen_host_numa_affinity_policy.h>
#include <xen/api/xen_host_patch.h>
#include <xen/api/xen_host_patch_decl.h>
#include <xen/api/xen_host_sched_gran.h>
#include <xen/api/xen_host_xen_host_record_map.h>
#include <xen/api/xen_int_set.h>
#include <xen/api/xen_latest_synced_updates_applied_state.h>
#include <xen/api/xen_network_decl.h>
#include <xen/api/xen_pbd.h>
#include <xen/api/xen_pbd_decl.h>
#include <xen/api/xen_pci.h>
#include <xen/api/xen_pci_decl.h>
#include <xen/api/xen_pgpu.h>
#include <xen/api/xen_pgpu_decl.h>
#include <xen/api/xen_pif.h>
#include <xen/api/xen_pif_decl.h>
#include <xen/api/xen_pool_update.h>
#include <xen/api/xen_pool_update_decl.h>
#include <xen/api/xen_pusb.h>
#include <xen/api/xen_pusb_decl.h>
#include <xen/api/xen_sr.h>
#include <xen/api/xen_sr_decl.h>
#include <xen/api/xen_string_blob_map.h>
#include <xen/api/xen_string_host_allowed_operations_map.h>
#include <xen/api/xen_string_set.h>
#include <xen/api/xen_string_set_set.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_update_guidances.h>
#include <xen/api/xen_vm.h>
#include <xen/api/xen_vm_decl.h>
#include <xen/api/xen_vm_string_set_map.h>


/*
 * The host class.
 * 
 * A physical host.
 */


/**
 * Free the given xen_host.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_host_free(xen_host host);


typedef struct xen_host_set
{
    size_t size;
    xen_host *contents[];
} xen_host_set;

/**
 * Allocate a xen_host_set of the given size.
 */
extern xen_host_set *
xen_host_set_alloc(size_t size);

/**
 * Free the given xen_host_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_host_set_free(xen_host_set *set);


typedef struct xen_host_record
{
    xen_host handle;
    char *uuid;
    char *name_label;
    char *name_description;
    int64_t memory_overhead;
    struct xen_host_allowed_operations_set *allowed_operations;
    xen_string_host_allowed_operations_map *current_operations;
    int64_t api_version_major;
    int64_t api_version_minor;
    char *api_version_vendor;
    xen_string_string_map *api_version_vendor_implementation;
    bool enabled;
    xen_string_string_map *software_version;
    xen_string_string_map *other_config;
    struct xen_string_set *capabilities;
    xen_string_string_map *cpu_configuration;
    char *sched_policy;
    struct xen_string_set *supported_bootloaders;
    struct xen_vm_record_opt_set *resident_vms;
    xen_string_string_map *logging;
    struct xen_pif_record_opt_set *pifs;
    struct xen_sr_record_opt *suspend_image_sr;
    struct xen_sr_record_opt *crash_dump_sr;
    struct xen_host_crashdump_record_opt_set *crashdumps;
    struct xen_host_patch_record_opt_set *patches;
    struct xen_pool_update_record_opt_set *updates;
    struct xen_pbd_record_opt_set *pbds;
    struct xen_host_cpu_record_opt_set *host_cpus;
    xen_string_string_map *cpu_info;
    char *hostname;
    char *address;
    struct xen_host_metrics_record_opt *metrics;
    xen_string_string_map *license_params;
    struct xen_string_set *ha_statefiles;
    struct xen_string_set *ha_network_peers;
    xen_string_blob_map *blobs;
    struct xen_string_set *tags;
    char *external_auth_type;
    char *external_auth_service_name;
    xen_string_string_map *external_auth_configuration;
    char *edition;
    xen_string_string_map *license_server;
    xen_string_string_map *bios_strings;
    char *power_on_mode;
    xen_string_string_map *power_on_config;
    struct xen_sr_record_opt *local_cache_sr;
    xen_string_string_map *chipset_info;
    struct xen_pci_record_opt_set *pcis;
    struct xen_pgpu_record_opt_set *pgpus;
    struct xen_pusb_record_opt_set *pusbs;
    bool ssl_legacy;
    xen_string_string_map *guest_vcpus_params;
    enum xen_host_display display;
    struct xen_int_set *virtual_hardware_platform_versions;
    struct xen_vm_record_opt *control_domain;
    struct xen_pool_update_record_opt_set *updates_requiring_reboot;
    struct xen_feature_record_opt_set *features;
    char *iscsi_iqn;
    bool multipathing;
    char *uefi_certificates;
    struct xen_certificate_record_opt_set *certificates;
    struct xen_string_set *editions;
    struct xen_update_guidances_set *pending_guidances;
    bool tls_verification_enabled;
    time_t last_software_update;
    bool https_only;
    enum xen_latest_synced_updates_applied_state latest_synced_updates_applied;
    enum xen_host_numa_affinity_policy numa_affinity_policy;
    struct xen_update_guidances_set *pending_guidances_recommended;
    struct xen_update_guidances_set *pending_guidances_full;
    char *last_update_hash;
    bool ssh_enabled;
    int64_t ssh_enabled_timeout;
    time_t ssh_expiry;
    int64_t console_idle_timeout;
    bool ssh_auto_mode;
} xen_host_record;

/**
 * Allocate a xen_host_record.
 */
extern xen_host_record *
xen_host_record_alloc(void);

/**
 * Free the given xen_host_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_host_record_free(xen_host_record *record);


typedef struct xen_host_record_opt
{
    bool is_record;
    union
    {
        xen_host handle;
        xen_host_record *record;
    } u;
} xen_host_record_opt;

/**
 * Allocate a xen_host_record_opt.
 */
extern xen_host_record_opt *
xen_host_record_opt_alloc(void);

/**
 * Free the given xen_host_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_host_record_opt_free(xen_host_record_opt *record_opt);


typedef struct xen_host_record_set
{
    size_t size;
    xen_host_record *contents[];
} xen_host_record_set;

/**
 * Allocate a xen_host_record_set of the given size.
 */
extern xen_host_record_set *
xen_host_record_set_alloc(size_t size);

/**
 * Free the given xen_host_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_host_record_set_free(xen_host_record_set *set);


typedef struct xen_host_record_opt_set
{
    size_t size;
    xen_host_record_opt *contents[];
} xen_host_record_opt_set;

/**
 * Allocate a xen_host_record_opt_set of the given size.
 */
extern xen_host_record_opt_set *
xen_host_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_host_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_host_record_opt_set_free(xen_host_record_opt_set *set);


/**
 * Get a record containing the current state of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_record(xen_session *session, xen_host_record **result, xen_host host);


/**
 * Get a reference to the host instance with the specified UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_by_uuid(xen_session *session, xen_host *result, char *uuid);


/**
 * Get all the host instances with the given label.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_by_name_label(xen_session *session, struct xen_host_set **result, char *label);


/**
 * Get the uuid field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_uuid(xen_session *session, char **result, xen_host host);


/**
 * Get the name/label field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_name_label(xen_session *session, char **result, xen_host host);


/**
 * Get the name/description field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_name_description(xen_session *session, char **result, xen_host host);


/**
 * Get the memory/overhead field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_memory_overhead(xen_session *session, int64_t *result, xen_host host);


/**
 * Get the allowed_operations field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_allowed_operations(xen_session *session, struct xen_host_allowed_operations_set **result, xen_host host);


/**
 * Get the current_operations field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_current_operations(xen_session *session, xen_string_host_allowed_operations_map **result, xen_host host);


/**
 * Get the API_version/major field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_api_version_major(xen_session *session, int64_t *result, xen_host host);


/**
 * Get the API_version/minor field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_api_version_minor(xen_session *session, int64_t *result, xen_host host);


/**
 * Get the API_version/vendor field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_api_version_vendor(xen_session *session, char **result, xen_host host);


/**
 * Get the API_version/vendor_implementation field of the given
 * host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_api_version_vendor_implementation(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the enabled field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_enabled(xen_session *session, bool *result, xen_host host);


/**
 * Get the software_version field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_software_version(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the other_config field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_other_config(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the capabilities field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_capabilities(xen_session *session, struct xen_string_set **result, xen_host host);


/**
 * Get the cpu_configuration field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_cpu_configuration(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the sched_policy field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_sched_policy(xen_session *session, char **result, xen_host host);


/**
 * Get the supported_bootloaders field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_supported_bootloaders(xen_session *session, struct xen_string_set **result, xen_host host);


/**
 * Get the resident_VMs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_resident_vms(xen_session *session, struct xen_vm_set **result, xen_host host);


/**
 * Get the logging field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_logging(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the PIFs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pifs(xen_session *session, struct xen_pif_set **result, xen_host host);


/**
 * Get the suspend_image_sr field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_suspend_image_sr(xen_session *session, xen_sr *result, xen_host host);


/**
 * Get the crash_dump_sr field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_crash_dump_sr(xen_session *session, xen_sr *result, xen_host host);


/**
 * Get the crashdumps field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_crashdumps(xen_session *session, struct xen_host_crashdump_set **result, xen_host host);


/**
 * Get the patches field of the given host.
 * Minimum allowed role: read-only.
 * Deprecated since XenServer 7.1.
 */
extern bool
xen_host_get_patches(xen_session *session, struct xen_host_patch_set **result, xen_host host);


/**
 * Get the updates field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_updates(xen_session *session, struct xen_pool_update_set **result, xen_host host);


/**
 * Get the PBDs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pbds(xen_session *session, struct xen_pbd_set **result, xen_host host);


/**
 * Get the host_CPUs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_host_cpus(xen_session *session, struct xen_host_cpu_set **result, xen_host host);


/**
 * Get the cpu_info field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_cpu_info(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the hostname field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_hostname(xen_session *session, char **result, xen_host host);


/**
 * Get the address field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_address(xen_session *session, char **result, xen_host host);


/**
 * Get the metrics field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_metrics(xen_session *session, xen_host_metrics *result, xen_host host);


/**
 * Get the license_params field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_license_params(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the ha_statefiles field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_ha_statefiles(xen_session *session, struct xen_string_set **result, xen_host host);


/**
 * Get the ha_network_peers field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_ha_network_peers(xen_session *session, struct xen_string_set **result, xen_host host);


/**
 * Get the blobs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_blobs(xen_session *session, xen_string_blob_map **result, xen_host host);


/**
 * Get the tags field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_tags(xen_session *session, struct xen_string_set **result, xen_host host);


/**
 * Get the external_auth_type field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_external_auth_type(xen_session *session, char **result, xen_host host);


/**
 * Get the external_auth_service_name field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_external_auth_service_name(xen_session *session, char **result, xen_host host);


/**
 * Get the external_auth_configuration field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_external_auth_configuration(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the edition field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_edition(xen_session *session, char **result, xen_host host);


/**
 * Get the license_server field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_license_server(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the bios_strings field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_bios_strings(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the power_on_mode field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_power_on_mode(xen_session *session, char **result, xen_host host);


/**
 * Get the power_on_config field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_power_on_config(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the local_cache_sr field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_local_cache_sr(xen_session *session, xen_sr *result, xen_host host);


/**
 * Get the chipset_info field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_chipset_info(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the PCIs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pcis(xen_session *session, struct xen_pci_set **result, xen_host host);


/**
 * Get the PGPUs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pgpus(xen_session *session, struct xen_pgpu_set **result, xen_host host);


/**
 * Get the PUSBs field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pusbs(xen_session *session, struct xen_pusb_set **result, xen_host host);


/**
 * Get the ssl_legacy field of the given host.
 * Minimum allowed role: read-only.
 * Deprecated since Citrix Hypervisor 8.2.
 */
extern bool
xen_host_get_ssl_legacy(xen_session *session, bool *result, xen_host host);


/**
 * Get the guest_VCPUs_params field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_guest_vcpus_params(xen_session *session, xen_string_string_map **result, xen_host host);


/**
 * Get the display field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_display(xen_session *session, enum xen_host_display *result, xen_host host);


/**
 * Get the virtual_hardware_platform_versions field of the given
 * host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_virtual_hardware_platform_versions(xen_session *session, struct xen_int_set **result, xen_host host);


/**
 * Get the control_domain field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_control_domain(xen_session *session, xen_vm *result, xen_host host);


/**
 * Get the updates_requiring_reboot field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_updates_requiring_reboot(xen_session *session, struct xen_pool_update_set **result, xen_host host);


/**
 * Get the features field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_features(xen_session *session, struct xen_feature_set **result, xen_host host);


/**
 * Get the iscsi_iqn field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_iscsi_iqn(xen_session *session, char **result, xen_host host);


/**
 * Get the multipathing field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_multipathing(xen_session *session, bool *result, xen_host host);


/**
 * Get the uefi_certificates field of the given host.
 * Minimum allowed role: read-only.
 * Deprecated since 22.16.0.
 */
extern bool
xen_host_get_uefi_certificates(xen_session *session, char **result, xen_host host);


/**
 * Get the certificates field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_certificates(xen_session *session, struct xen_certificate_set **result, xen_host host);


/**
 * Get the editions field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_editions(xen_session *session, struct xen_string_set **result, xen_host host);


/**
 * Get the pending_guidances field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pending_guidances(xen_session *session, struct xen_update_guidances_set **result, xen_host host);


/**
 * Get the tls_verification_enabled field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_tls_verification_enabled(xen_session *session, bool *result, xen_host host);


/**
 * Get the last_software_update field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_last_software_update(xen_session *session, time_t *result, xen_host host);


/**
 * Get the https_only field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_https_only(xen_session *session, bool *result, xen_host host);


/**
 * Get the latest_synced_updates_applied field of the given
 * host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_latest_synced_updates_applied(xen_session *session, enum xen_latest_synced_updates_applied_state *result, xen_host host);


/**
 * Get the numa_affinity_policy field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_numa_affinity_policy(xen_session *session, enum xen_host_numa_affinity_policy *result, xen_host host);


/**
 * Get the pending_guidances_recommended field of the given
 * host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pending_guidances_recommended(xen_session *session, struct xen_update_guidances_set **result, xen_host host);


/**
 * Get the pending_guidances_full field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_pending_guidances_full(xen_session *session, struct xen_update_guidances_set **result, xen_host host);


/**
 * Get the last_update_hash field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_last_update_hash(xen_session *session, char **result, xen_host host);


/**
 * Get the ssh_enabled field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_ssh_enabled(xen_session *session, bool *result, xen_host host);


/**
 * Get the ssh_enabled_timeout field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_ssh_enabled_timeout(xen_session *session, int64_t *result, xen_host host);


/**
 * Get the ssh_expiry field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_ssh_expiry(xen_session *session, time_t *result, xen_host host);


/**
 * Get the console_idle_timeout field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_console_idle_timeout(xen_session *session, int64_t *result, xen_host host);


/**
 * Get the ssh_auto_mode field of the given host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_ssh_auto_mode(xen_session *session, bool *result, xen_host host);


/**
 * Set the name/label field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_name_label(xen_session *session, xen_host host, char *label);


/**
 * Set the name/description field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_name_description(xen_session *session, xen_host host, char *description);


/**
 * Set the other_config field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_other_config(xen_session *session, xen_host host, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_add_to_other_config(xen_session *session, xen_host host, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given host.  If the key is not in that Map, then
 * do nothing.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_remove_from_other_config(xen_session *session, xen_host host, char *key);


/**
 * Set the logging field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_logging(xen_session *session, xen_host host, xen_string_string_map *logging);


/**
 * Add the given key-value pair to the logging field of the given
 * host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_add_to_logging(xen_session *session, xen_host host, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the logging
 * field of the given host.  If the key is not in that Map, then do
 * nothing.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_remove_from_logging(xen_session *session, xen_host host, char *key);


/**
 * Set the suspend_image_sr field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_suspend_image_sr(xen_session *session, xen_host host, xen_sr suspend_image_sr);


/**
 * Set the crash_dump_sr field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_crash_dump_sr(xen_session *session, xen_host host, xen_sr crash_dump_sr);


/**
 * Set the hostname field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_hostname(xen_session *session, xen_host host, char *hostname);


/**
 * Set the address field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_address(xen_session *session, xen_host host, char *address);


/**
 * Set the tags field of the given host.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_host_set_tags(xen_session *session, xen_host host, struct xen_string_set *tags);


/**
 * Add the given value to the tags field of the given host.  If the
 * value is already in that Set, then do nothing.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_host_add_tags(xen_session *session, xen_host host, char *value);


/**
 * Remove the given value from the tags field of the given host.  If
 * the value is not in that Set, then do nothing.
 * Minimum allowed role: vm-operator.
 */
extern bool
xen_host_remove_tags(xen_session *session, xen_host host, char *value);


/**
 * Set the license_server field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_license_server(xen_session *session, xen_host host, xen_string_string_map *license_server);


/**
 * Add the given key-value pair to the license_server field of the
 * given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_add_to_license_server(xen_session *session, xen_host host, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * license_server field of the given host.  If the key is not in that Map,
 * then do nothing.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_remove_from_license_server(xen_session *session, xen_host host, char *key);


/**
 * Set the guest_VCPUs_params field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_guest_vcpus_params(xen_session *session, xen_host host, xen_string_string_map *guest_vcpus_params);


/**
 * Add the given key-value pair to the guest_VCPUs_params field of the
 * given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_add_to_guest_vcpus_params(xen_session *session, xen_host host, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * guest_VCPUs_params field of the given host.  If the key is not in that Map,
 * then do nothing.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_remove_from_guest_vcpus_params(xen_session *session, xen_host host, char *key);


/**
 * Set the display field of the given host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_display(xen_session *session, xen_host host, enum xen_host_display display);


/**
 * Puts the host into a state in which no new VMs can be started.
 * Currently active VMs on the host continue to execute.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_disable(xen_session *session, xen_host host);


/**
 * Puts the host into a state in which no new VMs can be started.
 * Currently active VMs on the host continue to execute.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_disable_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Puts the host into a state in which new VMs can be started.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_enable(xen_session *session, xen_host host);


/**
 * Puts the host into a state in which new VMs can be started.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_enable_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Shutdown the host. (This function can only be called if there are no
 * currently running VMs on the host and it is disabled.).
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_shutdown(xen_session *session, xen_host host);


/**
 * Shutdown the host. (This function can only be called if there are no
 * currently running VMs on the host and it is disabled.).
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_shutdown_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Reboot the host. (This function can only be called if there are no
 * currently running VMs on the host and it is disabled.).
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_reboot(xen_session *session, xen_host host);


/**
 * Reboot the host. (This function can only be called if there are no
 * currently running VMs on the host and it is disabled.).
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_reboot_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Get the host xen dmesg.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_dmesg(xen_session *session, char **result, xen_host host);


/**
 * Get the host xen dmesg.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_dmesg_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Get the host xen dmesg, and clear the buffer.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_dmesg_clear(xen_session *session, char **result, xen_host host);


/**
 * Get the host xen dmesg, and clear the buffer.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_dmesg_clear_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Get the host's log file.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_log(xen_session *session, char **result, xen_host host);


/**
 * Get the host's log file.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_log_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Inject the given string as debugging keys into Xen.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_send_debug_keys(xen_session *session, xen_host host, char *keys);


/**
 * Inject the given string as debugging keys into Xen.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_send_debug_keys_async(xen_session *session, xen_task *result, xen_host host, char *keys);


/**
 * Run xen-bugtool --yestoall and upload the output to support.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_bugreport_upload(xen_session *session, xen_host host, char *url, xen_string_string_map *options);


/**
 * Run xen-bugtool --yestoall and upload the output to support.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_bugreport_upload_async(xen_session *session, xen_task *result, xen_host host, char *url, xen_string_string_map *options);


/**
 * List all supported methods.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_list_methods(xen_session *session, struct xen_string_set **result);


/**
 * Apply a new license to a host.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 6.2.
 */
extern bool
xen_host_license_apply(xen_session *session, xen_host host, char *contents);


/**
 * Apply a new license to a host.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 6.2.
 */
extern bool
xen_host_license_apply_async(xen_session *session, xen_task *result, xen_host host, char *contents);


/**
 * Apply a new license to a host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_license_add(xen_session *session, xen_host host, char *contents);


/**
 * Apply a new license to a host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_license_add_async(xen_session *session, xen_task *result, xen_host host, char *contents);


/**
 * Remove any license file from the specified host, and switch that
 * host to the unlicensed edition.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_license_remove(xen_session *session, xen_host host);


/**
 * Remove any license file from the specified host, and switch that
 * host to the unlicensed edition.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_license_remove_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Destroy specified host record in database.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_destroy(xen_session *session, xen_host self);


/**
 * Destroy specified host record in database.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_destroy_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Attempt to power-on the host (if the capability exists).
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_power_on(xen_session *session, xen_host host);


/**
 * Attempt to power-on the host (if the capability exists).
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_power_on_async(xen_session *session, xen_task *result, xen_host host);


/**
 * This call disables HA on the local host. This should only be used
 * with extreme care.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_emergency_ha_disable(xen_session *session, bool soft);


/**
 * Start recording the specified data source.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_record_data_source(xen_session *session, xen_host host, char *data_source);


/**
 * Query the latest value of the specified data source.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_query_data_source(xen_session *session, double *result, xen_host host, char *data_source);


/**
 * Forget the recorded statistics related to the specified data
 * source.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_forget_data_source_archives(xen_session *session, xen_host host, char *data_source);


/**
 * Check this host can be evacuated.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_assert_can_evacuate(xen_session *session, xen_host host);


/**
 * Check this host can be evacuated.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_assert_can_evacuate_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Return a set of VMs which prevent the host being evacuated, with
 * per-VM error codes.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_vms_which_prevent_evacuation(xen_session *session, xen_vm_string_set_map **result, xen_host self);


/**
 * Return a set of VMs which prevent the host being evacuated, with
 * per-VM error codes.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_vms_which_prevent_evacuation_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Return a set of VMs which are not co-operating with the host's
 * memory control system.
 * Minimum allowed role: read-only.
 * Deprecated since XenServer 6.1.
 */
extern bool
xen_host_get_uncooperative_resident_vms(xen_session *session, struct xen_vm_set **result, xen_host self);


/**
 * Return a set of VMs which are not co-operating with the host's
 * memory control system.
 * Minimum allowed role: read-only.
 * Deprecated since XenServer 6.1.
 */
extern bool
xen_host_get_uncooperative_resident_vms_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Migrate all VMs off of this host, where possible.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_evacuate(xen_session *session, xen_host host, xen_network network, int64_t evacuate_batch_size);


/**
 * Migrate all VMs off of this host, where possible.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_evacuate_async(xen_session *session, xen_task *result, xen_host host, xen_network network, int64_t evacuate_batch_size);


/**
 * Re-configure syslog logging.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_syslog_reconfigure(xen_session *session, xen_host host);


/**
 * Re-configure syslog logging.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_syslog_reconfigure_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Reconfigure the management network interface.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_management_reconfigure(xen_session *session, xen_pif pif);


/**
 * Reconfigure the management network interface.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_management_reconfigure_async(xen_session *session, xen_task *result, xen_pif pif);


/**
 * Reconfigure the management network interface. Should only be used if
 * Host.management_reconfigure is impossible because the network configuration
 * is broken.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_local_management_reconfigure(xen_session *session, char *interface);


/**
 * Disable the management network interface.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_management_disable(xen_session *session);


/**
 * Returns the management interface for the specified host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_get_management_interface(xen_session *session, xen_pif *result, xen_host host);


/**
 * Returns the management interface for the specified host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_get_management_interface_async(xen_session *session, xen_task *result, xen_host host);


/**
 * .
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_system_status_capabilities(xen_session *session, char **result, xen_host host);


/**
 * Restarts the agent after a 10 second pause. WARNING: this is a
 * dangerous operation. Any operations in progress will be aborted, and
 * unrecoverable data loss may occur. The caller is responsible for ensuring
 * that there are no operations in progress when this method is called.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_restart_agent(xen_session *session, xen_host host);


/**
 * Restarts the agent after a 10 second pause. WARNING: this is a
 * dangerous operation. Any operations in progress will be aborted, and
 * unrecoverable data loss may occur. The caller is responsible for ensuring
 * that there are no operations in progress when this method is called.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_restart_agent_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Shuts the agent down after a 10 second pause. WARNING: this is a
 * dangerous operation. Any operations in progress will be aborted, and
 * unrecoverable data loss may occur. The caller is responsible for ensuring
 * that there are no operations in progress when this method is called.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_shutdown_agent(xen_session *session);


/**
 * Sets the host name to the specified string.  Both the API and
 * lower-level system hostname are changed immediately.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_hostname_live(xen_session *session, xen_host host, char *hostname);


/**
 * Computes the amount of free memory on the host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_compute_free_memory(xen_session *session, int64_t *result, xen_host host);


/**
 * Computes the amount of free memory on the host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_compute_free_memory_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Computes the virtualization memory overhead of a host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_compute_memory_overhead(xen_session *session, int64_t *result, xen_host host);


/**
 * Computes the virtualization memory overhead of a host.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_compute_memory_overhead_async(xen_session *session, xen_task *result, xen_host host);


/**
 * This causes the synchronisation of the non-database data (messages,
 * RRDs and so on) stored on the master to be synchronised with the
 * host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_sync_data(xen_session *session, xen_host host);


/**
 * This causes the RRDs to be backed up to the master.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_backup_rrds(xen_session *session, xen_host host, double delay);


/**
 * Create a placeholder for a named binary blob of data that is
 * associated with this host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_create_new_blob(xen_session *session, xen_blob *result, xen_host host, char *name, char *mime_type, bool pubblic);


/**
 * Create a placeholder for a named binary blob of data that is
 * associated with this host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_create_new_blob_async(xen_session *session, xen_task *result, xen_host host, char *name, char *mime_type, bool pubblic);


/**
 * Call an API plugin on this host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_call_plugin(xen_session *session, char **result, xen_host host, char *plugin, char *fn, xen_string_string_map *args);


/**
 * Call an API plugin on this host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_call_plugin_async(xen_session *session, xen_task *result, xen_host host, char *plugin, char *fn, xen_string_string_map *args);


/**
 * Return true if the extension is available on the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_has_extension(xen_session *session, bool *result, xen_host host, char *name);


/**
 * Return true if the extension is available on the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_has_extension_async(xen_session *session, xen_task *result, xen_host host, char *name);


/**
 * Call an API extension on this host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_call_extension(xen_session *session, char **result, xen_host host, char *call);


/**
 * This call queries the host's clock for the current time.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_servertime(xen_session *session, time_t *result, xen_host host);


/**
 * This call queries the host's clock for the current time in the
 * host's local timezone.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_server_localtime(xen_session *session, time_t *result, xen_host host);


/**
 * This call enables external authentication on a host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_enable_external_auth(xen_session *session, xen_host host, xen_string_string_map *config, char *service_name, char *auth_type);


/**
 * This call disables external authentication on the local host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_disable_external_auth(xen_session *session, xen_host host, xen_string_string_map *config);


/**
 * Retrieves recommended host migrations to perform when evacuating the
 * host from the wlb server. If a VM cannot be migrated from the host the
 * reason is listed instead of a recommendation.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_retrieve_wlb_evacuate_recommendations(xen_session *session, xen_vm_string_set_map **result, xen_host self);


/**
 * Retrieves recommended host migrations to perform when evacuating the
 * host from the wlb server. If a VM cannot be migrated from the host the
 * reason is listed instead of a recommendation.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_retrieve_wlb_evacuate_recommendations_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Get the installed server public TLS certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_server_certificate(xen_session *session, char **result, xen_host host);


/**
 * Get the installed server public TLS certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_server_certificate_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Replace the internal self-signed host certficate with a new
 * one.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_refresh_server_certificate(xen_session *session, xen_host host);


/**
 * Replace the internal self-signed host certficate with a new
 * one.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_refresh_server_certificate_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Install the TLS server certificate.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_install_server_certificate(xen_session *session, xen_host host, char *certificate, char *private_key, char *certificate_chain);


/**
 * Install the TLS server certificate.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_install_server_certificate_async(xen_session *session, xen_task *result, xen_host host, char *certificate, char *private_key, char *certificate_chain);


/**
 * Delete the current TLS server certificate and replace by a new,
 * self-signed one. This should only be used with extreme care.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_emergency_reset_server_certificate(xen_session *session);


/**
 * Delete the current TLS server certificate and replace by a new,
 * self-signed one. This should only be used with extreme care.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_reset_server_certificate(xen_session *session, xen_host host);


/**
 * Delete the current TLS server certificate and replace by a new,
 * self-signed one. This should only be used with extreme care.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_reset_server_certificate_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Change to another edition, or reactivate the current edition after a
 * license has expired. This may be subject to the successful checkout of an
 * appropriate license.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_apply_edition(xen_session *session, xen_host host, char *edition, bool force);


/**
 * Refresh the list of installed Supplemental Packs.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.1.
 */
extern bool
xen_host_refresh_pack_info(xen_session *session, xen_host host);


/**
 * Refresh the list of installed Supplemental Packs.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.1.
 */
extern bool
xen_host_refresh_pack_info_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Set the power-on-mode, host, user and password.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_power_on_mode(xen_session *session, xen_host self, char *power_on_mode, xen_string_string_map *power_on_config);


/**
 * Set the power-on-mode, host, user and password.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_power_on_mode_async(xen_session *session, xen_task *result, xen_host self, char *power_on_mode, xen_string_string_map *power_on_config);


/**
 * Set the CPU features to be used after a reboot, if the given
 * features string is valid.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.0.
 */
extern bool
xen_host_set_cpu_features(xen_session *session, xen_host host, char *features);


/**
 * Remove the feature mask, such that after a reboot all features of
 * the CPU are enabled.
 * Minimum allowed role: pool-operator.
 * Deprecated since XenServer 7.0.
 */
extern bool
xen_host_reset_cpu_features(xen_session *session, xen_host host);


/**
 * Enable the use of a local SR for caching purposes.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_enable_local_storage_caching(xen_session *session, xen_host host, xen_sr sr);


/**
 * Disable the use of a local SR for caching purposes.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_disable_local_storage_caching(xen_session *session, xen_host host);


/**
 * Prepare to receive a VM, returning a token which can be passed to
 * VM.migrate.
 * Minimum allowed role: vm-power-admin.
 */
extern bool
xen_host_migrate_receive(xen_session *session, xen_string_string_map **result, xen_host host, xen_network network, xen_string_string_map *options);


/**
 * Prepare to receive a VM, returning a token which can be passed to
 * VM.migrate.
 * Minimum allowed role: vm-power-admin.
 */
extern bool
xen_host_migrate_receive_async(xen_session *session, xen_task *result, xen_host host, xen_network network, xen_string_string_map *options);


/**
 * Declare that a host is dead. This is a dangerous operation, and
 * should only be called if the administrator is absolutely sure the host is
 * definitely dead.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_declare_dead(xen_session *session, xen_host host);


/**
 * Declare that a host is dead. This is a dangerous operation, and
 * should only be called if the administrator is absolutely sure the host is
 * definitely dead.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_declare_dead_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Enable console output to the physical display device next time this
 * host boots.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_enable_display(xen_session *session, enum xen_host_display *result, xen_host host);


/**
 * Enable console output to the physical display device next time this
 * host boots.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_enable_display_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Disable console output to the physical display device next time this
 * host boots.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_disable_display(xen_session *session, enum xen_host_display *result, xen_host host);


/**
 * Disable console output to the physical display device next time this
 * host boots.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_disable_display_async(xen_session *session, xen_task *result, xen_host host);


/**
 * Enable/disable SSLv3 for interoperability with older server
 * versions. When this is set to a different value, the host immediately
 * restarts its SSL/TLS listening service; typically this takes less than a
 * second but existing connections to it will be broken. API login sessions
 * will remain valid.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_ssl_legacy(xen_session *session, xen_host self, bool value);


/**
 * Enable/disable SSLv3 for interoperability with older server
 * versions. When this is set to a different value, the host immediately
 * restarts its SSL/TLS listening service; typically this takes less than a
 * second but existing connections to it will be broken. API login sessions
 * will remain valid.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_ssl_legacy_async(xen_session *session, xen_task *result, xen_host self, bool value);


/**
 * Sets the initiator IQN for the host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_iscsi_iqn(xen_session *session, xen_host host, char *value);


/**
 * Sets the initiator IQN for the host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_iscsi_iqn_async(xen_session *session, xen_task *result, xen_host host, char *value);


/**
 * Specifies whether multipathing is enabled.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_multipathing(xen_session *session, xen_host host, bool value);


/**
 * Specifies whether multipathing is enabled.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_multipathing_async(xen_session *session, xen_task *result, xen_host host, bool value);


/**
 * Sets the UEFI certificates on a host.
 * Minimum allowed role: Not Applicable.
 * Deprecated since 22.16.0.
 */
extern bool
xen_host_set_uefi_certificates(xen_session *session, xen_host host, char *value);


/**
 * Sets the UEFI certificates on a host.
 * Minimum allowed role: Not Applicable.
 * Deprecated since 22.16.0.
 */
extern bool
xen_host_set_uefi_certificates_async(xen_session *session, xen_task *result, xen_host host, char *value);


/**
 * Sets xen's sched-gran on a host. See:
 * https://xenbits.xen.org/docs/unstable/misc/xen-command-line.html#sched-gran-x86.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_set_sched_gran(xen_session *session, xen_host self, enum xen_host_sched_gran value);


/**
 * Sets xen's sched-gran on a host. See:
 * https://xenbits.xen.org/docs/unstable/misc/xen-command-line.html#sched-gran-x86.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_set_sched_gran_async(xen_session *session, xen_task *result, xen_host self, enum xen_host_sched_gran value);


/**
 * Gets xen's sched-gran on a host.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_get_sched_gran(xen_session *session, enum xen_host_sched_gran *result, xen_host self);


/**
 * Gets xen's sched-gran on a host.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_get_sched_gran_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Set VM placement NUMA affinity policy.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_numa_affinity_policy(xen_session *session, xen_host self, enum xen_host_numa_affinity_policy value);


/**
 * Set VM placement NUMA affinity policy.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_numa_affinity_policy_async(xen_session *session, xen_task *result, xen_host self, enum xen_host_numa_affinity_policy value);


/**
 * Disable TLS verification for this host only.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_emergency_disable_tls_verification(xen_session *session);


/**
 * Reenable TLS verification for this host only, and only after it was
 * emergency disabled.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_emergency_reenable_tls_verification(xen_session *session);


/**
 * apply updates from current enabled repository on a host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_apply_updates(xen_session *session, struct xen_string_set_set **result, xen_host self, char *hash);


/**
 * apply updates from current enabled repository on a host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_apply_updates_async(xen_session *session, xen_task *result, xen_host self, char *hash);


/**
 * Scan the host and update its driver information.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_rescan_drivers(xen_session *session, xen_host self);


/**
 * Scan the host and update its driver information.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_rescan_drivers_async(xen_session *session, xen_task *result, xen_host self);


/**
 * updates the host firewall to open or close port 80 depending on the
 * value.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_https_only(xen_session *session, xen_host self, bool value);


/**
 * updates the host firewall to open or close port 80 depending on the
 * value.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_set_https_only_async(xen_session *session, xen_task *result, xen_host self, bool value);


/**
 * apply all recommended guidances both on the host and on all HVM VMs
 * on the host after updates are applied on the host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_apply_recommended_guidances(xen_session *session, xen_host self);


/**
 * apply all recommended guidances both on the host and on all HVM VMs
 * on the host after updates are applied on the host.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_host_apply_recommended_guidances_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Clear the pending mandatory guidance on this host.
 * Minimum allowed role: Not Applicable.
 */
extern bool
xen_host_emergency_clear_mandatory_guidance(xen_session *session);


/**
 * Enable SSH access on the host. It will start the service sshd only
 * if it is not running. It will also enable the service sshd only if it is
 * not enabled. A newly joined host in the pool or an ejected host from the
 * pool would keep the original status.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_enable_ssh(xen_session *session, xen_host self);


/**
 * Enable SSH access on the host. It will start the service sshd only
 * if it is not running. It will also enable the service sshd only if it is
 * not enabled. A newly joined host in the pool or an ejected host from the
 * pool would keep the original status.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_enable_ssh_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Disable SSH access on the host. It will stop the service sshd only
 * if it is running. It will also disable the service sshd only if it is
 * enabled. A newly joined host in the pool or an ejected host from the pool
 * would keep the original status.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_disable_ssh(xen_session *session, xen_host self);


/**
 * Disable SSH access on the host. It will stop the service sshd only
 * if it is running. It will also disable the service sshd only if it is
 * enabled. A newly joined host in the pool or an ejected host from the pool
 * would keep the original status.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_disable_ssh_async(xen_session *session, xen_task *result, xen_host self);


/**
 * Set the SSH service enabled timeout for the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_ssh_enabled_timeout(xen_session *session, xen_host self, int64_t value);


/**
 * Set the SSH service enabled timeout for the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_ssh_enabled_timeout_async(xen_session *session, xen_task *result, xen_host self, int64_t value);


/**
 * Set the console idle timeout for the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_console_idle_timeout(xen_session *session, xen_host self, int64_t value);


/**
 * Set the console idle timeout for the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_console_idle_timeout_async(xen_session *session, xen_task *result, xen_host self, int64_t value);


/**
 * Set the SSH auto mode for the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_ssh_auto_mode(xen_session *session, xen_host self, bool value);


/**
 * Set the SSH auto mode for the host.
 * Minimum allowed role: pool-admin.
 */
extern bool
xen_host_set_ssh_auto_mode_async(xen_session *session, xen_task *result, xen_host self, bool value);


/**
 * Return a list of all the hosts known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_all(xen_session *session, struct xen_host_set **result);


/**
 * Return a map of host references to host records for all hosts known
 * to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_host_get_all_records(xen_session *session, xen_host_xen_host_record_map **result);


#endif
