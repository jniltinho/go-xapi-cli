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
    [Cmdlet(VerbsCommon.Get, "XenVMGroupProperty", SupportsShouldProcess = false)]
    public class GetXenVMGroupProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.VM_group VMGroup { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.VM_group> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenVMGroupProperty XenProperty { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string vm_group = ParseVMGroup();

            switch (XenProperty)
            {
                case XenVMGroupProperty.Uuid:
                    ProcessRecordUuid(vm_group);
                    break;
                case XenVMGroupProperty.NameLabel:
                    ProcessRecordNameLabel(vm_group);
                    break;
                case XenVMGroupProperty.NameDescription:
                    ProcessRecordNameDescription(vm_group);
                    break;
                case XenVMGroupProperty.Placement:
                    ProcessRecordPlacement(vm_group);
                    break;
                case XenVMGroupProperty.VMs:
                    ProcessRecordVMs(vm_group);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseVMGroup()
        {
            string vm_group = null;

            if (VMGroup != null)
                vm_group = (new XenRef<XenAPI.VM_group>(VMGroup)).opaque_ref;
            else if (Ref != null)
                vm_group = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'VMGroup', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    VMGroup));
            }

            return vm_group;
        }

        private void ProcessRecordUuid(string vm_group)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.VM_group.get_uuid(session, vm_group);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameLabel(string vm_group)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.VM_group.get_name_label(session, vm_group);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameDescription(string vm_group)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.VM_group.get_name_description(session, vm_group);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordPlacement(string vm_group)
        {
            RunApiCall(()=>
            {
                    placement_policy obj = XenAPI.VM_group.get_placement(session, vm_group);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordVMs(string vm_group)
        {
            RunApiCall(()=>
            {
                    var refs = XenAPI.VM_group.get_VMs(session, vm_group);

                        var records = new List<XenAPI.VM>();

                        foreach (var _ref in refs)
                        {
                            if (_ref.opaque_ref == "OpaqueRef:NULL")
                                continue;

                            var record = XenAPI.VM.get_record(session, _ref);
                            record.opaque_ref = _ref.opaque_ref;
                            records.Add(record);
                        }

                        WriteObject(records, true);
            });
        }

        #endregion
    }

    public enum XenVMGroupProperty
    {
        Uuid,
        NameLabel,
        NameDescription,
        Placement,
        VMs
    }

}
