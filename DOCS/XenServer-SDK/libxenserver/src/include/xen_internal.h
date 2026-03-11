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


#ifndef XEN_INTERNAL_H
#define XEN_INTERNAL_H


#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xen/api/xen_common.h>


enum abstract_typename
{
  VOID,
  STRING,
  INT,
  FLOAT,
  BOOL,
  DATETIME,
  SET,
  MAP,
  STRUCT,
  REF,
  ENUM,
  ENUMSET
};


typedef struct
{
    size_t size;
    void *contents[];
} arbitrary_set;


typedef struct struct_member struct_member;


typedef struct abstract_type
{
    enum abstract_typename XEN_API_TYPE;
    const struct abstract_type *child;
    const char * (*enum_marshaller)(int);
    int (*enum_demarshaller)(xen_session *, const char *);
    size_t struct_size;
    size_t member_count;
    const struct_member *members;
} abstract_type;


struct struct_member
{
    const char *key;
    const struct abstract_type *type;
    int offset;
};


extern const abstract_type abstract_type_string;
extern const abstract_type abstract_type_int;
extern const abstract_type abstract_type_float;
extern const abstract_type abstract_type_bool;
extern const abstract_type abstract_type_datetime;
extern const abstract_type abstract_type_ref;

extern const abstract_type abstract_type_string_set;
extern const abstract_type abstract_type_string_set_set;
extern const abstract_type abstract_type_ref_set;
extern const abstract_type abstract_type_int_set;

extern const abstract_type abstract_type_string_int_map;
extern const abstract_type abstract_type_string_string_map;
extern const abstract_type abstract_type_string_ref_map;
extern const abstract_type abstract_type_int_float_map;
extern const abstract_type abstract_type_int_int_map;
extern const abstract_type abstract_type_int_string_set_map;

extern const abstract_type abstract_type_string_string_set_map;
extern const abstract_type abstract_type_string_string_string_map_map;

extern const abstract_type xen_auth_record_abstract_type_;
extern const abstract_type xen_auth_record_set_abstract_type_;
extern const abstract_type xen_blob_record_abstract_type_;
extern const abstract_type xen_blob_record_set_abstract_type_;
extern const abstract_type xen_bond_record_abstract_type_;
extern const abstract_type xen_bond_record_set_abstract_type_;
extern const abstract_type xen_certificate_record_abstract_type_;
extern const abstract_type xen_certificate_record_set_abstract_type_;
extern const abstract_type xen_cluster_host_record_abstract_type_;
extern const abstract_type xen_cluster_host_record_set_abstract_type_;
extern const abstract_type xen_cluster_record_abstract_type_;
extern const abstract_type xen_cluster_record_set_abstract_type_;
extern const abstract_type xen_console_record_abstract_type_;
extern const abstract_type xen_console_record_set_abstract_type_;
extern const abstract_type xen_crashdump_record_abstract_type_;
extern const abstract_type xen_crashdump_record_set_abstract_type_;
extern const abstract_type xen_dr_task_record_abstract_type_;
extern const abstract_type xen_dr_task_record_set_abstract_type_;
extern const abstract_type xen_driver_variant_record_abstract_type_;
extern const abstract_type xen_driver_variant_record_set_abstract_type_;
extern const abstract_type xen_event_record_abstract_type_;
extern const abstract_type xen_event_record_set_abstract_type_;
extern const abstract_type xen_feature_record_abstract_type_;
extern const abstract_type xen_feature_record_set_abstract_type_;
extern const abstract_type xen_gpu_group_record_abstract_type_;
extern const abstract_type xen_gpu_group_record_set_abstract_type_;
extern const abstract_type xen_host_cpu_record_abstract_type_;
extern const abstract_type xen_host_cpu_record_set_abstract_type_;
extern const abstract_type xen_host_crashdump_record_abstract_type_;
extern const abstract_type xen_host_crashdump_record_set_abstract_type_;
extern const abstract_type xen_host_driver_record_abstract_type_;
extern const abstract_type xen_host_driver_record_set_abstract_type_;
extern const abstract_type xen_host_metrics_record_abstract_type_;
extern const abstract_type xen_host_metrics_record_set_abstract_type_;
extern const abstract_type xen_host_patch_record_abstract_type_;
extern const abstract_type xen_host_patch_record_set_abstract_type_;
extern const abstract_type xen_host_record_abstract_type_;
extern const abstract_type xen_host_record_set_abstract_type_;
extern const abstract_type xen_lvhd_record_abstract_type_;
extern const abstract_type xen_lvhd_record_set_abstract_type_;
extern const abstract_type xen_message_record_abstract_type_;
extern const abstract_type xen_message_record_set_abstract_type_;
extern const abstract_type xen_network_record_abstract_type_;
extern const abstract_type xen_network_record_set_abstract_type_;
extern const abstract_type xen_network_sriov_record_abstract_type_;
extern const abstract_type xen_network_sriov_record_set_abstract_type_;
extern const abstract_type xen_observer_record_abstract_type_;
extern const abstract_type xen_observer_record_set_abstract_type_;
extern const abstract_type xen_pbd_record_abstract_type_;
extern const abstract_type xen_pbd_record_set_abstract_type_;
extern const abstract_type xen_pci_record_abstract_type_;
extern const abstract_type xen_pci_record_set_abstract_type_;
extern const abstract_type xen_pgpu_record_abstract_type_;
extern const abstract_type xen_pgpu_record_set_abstract_type_;
extern const abstract_type xen_pif_metrics_record_abstract_type_;
extern const abstract_type xen_pif_metrics_record_set_abstract_type_;
extern const abstract_type xen_pif_record_abstract_type_;
extern const abstract_type xen_pif_record_set_abstract_type_;
extern const abstract_type xen_pool_patch_record_abstract_type_;
extern const abstract_type xen_pool_patch_record_set_abstract_type_;
extern const abstract_type xen_pool_record_abstract_type_;
extern const abstract_type xen_pool_record_set_abstract_type_;
extern const abstract_type xen_pool_update_record_abstract_type_;
extern const abstract_type xen_pool_update_record_set_abstract_type_;
extern const abstract_type xen_probe_result_record_abstract_type_;
extern const abstract_type xen_probe_result_record_set_abstract_type_;
extern const abstract_type xen_pusb_record_abstract_type_;
extern const abstract_type xen_pusb_record_set_abstract_type_;
extern const abstract_type xen_pvs_cache_storage_record_abstract_type_;
extern const abstract_type xen_pvs_cache_storage_record_set_abstract_type_;
extern const abstract_type xen_pvs_proxy_record_abstract_type_;
extern const abstract_type xen_pvs_proxy_record_set_abstract_type_;
extern const abstract_type xen_pvs_server_record_abstract_type_;
extern const abstract_type xen_pvs_server_record_set_abstract_type_;
extern const abstract_type xen_pvs_site_record_abstract_type_;
extern const abstract_type xen_pvs_site_record_set_abstract_type_;
extern const abstract_type xen_repository_record_abstract_type_;
extern const abstract_type xen_repository_record_set_abstract_type_;
extern const abstract_type xen_role_record_abstract_type_;
extern const abstract_type xen_role_record_set_abstract_type_;
extern const abstract_type xen_sdn_controller_record_abstract_type_;
extern const abstract_type xen_sdn_controller_record_set_abstract_type_;
extern const abstract_type xen_secret_record_abstract_type_;
extern const abstract_type xen_secret_record_set_abstract_type_;
extern const abstract_type xen_sm_record_abstract_type_;
extern const abstract_type xen_sm_record_set_abstract_type_;
extern const abstract_type xen_sr_record_abstract_type_;
extern const abstract_type xen_sr_record_set_abstract_type_;
extern const abstract_type xen_sr_stat_record_abstract_type_;
extern const abstract_type xen_sr_stat_record_set_abstract_type_;
extern const abstract_type xen_subject_record_abstract_type_;
extern const abstract_type xen_subject_record_set_abstract_type_;
extern const abstract_type xen_task_record_abstract_type_;
extern const abstract_type xen_task_record_set_abstract_type_;
extern const abstract_type xen_tunnel_record_abstract_type_;
extern const abstract_type xen_tunnel_record_set_abstract_type_;
extern const abstract_type xen_usb_group_record_abstract_type_;
extern const abstract_type xen_usb_group_record_set_abstract_type_;
extern const abstract_type xen_user_record_abstract_type_;
extern const abstract_type xen_user_record_set_abstract_type_;
extern const abstract_type xen_vbd_metrics_record_abstract_type_;
extern const abstract_type xen_vbd_metrics_record_set_abstract_type_;
extern const abstract_type xen_vbd_record_abstract_type_;
extern const abstract_type xen_vbd_record_set_abstract_type_;
extern const abstract_type xen_vdi_nbd_server_info_record_abstract_type_;
extern const abstract_type xen_vdi_nbd_server_info_record_set_abstract_type_;
extern const abstract_type xen_vdi_record_abstract_type_;
extern const abstract_type xen_vdi_record_set_abstract_type_;
extern const abstract_type xen_vgpu_record_abstract_type_;
extern const abstract_type xen_vgpu_record_set_abstract_type_;
extern const abstract_type xen_vgpu_type_record_abstract_type_;
extern const abstract_type xen_vgpu_type_record_set_abstract_type_;
extern const abstract_type xen_vif_metrics_record_abstract_type_;
extern const abstract_type xen_vif_metrics_record_set_abstract_type_;
extern const abstract_type xen_vif_record_abstract_type_;
extern const abstract_type xen_vif_record_set_abstract_type_;
extern const abstract_type xen_vlan_record_abstract_type_;
extern const abstract_type xen_vlan_record_set_abstract_type_;
extern const abstract_type xen_vm_appliance_record_abstract_type_;
extern const abstract_type xen_vm_appliance_record_set_abstract_type_;
extern const abstract_type xen_vm_group_record_abstract_type_;
extern const abstract_type xen_vm_group_record_set_abstract_type_;
extern const abstract_type xen_vm_guest_metrics_record_abstract_type_;
extern const abstract_type xen_vm_guest_metrics_record_set_abstract_type_;
extern const abstract_type xen_vm_metrics_record_abstract_type_;
extern const abstract_type xen_vm_metrics_record_set_abstract_type_;
extern const abstract_type xen_vm_record_abstract_type_;
extern const abstract_type xen_vm_record_set_abstract_type_;
extern const abstract_type xen_vmpp_record_abstract_type_;
extern const abstract_type xen_vmpp_record_set_abstract_type_;
extern const abstract_type xen_vmss_record_abstract_type_;
extern const abstract_type xen_vmss_record_set_abstract_type_;
extern const abstract_type xen_vtpm_record_abstract_type_;
extern const abstract_type xen_vtpm_record_set_abstract_type_;
extern const abstract_type xen_vusb_record_abstract_type_;
extern const abstract_type xen_vusb_record_set_abstract_type_;


typedef struct abstract_value
{
    const abstract_type *type;
    union
    {
        const char *string_val;
        int64_t int_val;
        int enum_val;
        double float_val;
        bool bool_val;
        arbitrary_set *set_val;
        void *struct_val;
        time_t datetime_val;
    } u;
} abstract_value;


extern void
xen_call_(xen_session *s, const char *method_name, abstract_value params[],
          int param_count, const abstract_type *result_type, void *value);


#define XEN_CALL_(method_name__)                                \
    xen_call_(session, method_name__, param_values,             \
              sizeof(param_values) / sizeof(param_values[0]),   \
              &result_type, result)                             \


extern char *
xen_strdup_(const char *in);


extern int
xen_enum_lookup_(const char *str, const char **lookup_table, int n);

#define ENUM_LOOKUP(str__, lookup_table__)      \
    xen_enum_lookup_(str__, lookup_table__,     \
                     sizeof(lookup_table__) /   \
                     sizeof(lookup_table__[0])) \

#define XEN_ALLOC(type__)                       \
type__ *                                        \
type__ ## _alloc()                              \
{                                               \
    return calloc(1, sizeof(type__));           \
}                                               \


#define XEN_FREE(type__)                        \
void                                            \
type__ ## _free(type__ handle)                  \
{                                               \
    free(handle);                               \
}                                               \


#define XEN_SET_ALLOC_FREE(type__)              \
    XEN_SET_ALLOC(type__)                       \
    XEN_SET_FREE(type__)


#define XEN_SET_ALLOC(type__)                                           \
type__ ## _set *                                                        \
type__ ## _set_alloc(size_t size)                                       \
{                                                                       \
    type__ ## _set *result = calloc(1, sizeof(type__ ## _set) +         \
                                    size * sizeof(type__));             \
    result->size = size;                                                \
    return result;                                                      \
}


#define XEN_SET_FREE(type__)                                            \
void type__ ## _set_free(type__ ## _set *set)                           \
{                                                                       \
    if (set == NULL)                                                    \
        return;                                                         \
    for (size_t i = 0; i < set->size; i++)                              \
       type__ ## _free(set->contents[i]);                               \
    free(set);                                                          \
}


#define XEN_RECORD_OPT_FREE(type__)                                     \
void type__ ## _record_opt_free(type__ ## _record_opt *opt) {           \
    if (opt == NULL) return;                                            \
    if (opt->is_record)                                                 \
        type__ ## _record_free(opt->u.record);                          \
    else                                                                \
        type__ ## _free(opt->u.handle);                                 \
    free(opt);                                                          \
}


#endif
