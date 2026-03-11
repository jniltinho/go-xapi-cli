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
    [Cmdlet(VerbsCommon.Get, "XenDriverVariantProperty", SupportsShouldProcess = false)]
    public class GetXenDriverVariantProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Driver_variant DriverVariant { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Driver_variant> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenDriverVariantProperty XenProperty { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string driver_variant = ParseDriverVariant();

            switch (XenProperty)
            {
                case XenDriverVariantProperty.Uuid:
                    ProcessRecordUuid(driver_variant);
                    break;
                case XenDriverVariantProperty.Name:
                    ProcessRecordName(driver_variant);
                    break;
                case XenDriverVariantProperty.Driver:
                    ProcessRecordDriver(driver_variant);
                    break;
                case XenDriverVariantProperty.Version:
                    ProcessRecordVersion(driver_variant);
                    break;
                case XenDriverVariantProperty.HardwarePresent:
                    ProcessRecordHardwarePresent(driver_variant);
                    break;
                case XenDriverVariantProperty.Priority:
                    ProcessRecordPriority(driver_variant);
                    break;
                case XenDriverVariantProperty.Status:
                    ProcessRecordStatus(driver_variant);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseDriverVariant()
        {
            string driver_variant = null;

            if (DriverVariant != null)
                driver_variant = (new XenRef<XenAPI.Driver_variant>(DriverVariant)).opaque_ref;
            else if (Ref != null)
                driver_variant = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'DriverVariant', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    DriverVariant));
            }

            return driver_variant;
        }

        private void ProcessRecordUuid(string driver_variant)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Driver_variant.get_uuid(session, driver_variant);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordName(string driver_variant)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Driver_variant.get_name(session, driver_variant);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordDriver(string driver_variant)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Driver_variant.get_driver(session, driver_variant);

                        XenAPI.Host_driver obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Host_driver.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordVersion(string driver_variant)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Driver_variant.get_version(session, driver_variant);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHardwarePresent(string driver_variant)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Driver_variant.get_hardware_present(session, driver_variant);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordPriority(string driver_variant)
        {
            RunApiCall(()=>
            {
                    double obj = XenAPI.Driver_variant.get_priority(session, driver_variant);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordStatus(string driver_variant)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Driver_variant.get_status(session, driver_variant);

                        WriteObject(obj, true);
            });
        }

        #endregion
    }

    public enum XenDriverVariantProperty
    {
        Uuid,
        Name,
        Driver,
        Version,
        HardwarePresent,
        Priority,
        Status
    }

}
