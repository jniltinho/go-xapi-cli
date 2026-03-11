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
    [Cmdlet(VerbsCommon.New, "XenVDI", DefaultParameterSetName = "Hashtable", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.VDI))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class NewXenVDICommand : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "Hashtable", Mandatory = true)]
        public Hashtable HashTable { get; set; }

        [Parameter(ParameterSetName = "Record", Mandatory = true)]
        public XenAPI.VDI Record { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string NameLabel { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string NameDescription { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.SR> SR { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long VirtualSize { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public vdi_type Type { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool Sharable { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool ReadOnly { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable OtherConfig { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable XenstoreData { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable SmConfig { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string[] Tags { get; set; }

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
                Record = new XenAPI.VDI();
                if (MyInvocation.BoundParameters.ContainsKey("NameLabel"))
                    Record.name_label = NameLabel;
                if (MyInvocation.BoundParameters.ContainsKey("NameDescription"))
                    Record.name_description = NameDescription;
                if (MyInvocation.BoundParameters.ContainsKey("SR"))
                    Record.SR = new XenRef<XenAPI.SR>(SR == null ? "OpaqueRef:NULL" : SR.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("VirtualSize"))
                    Record.virtual_size = VirtualSize;
                if (MyInvocation.BoundParameters.ContainsKey("Type"))
                    Record.type = Type;
                if (MyInvocation.BoundParameters.ContainsKey("Sharable"))
                    Record.sharable = Sharable;
                if (MyInvocation.BoundParameters.ContainsKey("ReadOnly"))
                    Record.read_only = ReadOnly;
                if (MyInvocation.BoundParameters.ContainsKey("OtherConfig"))
                    Record.other_config = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(OtherConfig);
                if (MyInvocation.BoundParameters.ContainsKey("XenstoreData"))
                    Record.xenstore_data = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(XenstoreData);
                if (MyInvocation.BoundParameters.ContainsKey("SmConfig"))
                    Record.sm_config = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(SmConfig);
                if (MyInvocation.BoundParameters.ContainsKey("Tags"))
                    Record.tags = Tags;
            }
            else if (Record == null)
            {
                Record = new XenAPI.VDI(HashTable);
            }

            if (!ShouldProcess(session.Url, "VDI.create"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.VDI.async_create(session, Record);

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
                    string objRef = XenAPI.VDI.create(session, Record);

                    if (PassThru)
                    {
                        XenAPI.VDI obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.VDI.get_record(session, objRef);
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
