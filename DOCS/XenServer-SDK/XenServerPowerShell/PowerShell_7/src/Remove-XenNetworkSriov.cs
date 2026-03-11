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
using System.Management.Automation;
using XenAPI;

namespace Citrix.XenServer.Commands
{
    [Cmdlet(VerbsCommon.Remove, "XenNetworkSriov", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class RemoveXenNetworkSriov : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Network_sriov NetworkSriov { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Network_sriov> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }

        protected override bool GenerateAsyncParam => true;

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string network_sriov = ParseNetworkSriov();

            ProcessRecordDestroy(network_sriov);

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseNetworkSriov()
        {
            string network_sriov = null;

            if (NetworkSriov != null)
            {
                network_sriov = (new XenRef<XenAPI.Network_sriov>(NetworkSriov)).opaque_ref;
            }
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Network_sriov.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    network_sriov = xenRef.opaque_ref;
            }
            else if (Ref != null)
            {
                network_sriov = Ref.opaque_ref;
            }
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'NetworkSriov', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    NetworkSriov));
            }

            return network_sriov;
        }

        private void ProcessRecordDestroy(string network_sriov)
        {
            if (!ShouldProcess(network_sriov, "Network_sriov.destroy"))
                return;

            RunApiCall(() =>
            {
                if (_context is XenServerCmdletDynamicParameters conTxt && conTxt.Async)
                {
                    taskRef = XenAPI.Network_sriov.async_destroy(session, network_sriov);

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
                    XenAPI.Network_sriov.destroy(session, network_sriov);
                }
            });
        }

        #endregion
    }
}