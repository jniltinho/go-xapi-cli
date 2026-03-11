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
    [Cmdlet(VerbsCommon.New, "XenVBD", DefaultParameterSetName = "Hashtable", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.VBD))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class NewXenVBDCommand : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "Hashtable", Mandatory = true)]
        public Hashtable HashTable { get; set; }

        [Parameter(ParameterSetName = "Record", Mandatory = true)]
        public XenAPI.VBD Record { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.VM> VM { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.VDI> VDI { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string Device { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string Userdevice { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool Bootable { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public vbd_mode Mode { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public vbd_type Type { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool Unpluggable { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool Empty { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable OtherConfig { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool CurrentlyAttached { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string QosAlgorithmType { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable QosAlgorithmParams { get; set; }

        protected override bool GenerateAsyncParam
        {
            get { return true; }
        }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();
            if (Record == null && HashTable == null)
            {
                Record = new XenAPI.VBD();
                if (MyInvocation.BoundParameters.ContainsKey("VM"))
                    Record.VM = new XenRef<XenAPI.VM>(VM == null ? "OpaqueRef:NULL" : VM.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("VDI"))
                    Record.VDI = new XenRef<XenAPI.VDI>(VDI == null ? "OpaqueRef:NULL" : VDI.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("Device"))
                    Record.device = Device;
                if (MyInvocation.BoundParameters.ContainsKey("Userdevice"))
                    Record.userdevice = Userdevice;
                if (MyInvocation.BoundParameters.ContainsKey("Bootable"))
                    Record.bootable = Bootable;
                if (MyInvocation.BoundParameters.ContainsKey("Mode"))
                    Record.mode = Mode;
                if (MyInvocation.BoundParameters.ContainsKey("Type"))
                    Record.type = Type;
                if (MyInvocation.BoundParameters.ContainsKey("Unpluggable"))
                    Record.unpluggable = Unpluggable;
                if (MyInvocation.BoundParameters.ContainsKey("Empty"))
                    Record.empty = Empty;
                if (MyInvocation.BoundParameters.ContainsKey("OtherConfig"))
                    Record.other_config = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(OtherConfig);
                if (MyInvocation.BoundParameters.ContainsKey("CurrentlyAttached"))
                    Record.currently_attached = CurrentlyAttached;
                if (MyInvocation.BoundParameters.ContainsKey("QosAlgorithmType"))
                    Record.qos_algorithm_type = QosAlgorithmType;
                if (MyInvocation.BoundParameters.ContainsKey("QosAlgorithmParams"))
                    Record.qos_algorithm_params = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(QosAlgorithmParams);
            }
            else if (Record == null)
            {
                Record = new XenAPI.VBD(HashTable);
            }

            if (!ShouldProcess(session.Url, "VBD.create"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.VBD.async_create(session, Record);

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
                    string objRef = XenAPI.VBD.create(session, Record);

                    if (PassThru)
                    {
                        XenAPI.VBD obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.VBD.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                    }
                }

            });

            UpdateSessions();
        }

        #endregion
   }
}
