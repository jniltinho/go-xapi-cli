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
    [Cmdlet(VerbsCommon.Set, "XenRepository", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.Repository))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class SetXenRepository : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Repository Repository { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Repository> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }

        [Parameter(ParameterSetName = "Name", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("name_label")]
        public string Name { get; set; }


        protected override bool GenerateAsyncParam
        {
            get
            {
                return _gpgkeyPathIsSpecified;
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
        public string GpgkeyPath
        {
            get { return _gpgkeyPath; }
            set
            {
                _gpgkeyPath = value;
                _gpgkeyPathIsSpecified = true;
            }
        }
        private string _gpgkeyPath;
        private bool _gpgkeyPathIsSpecified;

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string repository = ParseRepository();

            if (_nameLabelIsSpecified)
                ProcessRecordNameLabel(repository);
            if (_nameDescriptionIsSpecified)
                ProcessRecordNameDescription(repository);
            if (_gpgkeyPathIsSpecified)
                ProcessRecordGpgkeyPath(repository);

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

                        var obj = XenAPI.Repository.get_record(session, repository);
                        if (obj != null)
                            obj.opaque_ref = repository;
                        WriteObject(obj, true);

                    }
                });

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseRepository()
        {
            string repository = null;

            if (Repository != null)
                repository = (new XenRef<XenAPI.Repository>(Repository)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Repository.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    repository = xenRef.opaque_ref;
            }
            else if (Name != null)
            {
                var xenRefs = XenAPI.Repository.get_by_name_label(session, Name);
                if (xenRefs.Count == 1)
                    repository = xenRefs[0].opaque_ref;
                else if (xenRefs.Count > 1)
                    ThrowTerminatingError(new ErrorRecord(
                        new ArgumentException(string.Format("More than one XenAPI.Repository with name label {0} exist", Name)),
                        string.Empty,
                        ErrorCategory.InvalidArgument,
                        Name));
            }
            else if (Ref != null)
                repository = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Repository', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Repository));
            }

            return repository;
        }

        private void ProcessRecordNameLabel(string repository)
        {
            if (!ShouldProcess(repository, "Repository.set_name_label"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Repository.set_name_label(session, repository, NameLabel);

            });
        }

        private void ProcessRecordNameDescription(string repository)
        {
            if (!ShouldProcess(repository, "Repository.set_name_description"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Repository.set_name_description(session, repository, NameDescription);

            });
        }

        private void ProcessRecordGpgkeyPath(string repository)
        {
            if (!ShouldProcess(repository, "Repository.set_gpgkey_path"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Repository.async_set_gpgkey_path(session, repository, GpgkeyPath);

                }
                else
                {
                    XenAPI.Repository.set_gpgkey_path(session, repository, GpgkeyPath);

                }

            });
        }

        #endregion
    }
}
