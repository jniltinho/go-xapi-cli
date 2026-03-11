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
    [Cmdlet(VerbsLifecycle.Invoke, "XenCluster", SupportsShouldProcess = true)]
    public class InvokeXenCluster : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Cluster Cluster { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Cluster> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }


        [Parameter(Mandatory = true)]
        public XenClusterAction XenAction { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenAction)
            {
                case XenClusterAction.PoolCreate:
                    _context = new XenClusterActionPoolCreateDynamicParameters();
                    return _context;
                case XenClusterAction.PoolForceDestroy:
                    _context = new XenClusterActionPoolForceDestroyDynamicParameters();
                    return _context;
                case XenClusterAction.PoolDestroy:
                    _context = new XenClusterActionPoolDestroyDynamicParameters();
                    return _context;
                case XenClusterAction.PoolResync:
                    _context = new XenClusterActionPoolResyncDynamicParameters();
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

            switch (XenAction)
            {
                case XenClusterAction.PoolCreate:
                    ProcessRecordPoolCreate(cluster);
                    break;
                case XenClusterAction.PoolForceDestroy:
                    ProcessRecordPoolForceDestroy(cluster);
                    break;
                case XenClusterAction.PoolDestroy:
                    ProcessRecordPoolDestroy(cluster);
                    break;
                case XenClusterAction.PoolResync:
                    ProcessRecordPoolResync(cluster);
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
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Cluster.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    cluster = xenRef.opaque_ref;
            }
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

        private void ProcessRecordPoolCreate(string cluster)
        {
            if (!ShouldProcess(cluster, "Cluster.pool_create"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenClusterActionPoolCreateDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster.async_pool_create(session, contxt.Network, contxt.ClusterStack, contxt.TokenTimeout, contxt.TokenTimeoutCoefficient);

                    if (PassThru)
                    {
                        XenAPI.Task taskObj = null;
                        if (taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                    }
                }
                else
                {
                    string objRef = XenAPI.Cluster.pool_create(session, contxt.Network, contxt.ClusterStack, contxt.TokenTimeout, contxt.TokenTimeoutCoefficient);

                    if (PassThru)
                    {
                        XenAPI.Cluster obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Cluster.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                    }
                }

            });
        }

        private void ProcessRecordPoolForceDestroy(string cluster)
        {
            if (!ShouldProcess(cluster, "Cluster.pool_force_destroy"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenClusterActionPoolForceDestroyDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster.async_pool_force_destroy(session, cluster);

                    if (PassThru)
                    {
                        XenAPI.Task taskObj = null;
                        if (taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                    }
                }
                else
                {
                    XenAPI.Cluster.pool_force_destroy(session, cluster);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        private void ProcessRecordPoolDestroy(string cluster)
        {
            if (!ShouldProcess(cluster, "Cluster.pool_destroy"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenClusterActionPoolDestroyDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster.async_pool_destroy(session, cluster);

                    if (PassThru)
                    {
                        XenAPI.Task taskObj = null;
                        if (taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                    }
                }
                else
                {
                    XenAPI.Cluster.pool_destroy(session, cluster);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        private void ProcessRecordPoolResync(string cluster)
        {
            if (!ShouldProcess(cluster, "Cluster.pool_resync"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenClusterActionPoolResyncDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster.async_pool_resync(session, cluster);

                    if (PassThru)
                    {
                        XenAPI.Task taskObj = null;
                        if (taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                    }
                }
                else
                {
                    XenAPI.Cluster.pool_resync(session, cluster);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        #endregion
    }

    public enum XenClusterAction
    {
        PoolCreate,
        PoolForceDestroy,
        PoolDestroy,
        PoolResync
    }

    public class XenClusterActionPoolCreateDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

        [Parameter]
        public XenRef<XenAPI.Network> Network { get; set; }

        [Parameter]
        public string ClusterStack { get; set; }

        [Parameter]
        public double TokenTimeout { get; set; }

        [Parameter]
        public double TokenTimeoutCoefficient { get; set; }
    
    }

    public class XenClusterActionPoolForceDestroyDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

    public class XenClusterActionPoolDestroyDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

    public class XenClusterActionPoolResyncDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

}
