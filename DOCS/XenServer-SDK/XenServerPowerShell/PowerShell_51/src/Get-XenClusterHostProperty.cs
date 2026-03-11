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
    [Cmdlet(VerbsCommon.Get, "XenClusterHostProperty", SupportsShouldProcess = false)]
    public class GetXenClusterHostProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Cluster_host ClusterHost { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Cluster_host> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenClusterHostProperty XenProperty { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string cluster_host = ParseClusterHost();

            switch (XenProperty)
            {
                case XenClusterHostProperty.Uuid:
                    ProcessRecordUuid(cluster_host);
                    break;
                case XenClusterHostProperty.Cluster:
                    ProcessRecordCluster(cluster_host);
                    break;
                case XenClusterHostProperty.Host:
                    ProcessRecordHost(cluster_host);
                    break;
                case XenClusterHostProperty.Enabled:
                    ProcessRecordEnabled(cluster_host);
                    break;
                case XenClusterHostProperty.PIF:
                    ProcessRecordPIF(cluster_host);
                    break;
                case XenClusterHostProperty.Joined:
                    ProcessRecordJoined(cluster_host);
                    break;
                case XenClusterHostProperty.Live:
                    ProcessRecordLive(cluster_host);
                    break;
                case XenClusterHostProperty.LastUpdateLive:
                    ProcessRecordLastUpdateLive(cluster_host);
                    break;
                case XenClusterHostProperty.AllowedOperations:
                    ProcessRecordAllowedOperations(cluster_host);
                    break;
                case XenClusterHostProperty.CurrentOperations:
                    ProcessRecordCurrentOperations(cluster_host);
                    break;
                case XenClusterHostProperty.OtherConfig:
                    ProcessRecordOtherConfig(cluster_host);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseClusterHost()
        {
            string cluster_host = null;

            if (ClusterHost != null)
                cluster_host = (new XenRef<XenAPI.Cluster_host>(ClusterHost)).opaque_ref;
            else if (Ref != null)
                cluster_host = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'ClusterHost', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    ClusterHost));
            }

            return cluster_host;
        }

        private void ProcessRecordUuid(string cluster_host)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Cluster_host.get_uuid(session, cluster_host);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCluster(string cluster_host)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Cluster_host.get_cluster(session, cluster_host);

                        XenAPI.Cluster obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Cluster.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHost(string cluster_host)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Cluster_host.get_host(session, cluster_host);

                        XenAPI.Host obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Host.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordEnabled(string cluster_host)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Cluster_host.get_enabled(session, cluster_host);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordPIF(string cluster_host)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Cluster_host.get_PIF(session, cluster_host);

                        XenAPI.PIF obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.PIF.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordJoined(string cluster_host)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Cluster_host.get_joined(session, cluster_host);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLive(string cluster_host)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Cluster_host.get_live(session, cluster_host);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLastUpdateLive(string cluster_host)
        {
            RunApiCall(()=>
            {
                    DateTime obj = XenAPI.Cluster_host.get_last_update_live(session, cluster_host);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordAllowedOperations(string cluster_host)
        {
            RunApiCall(()=>
            {
                    List<cluster_host_operation> obj = XenAPI.Cluster_host.get_allowed_operations(session, cluster_host);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCurrentOperations(string cluster_host)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Cluster_host.get_current_operations(session, cluster_host);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordOtherConfig(string cluster_host)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Cluster_host.get_other_config(session, cluster_host);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        #endregion
    }

    public enum XenClusterHostProperty
    {
        Uuid,
        Cluster,
        Host,
        Enabled,
        PIF,
        Joined,
        Live,
        LastUpdateLive,
        AllowedOperations,
        CurrentOperations,
        OtherConfig
    }

}
