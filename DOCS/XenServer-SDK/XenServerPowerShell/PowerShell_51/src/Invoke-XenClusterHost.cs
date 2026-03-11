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
    [Cmdlet(VerbsLifecycle.Invoke, "XenClusterHost", SupportsShouldProcess = true)]
    public class InvokeXenClusterHost : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Cluster_host ClusterHost { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Cluster_host> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }


        [Parameter(Mandatory = true)]
        public XenClusterHostAction XenAction { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenAction)
            {
                case XenClusterHostAction.Enable:
                    _context = new XenClusterHostActionEnableDynamicParameters();
                    return _context;
                case XenClusterHostAction.ForceDestroy:
                    _context = new XenClusterHostActionForceDestroyDynamicParameters();
                    return _context;
                case XenClusterHostAction.Disable:
                    _context = new XenClusterHostActionDisableDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string cluster_host = ParseClusterHost();

            switch (XenAction)
            {
                case XenClusterHostAction.Enable:
                    ProcessRecordEnable(cluster_host);
                    break;
                case XenClusterHostAction.ForceDestroy:
                    ProcessRecordForceDestroy(cluster_host);
                    break;
                case XenClusterHostAction.Disable:
                    ProcessRecordDisable(cluster_host);
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
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Cluster_host.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    cluster_host = xenRef.opaque_ref;
            }
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

        private void ProcessRecordEnable(string cluster_host)
        {
            if (!ShouldProcess(cluster_host, "Cluster_host.enable"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenClusterHostActionEnableDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster_host.async_enable(session, cluster_host);

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
                    XenAPI.Cluster_host.enable(session, cluster_host);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        private void ProcessRecordForceDestroy(string cluster_host)
        {
            if (!ShouldProcess(cluster_host, "Cluster_host.force_destroy"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenClusterHostActionForceDestroyDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster_host.async_force_destroy(session, cluster_host);

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
                    XenAPI.Cluster_host.force_destroy(session, cluster_host);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        private void ProcessRecordDisable(string cluster_host)
        {
            if (!ShouldProcess(cluster_host, "Cluster_host.disable"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenClusterHostActionDisableDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster_host.async_disable(session, cluster_host);

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
                    XenAPI.Cluster_host.disable(session, cluster_host);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        #endregion
    }

    public enum XenClusterHostAction
    {
        Enable,
        ForceDestroy,
        Disable
    }

    public class XenClusterHostActionEnableDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

    public class XenClusterHostActionForceDestroyDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

    public class XenClusterHostActionDisableDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

}
