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
    [Cmdlet(VerbsCommon.Get, "XenObserverProperty", SupportsShouldProcess = false)]
    public class GetXenObserverProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Observer Observer { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Observer> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenObserverProperty XenProperty { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string observer = ParseObserver();

            switch (XenProperty)
            {
                case XenObserverProperty.Uuid:
                    ProcessRecordUuid(observer);
                    break;
                case XenObserverProperty.NameLabel:
                    ProcessRecordNameLabel(observer);
                    break;
                case XenObserverProperty.NameDescription:
                    ProcessRecordNameDescription(observer);
                    break;
                case XenObserverProperty.Hosts:
                    ProcessRecordHosts(observer);
                    break;
                case XenObserverProperty.Attributes:
                    ProcessRecordAttributes(observer);
                    break;
                case XenObserverProperty.Endpoints:
                    ProcessRecordEndpoints(observer);
                    break;
                case XenObserverProperty.Components:
                    ProcessRecordComponents(observer);
                    break;
                case XenObserverProperty.Enabled:
                    ProcessRecordEnabled(observer);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseObserver()
        {
            string observer = null;

            if (Observer != null)
                observer = (new XenRef<XenAPI.Observer>(Observer)).opaque_ref;
            else if (Ref != null)
                observer = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Observer', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Observer));
            }

            return observer;
        }

        private void ProcessRecordUuid(string observer)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Observer.get_uuid(session, observer);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameLabel(string observer)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Observer.get_name_label(session, observer);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameDescription(string observer)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Observer.get_name_description(session, observer);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHosts(string observer)
        {
            RunApiCall(()=>
            {
                    var refs = XenAPI.Observer.get_hosts(session, observer);

                        var records = new List<XenAPI.Host>();

                        foreach (var _ref in refs)
                        {
                            if (_ref.opaque_ref == "OpaqueRef:NULL")
                                continue;

                            var record = XenAPI.Host.get_record(session, _ref);
                            record.opaque_ref = _ref.opaque_ref;
                            records.Add(record);
                        }

                        WriteObject(records, true);
            });
        }

        private void ProcessRecordAttributes(string observer)
        {
            RunApiCall(()=>
            {
                    var dict = XenAPI.Observer.get_attributes(session, observer);

                        Hashtable ht = CommonCmdletFunctions.ConvertDictionaryToHashtable(dict);
                        WriteObject(ht, true);
            });
        }

        private void ProcessRecordEndpoints(string observer)
        {
            RunApiCall(()=>
            {
                    string[] obj = XenAPI.Observer.get_endpoints(session, observer);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordComponents(string observer)
        {
            RunApiCall(()=>
            {
                    string[] obj = XenAPI.Observer.get_components(session, observer);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordEnabled(string observer)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Observer.get_enabled(session, observer);

                        WriteObject(obj, true);
            });
        }

        #endregion
    }

    public enum XenObserverProperty
    {
        Uuid,
        NameLabel,
        NameDescription,
        Hosts,
        Attributes,
        Endpoints,
        Components,
        Enabled
    }

}
