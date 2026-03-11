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
    [Cmdlet(VerbsCommon.Set, "XenObserver", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.Observer))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class SetXenObserver : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Observer Observer { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Observer> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }

        [Parameter(ParameterSetName = "Name", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("name_label")]
        public string Name { get; set; }


        protected override bool GenerateAsyncParam
        {
            get
            {
                return _hostsIsSpecified
                       ^ _enabledIsSpecified
                       ^ _attributesIsSpecified
                       ^ _endpointsIsSpecified
                       ^ _componentsIsSpecified;
            }
        }

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

        [Parameter]
        public List<XenRef<XenAPI.Host>> Hosts
        {
            get { return _hosts; }
            set
            {
                _hosts = value;
                _hostsIsSpecified = true;
            }
        }
        private List<XenRef<XenAPI.Host>> _hosts;
        private bool _hostsIsSpecified;

        [Parameter]
        public bool Enabled
        {
            get { return _enabled; }
            set
            {
                _enabled = value;
                _enabledIsSpecified = true;
            }
        }
        private bool _enabled;
        private bool _enabledIsSpecified;

        [Parameter]
        public Hashtable Attributes
        {
            get { return _attributes; }
            set
            {
                _attributes = value;
                _attributesIsSpecified = true;
            }
        }
        private Hashtable _attributes;
        private bool _attributesIsSpecified;

        [Parameter]
        public string[] Endpoints
        {
            get { return _endpoints; }
            set
            {
                _endpoints = value;
                _endpointsIsSpecified = true;
            }
        }
        private string[] _endpoints;
        private bool _endpointsIsSpecified;

        [Parameter]
        public string[] Components
        {
            get { return _components; }
            set
            {
                _components = value;
                _componentsIsSpecified = true;
            }
        }
        private string[] _components;
        private bool _componentsIsSpecified;

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string observer = ParseObserver();

            if (_nameLabelIsSpecified)
                ProcessRecordNameLabel(observer);
            if (_nameDescriptionIsSpecified)
                ProcessRecordNameDescription(observer);
            if (_hostsIsSpecified)
                ProcessRecordHosts(observer);
            if (_enabledIsSpecified)
                ProcessRecordEnabled(observer);
            if (_attributesIsSpecified)
                ProcessRecordAttributes(observer);
            if (_endpointsIsSpecified)
                ProcessRecordEndpoints(observer);
            if (_componentsIsSpecified)
                ProcessRecordComponents(observer);

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

                        var obj = XenAPI.Observer.get_record(session, observer);
                        if (obj != null)
                            obj.opaque_ref = observer;
                        WriteObject(obj, true);

                    }
                });

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseObserver()
        {
            string observer = null;

            if (Observer != null)
                observer = (new XenRef<XenAPI.Observer>(Observer)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Observer.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    observer = xenRef.opaque_ref;
            }
            else if (Name != null)
            {
                var xenRefs = XenAPI.Observer.get_by_name_label(session, Name);
                if (xenRefs.Count == 1)
                    observer = xenRefs[0].opaque_ref;
                else if (xenRefs.Count > 1)
                    ThrowTerminatingError(new ErrorRecord(
                        new ArgumentException(string.Format("More than one XenAPI.Observer with name label {0} exist", Name)),
                        string.Empty,
                        ErrorCategory.InvalidArgument,
                        Name));
            }
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

        private void ProcessRecordNameLabel(string observer)
        {
            if (!ShouldProcess(observer, "Observer.set_name_label"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Observer.set_name_label(session, observer, NameLabel);

            });
        }

        private void ProcessRecordNameDescription(string observer)
        {
            if (!ShouldProcess(observer, "Observer.set_name_description"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Observer.set_name_description(session, observer, NameDescription);

            });
        }

        private void ProcessRecordHosts(string observer)
        {
            if (!ShouldProcess(observer, "Observer.set_hosts"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Observer.async_set_hosts(session, observer, Hosts);

                }
                else
                {
                    XenAPI.Observer.set_hosts(session, observer, Hosts);

                }

            });
        }

        private void ProcessRecordEnabled(string observer)
        {
            if (!ShouldProcess(observer, "Observer.set_enabled"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Observer.async_set_enabled(session, observer, Enabled);

                }
                else
                {
                    XenAPI.Observer.set_enabled(session, observer, Enabled);

                }

            });
        }

        private void ProcessRecordAttributes(string observer)
        {
            if (!ShouldProcess(observer, "Observer.set_attributes"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Observer.async_set_attributes(session, observer, CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(Attributes));

                }
                else
                {
                    XenAPI.Observer.set_attributes(session, observer, CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(Attributes));

                }

            });
        }

        private void ProcessRecordEndpoints(string observer)
        {
            if (!ShouldProcess(observer, "Observer.set_endpoints"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Observer.async_set_endpoints(session, observer, Endpoints);

                }
                else
                {
                    XenAPI.Observer.set_endpoints(session, observer, Endpoints);

                }

            });
        }

        private void ProcessRecordComponents(string observer)
        {
            if (!ShouldProcess(observer, "Observer.set_components"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Observer.async_set_components(session, observer, Components);

                }
                else
                {
                    XenAPI.Observer.set_components(session, observer, Components);

                }

            });
        }

        #endregion
    }
}
