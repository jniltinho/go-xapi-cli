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
    [Cmdlet(VerbsCommon.New, "XenVIF", DefaultParameterSetName = "Hashtable", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.VIF))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class NewXenVIFCommand : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "Hashtable", Mandatory = true)]
        public Hashtable HashTable { get; set; }

        [Parameter(ParameterSetName = "Record", Mandatory = true)]
        public XenAPI.VIF Record { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string Device { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.Network> Network { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.VM> VM { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string MAC { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long MTU { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable OtherConfig { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool CurrentlyAttached { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string QosAlgorithmType { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable QosAlgorithmParams { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public vif_locking_mode LockingMode { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string[] Ipv4Allowed { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string[] Ipv6Allowed { get; set; }

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
                Record = new XenAPI.VIF();
                if (MyInvocation.BoundParameters.ContainsKey("Device"))
                    Record.device = Device;
                if (MyInvocation.BoundParameters.ContainsKey("Network"))
                    Record.network = new XenRef<XenAPI.Network>(Network == null ? "OpaqueRef:NULL" : Network.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("VM"))
                    Record.VM = new XenRef<XenAPI.VM>(VM == null ? "OpaqueRef:NULL" : VM.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("MAC"))
                    Record.MAC = MAC;
                if (MyInvocation.BoundParameters.ContainsKey("MTU"))
                    Record.MTU = MTU;
                if (MyInvocation.BoundParameters.ContainsKey("OtherConfig"))
                    Record.other_config = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(OtherConfig);
                if (MyInvocation.BoundParameters.ContainsKey("CurrentlyAttached"))
                    Record.currently_attached = CurrentlyAttached;
                if (MyInvocation.BoundParameters.ContainsKey("QosAlgorithmType"))
                    Record.qos_algorithm_type = QosAlgorithmType;
                if (MyInvocation.BoundParameters.ContainsKey("QosAlgorithmParams"))
                    Record.qos_algorithm_params = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(QosAlgorithmParams);
                if (MyInvocation.BoundParameters.ContainsKey("LockingMode"))
                    Record.locking_mode = LockingMode;
                if (MyInvocation.BoundParameters.ContainsKey("Ipv4Allowed"))
                    Record.ipv4_allowed = Ipv4Allowed;
                if (MyInvocation.BoundParameters.ContainsKey("Ipv6Allowed"))
                    Record.ipv6_allowed = Ipv6Allowed;
            }
            else if (Record == null)
            {
                Record = new XenAPI.VIF(HashTable);
            }

            if (!ShouldProcess(session.Url, "VIF.create"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.VIF.async_create(session, Record);

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
                    string objRef = XenAPI.VIF.create(session, Record);

                    if (PassThru)
                    {
                        XenAPI.VIF obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.VIF.get_record(session, objRef);
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
