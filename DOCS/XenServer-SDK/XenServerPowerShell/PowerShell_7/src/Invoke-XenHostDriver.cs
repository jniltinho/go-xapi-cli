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
    [Cmdlet(VerbsLifecycle.Invoke, "XenHostDriver", SupportsShouldProcess = true)]
    public class InvokeXenHostDriver : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Host_driver HostDriver { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Host_driver> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }


        [Parameter(Mandatory = true)]
        public XenHostDriverAction XenAction { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenAction)
            {
                case XenHostDriverAction.Select:
                    _context = new XenHostDriverActionSelectDynamicParameters();
                    return _context;
                case XenHostDriverAction.Deselect:
                    _context = new XenHostDriverActionDeselectDynamicParameters();
                    return _context;
                case XenHostDriverAction.Rescan:
                    _context = new XenHostDriverActionRescanDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string host_driver = ParseHostDriver();

            switch (XenAction)
            {
                case XenHostDriverAction.Select:
                    ProcessRecordSelect(host_driver);
                    break;
                case XenHostDriverAction.Deselect:
                    ProcessRecordDeselect(host_driver);
                    break;
                case XenHostDriverAction.Rescan:
                    ProcessRecordRescan(host_driver);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseHostDriver()
        {
            string host_driver = null;

            if (HostDriver != null)
                host_driver = (new XenRef<XenAPI.Host_driver>(HostDriver)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Host_driver.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    host_driver = xenRef.opaque_ref;
            }
            else if (Ref != null)
                host_driver = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'HostDriver', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    HostDriver));
            }

            return host_driver;
        }

        private void ProcessRecordSelect(string host_driver)
        {
            if (!ShouldProcess(host_driver, "Host_driver.select"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenHostDriverActionSelectDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Host_driver.async_select(session, host_driver, contxt.Variant);

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
                    XenAPI.Host_driver.select(session, host_driver, contxt.Variant);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        private void ProcessRecordDeselect(string host_driver)
        {
            if (!ShouldProcess(host_driver, "Host_driver.deselect"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenHostDriverActionDeselectDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Host_driver.async_deselect(session, host_driver);

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
                    XenAPI.Host_driver.deselect(session, host_driver);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        private void ProcessRecordRescan(string host_driver)
        {
            if (!ShouldProcess(host_driver, "Host_driver.rescan"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenHostDriverActionRescanDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Host_driver.async_rescan(session, contxt.XenHost);

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
                    XenAPI.Host_driver.rescan(session, contxt.XenHost);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        #endregion
    }

    public enum XenHostDriverAction
    {
        Select,
        Deselect,
        Rescan
    }

    public class XenHostDriverActionSelectDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

        [Parameter]
        public XenRef<XenAPI.Driver_variant> Variant { get; set; }
 
    }

    public class XenHostDriverActionDeselectDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

    public class XenHostDriverActionRescanDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

        [Parameter]
        public XenRef<XenAPI.Host> XenHost { get; set; }
 
    }

}
