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


using System;
using System.Collections;
using System.Collections.Generic;
using System.Management.Automation;
using XenAPI;

namespace Citrix.XenServer.Commands
{
    [Cmdlet(VerbsCommon.Get, "XenPoolProperty", SupportsShouldProcess = false)]
    public class GetXenPoolProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Pool Pool { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Pool> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenPoolProperty XenProperty { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenProperty)
            {
                case XenPoolProperty.LicenseState:
                    _context = new XenPoolPropertyLicenseStateDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string pool = ParsePool();

            switch (XenProperty)
            {
                case XenPoolProperty.Uuid:
                    ProcessRecordUuid(pool);
                    break;
                case XenPoolProperty.NameLabel:
                    ProcessRecordNameLabel(pool);
                    break;
                case XenPoolProperty.NameDescription:
                    ProcessRecordNameDescription(pool);
                    break;
                case XenPoolProperty.Master:
                    ProcessRecordMaster(pool);
                    break;
                case XenPoolProperty.DefaultSR:
                    ProcessRecordDefaultSR(pool);
                    break;
                case XenPoolProperty.SuspendImageSR:
                    ProcessRecordSuspendImageSR(pool);
                    break;
                case XenPoolProperty.CrashDumpSR:
                    ProcessRecordCrashDumpSR(pool);
                    break;
                case XenPoolProperty.OtherConfig:
                    ProcessRecordOtherConfig(pool);
                    break;
                case XenPoolProperty.HaEnabled:
                    ProcessRecordHaEnabled(pool);
                    break;
                case XenPoolProperty.HaConfiguration:
                    ProcessRecordHaConfiguration(pool);
                    break;
                case XenPoolProperty.HaStatefiles:
                    ProcessRecordHaStatefiles(pool);
                    break;
                case XenPoolProperty.HaHostFailuresToTolerate:
                    ProcessRecordHaHostFailuresToTolerate(pool);
                    break;
                case XenPoolProperty.HaPlanExistsFor:
                    ProcessRecordHaPlanExistsFor(pool);
                    break;
                case XenPoolProperty.HaAllowOvercommit:
                    ProcessRecordHaAllowOvercommit(pool);
                    break;
                case XenPoolProperty.HaOvercommitted:
                    ProcessRecordHaOvercommitted(pool);
                    break;
                case XenPoolProperty.Blobs:
                    ProcessRecordBlobs(pool);
                    break;
                case XenPoolProperty.Tags:
                    ProcessRecordTags(pool);
                    break;
                case XenPoolProperty.GuiConfig:
                    ProcessRecordGuiConfig(pool);
                    break;
                case XenPoolProperty.HealthCheckConfig:
                    ProcessRecordHealthCheckConfig(pool);
                    break;
                case XenPoolProperty.WlbUrl:
                    ProcessRecordWlbUrl(pool);
                    break;
                case XenPoolProperty.WlbUsername:
                    ProcessRecordWlbUsername(pool);
                    break;
                case XenPoolProperty.WlbEnabled:
                    ProcessRecordWlbEnabled(pool);
                    break;
                case XenPoolProperty.WlbVerifyCert:
                    ProcessRecordWlbVerifyCert(pool);
                    break;
                case XenPoolProperty.RedoLogEnabled:
                    ProcessRecordRedoLogEnabled(pool);
                    break;
                case XenPoolProperty.RedoLogVdi:
                    ProcessRecordRedoLogVdi(pool);
                    break;
                case XenPoolProperty.VswitchController:
                    ProcessRecordVswitchController(pool);
                    break;
                case XenPoolProperty.Restrictions:
                    ProcessRecordRestrictions(pool);
                    break;
                case XenPoolProperty.MetadataVDIs:
                    ProcessRecordMetadataVDIs(pool);
                    break;
                case XenPoolProperty.HaClusterStack:
                    ProcessRecordHaClusterStack(pool);
                    break;
                case XenPoolProperty.AllowedOperations:
                    ProcessRecordAllowedOperations(pool);
                    break;
                case XenPoolProperty.CurrentOperations:
                    ProcessRecordCurrentOperations(pool);
                    break;
                case XenPoolProperty.GuestAgentConfig:
                    ProcessRecordGuestAgentConfig(pool);
                    break;
                case XenPoolProperty.CpuInfo:
                    ProcessRecordCpuInfo(pool);
                    break;
                case XenPoolProperty.PolicyNoVendorDevice:
                    ProcessRecordPolicyNoVendorDevice(pool);
                    break;
                case XenPoolProperty.LivePatchingDisabled:
                    ProcessRecordLivePatchingDisabled(pool);
                    break;
                case XenPoolProperty.IgmpSnoopingEnabled:
                    ProcessRecordIgmpSnoopingEnabled(pool);
                    break;
                case XenPoolProperty.UefiCertificates:
                    ProcessRecordUefiCertificates(pool);
                    break;
                case XenPoolProperty.CustomUefiCertificates:
                    ProcessRecordCustomUefiCertificates(pool);
                    break;
                case XenPoolProperty.IsPsrPending:
                    ProcessRecordIsPsrPending(pool);
                    break;
                case XenPoolProperty.TlsVerificationEnabled:
                    ProcessRecordTlsVerificationEnabled(pool);
                    break;
                case XenPoolProperty.Repositories:
                    ProcessRecordRepositories(pool);
                    break;
                case XenPoolProperty.ClientCertificateAuthEnabled:
                    ProcessRecordClientCertificateAuthEnabled(pool);
                    break;
                case XenPoolProperty.ClientCertificateAuthName:
                    ProcessRecordClientCertificateAuthName(pool);
                    break;
                case XenPoolProperty.RepositoryProxyUrl:
                    ProcessRecordRepositoryProxyUrl(pool);
                    break;
                case XenPoolProperty.RepositoryProxyUsername:
                    ProcessRecordRepositoryProxyUsername(pool);
                    break;
                case XenPoolProperty.RepositoryProxyPassword:
                    ProcessRecordRepositoryProxyPassword(pool);
                    break;
                case XenPoolProperty.MigrationCompression:
                    ProcessRecordMigrationCompression(pool);
                    break;
                case XenPoolProperty.CoordinatorBias:
                    ProcessRecordCoordinatorBias(pool);
                    break;
                case XenPoolProperty.LocalAuthMaxThreads:
                    ProcessRecordLocalAuthMaxThreads(pool);
                    break;
                case XenPoolProperty.ExtAuthMaxThreads:
                    ProcessRecordExtAuthMaxThreads(pool);
                    break;
                case XenPoolProperty.ExtAuthCacheEnabled:
                    ProcessRecordExtAuthCacheEnabled(pool);
                    break;
                case XenPoolProperty.ExtAuthCacheSize:
                    ProcessRecordExtAuthCacheSize(pool);
                    break;
                case XenPoolProperty.ExtAuthCacheExpiry:
                    ProcessRecordExtAuthCacheExpiry(pool);
                    break;
                case XenPoolProperty.TelemetryUuid:
                    ProcessRecordTelemetryUuid(pool);
                    break;
                case XenPoolProperty.TelemetryFrequency:
                    ProcessRecordTelemetryFrequency(pool);
                    break;
                case XenPoolProperty.TelemetryNextCollection:
                    ProcessRecordTelemetryNextCollection(pool);
                    break;
                case XenPoolProperty.LastUpdateSync:
                    ProcessRecordLastUpdateSync(pool);
                    break;
                case XenPoolProperty.UpdateSyncFrequency:
                    ProcessRecordUpdateSyncFrequency(pool);
                    break;
                case XenPoolProperty.UpdateSyncDay:
                    ProcessRecordUpdateSyncDay(pool);
                    break;
                case XenPoolProperty.UpdateSyncEnabled:
                    ProcessRecordUpdateSyncEnabled(pool);
                    break;
                case XenPoolProperty.Recommendations:
                    ProcessRecordRecommendations(pool);
                    break;
                case XenPoolProperty.LicenseServer:
                    ProcessRecordLicenseServer(pool);
                    break;
                case XenPoolProperty.HaRebootVmOnInternalShutdown:
                    ProcessRecordHaRebootVmOnInternalShutdown(pool);
                    break;
                case XenPoolProperty.LicenseState:
                    ProcessRecordLicenseState(pool);
                    break;
                case XenPoolProperty.GuestSecurebootReadiness:
                    ProcessRecordGuestSecurebootReadiness(pool);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParsePool()
        {
            string pool = null;

            if (Pool != null)
                pool = (new XenRef<XenAPI.Pool>(Pool)).opaque_ref;
            else if (Ref != null)
                pool = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Pool', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Pool));
            }

            return pool;
        }

        private void ProcessRecordUuid(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_uuid(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameLabel(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_name_label(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameDescription(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_name_description(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordMaster(string pool)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Pool.get_master(session, pool);

                        XenAPI.Host obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Host.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordDefaultSR(string pool)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Pool.get_default_SR(session, pool);

                        XenAPI.SR obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.SR.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordSuspendImageSR(string pool)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Pool.get_suspend_image_SR(session, pool);

                        XenAPI.SR obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.SR.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCrashDumpSR(string pool)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Pool.get_crash_dump_SR(session, pool);

                        XenAPI.SR obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.SR.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordOtherConfig(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_other_config(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordHaEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_ha_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHaConfiguration(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_ha_configuration(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordHaStatefiles(string pool)
        {
            RunApiCall(()=>
            {
                    string[] obj = XenAPI.Pool.get_ha_statefiles(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHaHostFailuresToTolerate(string pool)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Pool.get_ha_host_failures_to_tolerate(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHaPlanExistsFor(string pool)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Pool.get_ha_plan_exists_for(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHaAllowOvercommit(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_ha_allow_overcommit(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHaOvercommitted(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_ha_overcommitted(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordBlobs(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_blobs(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordTags(string pool)
        {
            RunApiCall(()=>
            {
                    string[] obj = XenAPI.Pool.get_tags(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordGuiConfig(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_gui_config(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordHealthCheckConfig(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_health_check_config(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordWlbUrl(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_wlb_url(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordWlbUsername(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_wlb_username(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordWlbEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_wlb_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordWlbVerifyCert(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_wlb_verify_cert(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRedoLogEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_redo_log_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRedoLogVdi(string pool)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Pool.get_redo_log_vdi(session, pool);

                        XenAPI.VDI obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.VDI.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordVswitchController(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_vswitch_controller(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRestrictions(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_restrictions(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordMetadataVDIs(string pool)
        {
            RunApiCall(()=>
            {
                    var refs = XenAPI.Pool.get_metadata_VDIs(session, pool);

                        var records = new List<XenAPI.VDI>();

                        foreach (var _ref in refs)
                        {
                            if (_ref.opaque_ref == "OpaqueRef:NULL")
                                continue;

                            var record = XenAPI.VDI.get_record(session, _ref);
                            record.opaque_ref = _ref.opaque_ref;
                            records.Add(record);
                        }

                        WriteObject(records, true);
            });
        }

        private void ProcessRecordHaClusterStack(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_ha_cluster_stack(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordAllowedOperations(string pool)
        {
            RunApiCall(()=>
            {
                    List<pool_allowed_operations> obj = XenAPI.Pool.get_allowed_operations(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCurrentOperations(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_current_operations(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordGuestAgentConfig(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_guest_agent_config(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordCpuInfo(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_cpu_info(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordPolicyNoVendorDevice(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_policy_no_vendor_device(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLivePatchingDisabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_live_patching_disabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordIgmpSnoopingEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_igmp_snooping_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordUefiCertificates(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_uefi_certificates(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCustomUefiCertificates(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_custom_uefi_certificates(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordIsPsrPending(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_is_psr_pending(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordTlsVerificationEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_tls_verification_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRepositories(string pool)
        {
            RunApiCall(()=>
            {
                    var refs = XenAPI.Pool.get_repositories(session, pool);

                        var records = new List<XenAPI.Repository>();

                        foreach (var _ref in refs)
                        {
                            if (_ref.opaque_ref == "OpaqueRef:NULL")
                                continue;

                            var record = XenAPI.Repository.get_record(session, _ref);
                            record.opaque_ref = _ref.opaque_ref;
                            records.Add(record);
                        }

                        WriteObject(records, true);
            });
        }

        private void ProcessRecordClientCertificateAuthEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_client_certificate_auth_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordClientCertificateAuthName(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_client_certificate_auth_name(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRepositoryProxyUrl(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_repository_proxy_url(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRepositoryProxyUsername(string pool)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Pool.get_repository_proxy_username(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRepositoryProxyPassword(string pool)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Pool.get_repository_proxy_password(session, pool);

                        XenAPI.Secret obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Secret.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordMigrationCompression(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_migration_compression(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCoordinatorBias(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_coordinator_bias(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLocalAuthMaxThreads(string pool)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Pool.get_local_auth_max_threads(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordExtAuthMaxThreads(string pool)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Pool.get_ext_auth_max_threads(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordExtAuthCacheEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_ext_auth_cache_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordExtAuthCacheSize(string pool)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Pool.get_ext_auth_cache_size(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordExtAuthCacheExpiry(string pool)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Pool.get_ext_auth_cache_expiry(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordTelemetryUuid(string pool)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Pool.get_telemetry_uuid(session, pool);

                        XenAPI.Secret obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Secret.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordTelemetryFrequency(string pool)
        {
            RunApiCall(()=>
            {
                    telemetry_frequency obj = XenAPI.Pool.get_telemetry_frequency(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordTelemetryNextCollection(string pool)
        {
            RunApiCall(()=>
            {
                    DateTime obj = XenAPI.Pool.get_telemetry_next_collection(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLastUpdateSync(string pool)
        {
            RunApiCall(()=>
            {
                    DateTime obj = XenAPI.Pool.get_last_update_sync(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordUpdateSyncFrequency(string pool)
        {
            RunApiCall(()=>
            {
                    update_sync_frequency obj = XenAPI.Pool.get_update_sync_frequency(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordUpdateSyncDay(string pool)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Pool.get_update_sync_day(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordUpdateSyncEnabled(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_update_sync_enabled(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRecommendations(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_recommendations(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordLicenseServer(string pool)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Pool.get_license_server(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordHaRebootVmOnInternalShutdown(string pool)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Pool.get_ha_reboot_vm_on_internal_shutdown(session, pool);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLicenseState(string pool)
        {
            RunApiCall(()=>
            {
                var contxt = _context as XenPoolPropertyLicenseStateDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Pool.async_get_license_state(session, pool);

                        XenAPI.Task taskObj = null;
                        if (taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                }
                else
                {
                    var dict = XenAPI.Pool.get_license_state(session, pool);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
                }

            });
        }

        private void ProcessRecordGuestSecurebootReadiness(string pool)
        {
            RunApiCall(()=>
            {
                    pool_guest_secureboot_readiness obj = XenAPI.Pool.get_guest_secureboot_readiness(session, pool);

                        WriteObject(obj, true);
            });
        }

        #endregion
    }

    public enum XenPoolProperty
    {
        Uuid,
        NameLabel,
        NameDescription,
        Master,
        DefaultSR,
        SuspendImageSR,
        CrashDumpSR,
        OtherConfig,
        HaEnabled,
        HaConfiguration,
        HaStatefiles,
        HaHostFailuresToTolerate,
        HaPlanExistsFor,
        HaAllowOvercommit,
        HaOvercommitted,
        Blobs,
        Tags,
        GuiConfig,
        HealthCheckConfig,
        WlbUrl,
        WlbUsername,
        WlbEnabled,
        WlbVerifyCert,
        RedoLogEnabled,
        RedoLogVdi,
        VswitchController,
        Restrictions,
        MetadataVDIs,
        HaClusterStack,
        AllowedOperations,
        CurrentOperations,
        GuestAgentConfig,
        CpuInfo,
        PolicyNoVendorDevice,
        LivePatchingDisabled,
        IgmpSnoopingEnabled,
        UefiCertificates,
        CustomUefiCertificates,
        IsPsrPending,
        TlsVerificationEnabled,
        Repositories,
        ClientCertificateAuthEnabled,
        ClientCertificateAuthName,
        RepositoryProxyUrl,
        RepositoryProxyUsername,
        RepositoryProxyPassword,
        MigrationCompression,
        CoordinatorBias,
        LocalAuthMaxThreads,
        ExtAuthMaxThreads,
        ExtAuthCacheEnabled,
        ExtAuthCacheSize,
        ExtAuthCacheExpiry,
        TelemetryUuid,
        TelemetryFrequency,
        TelemetryNextCollection,
        LastUpdateSync,
        UpdateSyncFrequency,
        UpdateSyncDay,
        UpdateSyncEnabled,
        Recommendations,
        LicenseServer,
        HaRebootVmOnInternalShutdown,
        LicenseState,
        GuestSecurebootReadiness
    }

    public class XenPoolPropertyLicenseStateDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

}
