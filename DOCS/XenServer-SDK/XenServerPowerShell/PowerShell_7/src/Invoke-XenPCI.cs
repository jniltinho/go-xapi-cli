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
    [Cmdlet(VerbsLifecycle.Invoke, "XenPCI", SupportsShouldProcess = true)]
    public class InvokeXenPCI : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.PCI PCI { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.PCI> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }


        [Parameter(Mandatory = true)]
        public XenPCIAction XenAction { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenAction)
            {
                case XenPCIAction.DisableDom0Access:
                    _context = new XenPCIActionDisableDom0AccessDynamicParameters();
                    return _context;
                case XenPCIAction.EnableDom0Access:
                    _context = new XenPCIActionEnableDom0AccessDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string pci = ParsePCI();

            switch (XenAction)
            {
                case XenPCIAction.DisableDom0Access:
                    ProcessRecordDisableDom0Access(pci);
                    break;
                case XenPCIAction.EnableDom0Access:
                    ProcessRecordEnableDom0Access(pci);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParsePCI()
        {
            string pci = null;

            if (PCI != null)
                pci = (new XenRef<XenAPI.PCI>(PCI)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.PCI.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    pci = xenRef.opaque_ref;
            }
            else if (Ref != null)
                pci = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'PCI', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    PCI));
            }

            return pci;
        }

        private void ProcessRecordDisableDom0Access(string pci)
        {
            if (!ShouldProcess(pci, "PCI.disable_dom0_access"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenPCIActionDisableDom0AccessDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.PCI.async_disable_dom0_access(session, pci);

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
                    pci_dom0_access obj = XenAPI.PCI.disable_dom0_access(session, pci);

                    if (PassThru)
                    {
                        WriteObject(obj, true);
                    }
                }

            });
        }

        private void ProcessRecordEnableDom0Access(string pci)
        {
            if (!ShouldProcess(pci, "PCI.enable_dom0_access"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenPCIActionEnableDom0AccessDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.PCI.async_enable_dom0_access(session, pci);

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
                    pci_dom0_access obj = XenAPI.PCI.enable_dom0_access(session, pci);

                    if (PassThru)
                    {
                        WriteObject(obj, true);
                    }
                }

            });
        }

        #endregion
    }

    public enum XenPCIAction
    {
        DisableDom0Access,
        EnableDom0Access
    }

    public class XenPCIActionDisableDom0AccessDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

    public class XenPCIActionEnableDom0AccessDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

}
