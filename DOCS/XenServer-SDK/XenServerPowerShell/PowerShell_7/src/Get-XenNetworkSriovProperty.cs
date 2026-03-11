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
    [Cmdlet(VerbsCommon.Get, "XenNetworkSriovProperty", SupportsShouldProcess = false)]
    public class GetXenNetworkSriovProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Network_sriov NetworkSriov { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Network_sriov> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenNetworkSriovProperty XenProperty { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenProperty)
            {
                case XenNetworkSriovProperty.RemainingCapacity:
                    _context = new XenNetworkSriovPropertyRemainingCapacityDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string network_sriov = ParseNetworkSriov();

            switch (XenProperty)
            {
                case XenNetworkSriovProperty.Uuid:
                    ProcessRecordUuid(network_sriov);
                    break;
                case XenNetworkSriovProperty.PhysicalPIF:
                    ProcessRecordPhysicalPIF(network_sriov);
                    break;
                case XenNetworkSriovProperty.LogicalPIF:
                    ProcessRecordLogicalPIF(network_sriov);
                    break;
                case XenNetworkSriovProperty.RequiresReboot:
                    ProcessRecordRequiresReboot(network_sriov);
                    break;
                case XenNetworkSriovProperty.ConfigurationMode:
                    ProcessRecordConfigurationMode(network_sriov);
                    break;
                case XenNetworkSriovProperty.RemainingCapacity:
                    ProcessRecordRemainingCapacity(network_sriov);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseNetworkSriov()
        {
            string network_sriov = null;

            if (NetworkSriov != null)
                network_sriov = (new XenRef<XenAPI.Network_sriov>(NetworkSriov)).opaque_ref;
            else if (Ref != null)
                network_sriov = Ref.opaque_ref;
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

        private void ProcessRecordUuid(string network_sriov)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Network_sriov.get_uuid(session, network_sriov);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordPhysicalPIF(string network_sriov)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Network_sriov.get_physical_PIF(session, network_sriov);

                        XenAPI.PIF obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.PIF.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordLogicalPIF(string network_sriov)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Network_sriov.get_logical_PIF(session, network_sriov);

                        XenAPI.PIF obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.PIF.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRequiresReboot(string network_sriov)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Network_sriov.get_requires_reboot(session, network_sriov);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordConfigurationMode(string network_sriov)
        {
            RunApiCall(()=>
            {
                    sriov_configuration_mode obj = XenAPI.Network_sriov.get_configuration_mode(session, network_sriov);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordRemainingCapacity(string network_sriov)
        {
            RunApiCall(()=>
            {
                var contxt = _context as XenNetworkSriovPropertyRemainingCapacityDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Network_sriov.async_get_remaining_capacity(session, network_sriov);

                        XenAPI.Task taskObj = null;
                        if (taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                }
                else
                {
                    long obj = XenAPI.Network_sriov.get_remaining_capacity(session, network_sriov);

                        WriteObject(obj, true);
                }

            });
        }

        #endregion
    }

    public enum XenNetworkSriovProperty
    {
        Uuid,
        PhysicalPIF,
        LogicalPIF,
        RequiresReboot,
        ConfigurationMode,
        RemainingCapacity
    }

    public class XenNetworkSriovPropertyRemainingCapacityDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

}
