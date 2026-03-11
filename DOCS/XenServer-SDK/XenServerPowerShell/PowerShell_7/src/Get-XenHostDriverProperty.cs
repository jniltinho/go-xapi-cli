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
    [Cmdlet(VerbsCommon.Get, "XenHostDriverProperty", SupportsShouldProcess = false)]
    public class GetXenHostDriverProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Host_driver HostDriver { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Host_driver> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenHostDriverProperty XenProperty { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string host_driver = ParseHostDriver();

            switch (XenProperty)
            {
                case XenHostDriverProperty.Uuid:
                    ProcessRecordUuid(host_driver);
                    break;
                case XenHostDriverProperty.Host:
                    ProcessRecordHost(host_driver);
                    break;
                case XenHostDriverProperty.Name:
                    ProcessRecordName(host_driver);
                    break;
                case XenHostDriverProperty.FriendlyName:
                    ProcessRecordFriendlyName(host_driver);
                    break;
                case XenHostDriverProperty.Variants:
                    ProcessRecordVariants(host_driver);
                    break;
                case XenHostDriverProperty.ActiveVariant:
                    ProcessRecordActiveVariant(host_driver);
                    break;
                case XenHostDriverProperty.SelectedVariant:
                    ProcessRecordSelectedVariant(host_driver);
                    break;
                case XenHostDriverProperty.Type:
                    ProcessRecordType(host_driver);
                    break;
                case XenHostDriverProperty.Description:
                    ProcessRecordDescription(host_driver);
                    break;
                case XenHostDriverProperty.Info:
                    ProcessRecordInfo(host_driver);
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

        private void ProcessRecordUuid(string host_driver)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Host_driver.get_uuid(session, host_driver);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHost(string host_driver)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Host_driver.get_host(session, host_driver);

                        XenAPI.Host obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Host.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordName(string host_driver)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Host_driver.get_name(session, host_driver);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordFriendlyName(string host_driver)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Host_driver.get_friendly_name(session, host_driver);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordVariants(string host_driver)
        {
            RunApiCall(()=>
            {
                    var refs = XenAPI.Host_driver.get_variants(session, host_driver);

                        var records = new List<XenAPI.Driver_variant>();

                        foreach (var _ref in refs)
                        {
                            if (_ref.opaque_ref == "OpaqueRef:NULL")
                                continue;

                            var record = XenAPI.Driver_variant.get_record(session, _ref);
                            record.opaque_ref = _ref.opaque_ref;
                            records.Add(record);
                        }

                        WriteObject(records, true);
            });
        }

        private void ProcessRecordActiveVariant(string host_driver)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Host_driver.get_active_variant(session, host_driver);

                        XenAPI.Driver_variant obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Driver_variant.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordSelectedVariant(string host_driver)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Host_driver.get_selected_variant(session, host_driver);

                        XenAPI.Driver_variant obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Driver_variant.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordType(string host_driver)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Host_driver.get_type(session, host_driver);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordDescription(string host_driver)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Host_driver.get_description(session, host_driver);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordInfo(string host_driver)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Host_driver.get_info(session, host_driver);

                        WriteObject(obj, true);
            });
        }

        #endregion
    }

    public enum XenHostDriverProperty
    {
        Uuid,
        Host,
        Name,
        FriendlyName,
        Variants,
        ActiveVariant,
        SelectedVariant,
        Type,
        Description,
        Info
    }

}
