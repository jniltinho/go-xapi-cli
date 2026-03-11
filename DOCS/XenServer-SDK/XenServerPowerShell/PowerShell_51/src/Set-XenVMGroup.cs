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
    [Cmdlet(VerbsCommon.Set, "XenVMGroup", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.VM_group))]
    [OutputType(typeof(void))]
    public class SetXenVMGroup : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.VM_group VMGroup { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.VM_group> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }

        [Parameter(ParameterSetName = "Name", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("name_label")]
        public string Name { get; set; }


        [Parameter]
        public string NameLabel
        {
            get { return _nameLabel; }
            set
            {
                _nameLabel = value;
                _nameLabelIsSpecified = true;
            }
        }
        private string _nameLabel;
        private bool _nameLabelIsSpecified;

        [Parameter]
        public string NameDescription
        {
            get { return _nameDescription; }
            set
            {
                _nameDescription = value;
                _nameDescriptionIsSpecified = true;
            }
        }
        private string _nameDescription;
        private bool _nameDescriptionIsSpecified;

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string vm_group = ParseVMGroup();

            if (_nameLabelIsSpecified)
                ProcessRecordNameLabel(vm_group);
            if (_nameDescriptionIsSpecified)
                ProcessRecordNameDescription(vm_group);

            if (!PassThru)
                return;

            RunApiCall(() =>
                {
                    var contxt = _context as XenServerCmdletDynamicParameters;

                    if (contxt != null && contxt.Async)
                    {
                        XenAPI.Task taskObj = null;
                        if (taskRef != null && taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                    }
                    else
                    {

                        var obj = XenAPI.VM_group.get_record(session, vm_group);
                        if (obj != null)
                            obj.opaque_ref = vm_group;
                        WriteObject(obj, true);

                    }
                });

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseVMGroup()
        {
            string vm_group = null;

            if (VMGroup != null)
                vm_group = (new XenRef<XenAPI.VM_group>(VMGroup)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.VM_group.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    vm_group = xenRef.opaque_ref;
            }
            else if (Name != null)
            {
                var xenRefs = XenAPI.VM_group.get_by_name_label(session, Name);
                if (xenRefs.Count == 1)
                    vm_group = xenRefs[0].opaque_ref;
                else if (xenRefs.Count > 1)
                    ThrowTerminatingError(new ErrorRecord(
                        new ArgumentException(string.Format("More than one XenAPI.VM_group with name label {0} exist", Name)),
                        string.Empty,
                        ErrorCategory.InvalidArgument,
                        Name));
            }
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

        private void ProcessRecordNameLabel(string vm_group)
        {
            if (!ShouldProcess(vm_group, "VM_group.set_name_label"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.VM_group.set_name_label(session, vm_group, NameLabel);

            });
        }

        private void ProcessRecordNameDescription(string vm_group)
        {
            if (!ShouldProcess(vm_group, "VM_group.set_name_description"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.VM_group.set_name_description(session, vm_group, NameDescription);

            });
        }

        #endregion
    }
}
