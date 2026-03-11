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
    [Cmdlet(VerbsCommon.Get, "XenClusterProperty", SupportsShouldProcess = false)]
    public class GetXenClusterProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Cluster Cluster { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Cluster> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenClusterProperty XenProperty { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenProperty)
            {
                case XenClusterProperty.Network:
                    _context = new XenClusterPropertyNetworkDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string cluster = ParseCluster();

            switch (XenProperty)
            {
                case XenClusterProperty.Uuid:
                    ProcessRecordUuid(cluster);
                    break;
                case XenClusterProperty.ClusterHosts:
                    ProcessRecordClusterHosts(cluster);
                    break;
                case XenClusterProperty.PendingForget:
                    ProcessRecordPendingForget(cluster);
                    break;
                case XenClusterProperty.ClusterToken:
                    ProcessRecordClusterToken(cluster);
                    break;
                case XenClusterProperty.ClusterStack:
                    ProcessRecordClusterStack(cluster);
                    break;
                case XenClusterProperty.ClusterStackVersion:
                    ProcessRecordClusterStackVersion(cluster);
                    break;
                case XenClusterProperty.IsQuorate:
                    ProcessRecordIsQuorate(cluster);
                    break;
                case XenClusterProperty.Quorum:
                    ProcessRecordQuorum(cluster);
                    break;
                case XenClusterProperty.LiveHosts:
                    ProcessRecordLiveHosts(cluster);
                    break;
                case XenClusterProperty.ExpectedHosts:
                    ProcessRecordExpectedHosts(cluster);
                    break;
                case XenClusterProperty.AllowedOperations:
                    ProcessRecordAllowedOperations(cluster);
                    break;
                case XenClusterProperty.CurrentOperations:
                    ProcessRecordCurrentOperations(cluster);
                    break;
                case XenClusterProperty.PoolAutoJoin:
                    ProcessRecordPoolAutoJoin(cluster);
                    break;
                case XenClusterProperty.TokenTimeout:
                    ProcessRecordTokenTimeout(cluster);
                    break;
                case XenClusterProperty.TokenTimeoutCoefficient:
                    ProcessRecordTokenTimeoutCoefficient(cluster);
                    break;
                case XenClusterProperty.ClusterConfig:
                    ProcessRecordClusterConfig(cluster);
                    break;
                case XenClusterProperty.OtherConfig:
                    ProcessRecordOtherConfig(cluster);
                    break;
                case XenClusterProperty.Network:
                    ProcessRecordNetwork(cluster);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseCluster()
        {
            string cluster = null;

            if (Cluster != null)
                cluster = (new XenRef<XenAPI.Cluster>(Cluster)).opaque_ref;
            else if (Ref != null)
                cluster = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Cluster', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Cluster));
            }

            return cluster;
        }

        private void ProcessRecordUuid(string cluster)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Cluster.get_uuid(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordClusterHosts(string cluster)
        {
            RunApiCall(()=>
            {
                    var refs = XenAPI.Cluster.get_cluster_hosts(session, cluster);

                        var records = new List<XenAPI.Cluster_host>();

                        foreach (var _ref in refs)
                        {
                            if (_ref.opaque_ref == "OpaqueRef:NULL")
                                continue;

                            var record = XenAPI.Cluster_host.get_record(session, _ref);
                            record.opaque_ref = _ref.opaque_ref;
                            records.Add(record);
                        }

                        WriteObject(records, true);
            });
        }

        private void ProcessRecordPendingForget(string cluster)
        {
            RunApiCall(()=>
            {
                    string[] obj = XenAPI.Cluster.get_pending_forget(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordClusterToken(string cluster)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Cluster.get_cluster_token(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordClusterStack(string cluster)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Cluster.get_cluster_stack(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordClusterStackVersion(string cluster)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Cluster.get_cluster_stack_version(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordIsQuorate(string cluster)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Cluster.get_is_quorate(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordQuorum(string cluster)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Cluster.get_quorum(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLiveHosts(string cluster)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Cluster.get_live_hosts(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordExpectedHosts(string cluster)
        {
            RunApiCall(()=>
            {
                    long obj = XenAPI.Cluster.get_expected_hosts(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordAllowedOperations(string cluster)
        {
            RunApiCall(()=>
            {
                    List<cluster_operation> obj = XenAPI.Cluster.get_allowed_operations(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCurrentOperations(string cluster)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Cluster.get_current_operations(session, cluster);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordPoolAutoJoin(string cluster)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Cluster.get_pool_auto_join(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordTokenTimeout(string cluster)
        {
            RunApiCall(()=>
            {
                    double obj = XenAPI.Cluster.get_token_timeout(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordTokenTimeoutCoefficient(string cluster)
        {
            RunApiCall(()=>
            {
                    double obj = XenAPI.Cluster.get_token_timeout_coefficient(session, cluster);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordClusterConfig(string cluster)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Cluster.get_cluster_config(session, cluster);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordOtherConfig(string cluster)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Cluster.get_other_config(session, cluster);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordNetwork(string cluster)
        {
            RunApiCall(()=>
            {
                var contxt = _context as XenClusterPropertyNetworkDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster.async_get_network(session, cluster);

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
                    string objRef = XenAPI.Cluster.get_network(session, cluster);

                        XenAPI.Network obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Network.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                }

            });
        }

        #endregion
    }

    public enum XenClusterProperty
    {
        Uuid,
        ClusterHosts,
        PendingForget,
        ClusterToken,
        ClusterStack,
        ClusterStackVersion,
        IsQuorate,
        Quorum,
        LiveHosts,
        ExpectedHosts,
        AllowedOperations,
        CurrentOperations,
        PoolAutoJoin,
        TokenTimeout,
        TokenTimeoutCoefficient,
        ClusterConfig,
        OtherConfig,
        Network
    }

    public class XenClusterPropertyNetworkDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

}
