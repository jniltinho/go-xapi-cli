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
    [Cmdlet(VerbsLifecycle.Invoke, "XenRepository", SupportsShouldProcess = true)]
    public class InvokeXenRepository : XenServerCmdlet
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


        [Parameter(Mandatory = true)]
        public XenRepositoryAction XenAction { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenAction)
            {
                case XenRepositoryAction.Introduce:
                    _context = new XenRepositoryActionIntroduceDynamicParameters();
                    return _context;
                case XenRepositoryAction.IntroduceBundle:
                    _context = new XenRepositoryActionIntroduceBundleDynamicParameters();
                    return _context;
                case XenRepositoryAction.IntroduceRemotePool:
                    _context = new XenRepositoryActionIntroduceRemotePoolDynamicParameters();
                    return _context;
                case XenRepositoryAction.Forget:
                    _context = new XenRepositoryActionForgetDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string repository = ParseRepository();

            switch (XenAction)
            {
                case XenRepositoryAction.Introduce:
                    ProcessRecordIntroduce(repository);
                    break;
                case XenRepositoryAction.IntroduceBundle:
                    ProcessRecordIntroduceBundle(repository);
                    break;
                case XenRepositoryAction.IntroduceRemotePool:
                    ProcessRecordIntroduceRemotePool(repository);
                    break;
                case XenRepositoryAction.Forget:
                    ProcessRecordForget(repository);
                    break;
            }

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

        private void ProcessRecordIntroduce(string repository)
        {
            if (!ShouldProcess(repository, "Repository.introduce"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenRepositoryActionIntroduceDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Repository.async_introduce(session, contxt.NameLabel, contxt.NameDescription, contxt.BinaryUrl, contxt.SourceUrl, contxt.Update, contxt.GpgkeyPath);

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
                    string objRef = XenAPI.Repository.introduce(session, contxt.NameLabel, contxt.NameDescription, contxt.BinaryUrl, contxt.SourceUrl, contxt.Update, contxt.GpgkeyPath);

                    if (PassThru)
                    {
                        XenAPI.Repository obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Repository.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                    }
                }

            });
        }

        private void ProcessRecordIntroduceBundle(string repository)
        {
            if (!ShouldProcess(repository, "Repository.introduce_bundle"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenRepositoryActionIntroduceBundleDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Repository.async_introduce_bundle(session, contxt.NameLabel, contxt.NameDescription);

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
                    string objRef = XenAPI.Repository.introduce_bundle(session, contxt.NameLabel, contxt.NameDescription);

                    if (PassThru)
                    {
                        XenAPI.Repository obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Repository.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                    }
                }

            });
        }

        private void ProcessRecordIntroduceRemotePool(string repository)
        {
            if (!ShouldProcess(repository, "Repository.introduce_remote_pool"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenRepositoryActionIntroduceRemotePoolDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Repository.async_introduce_remote_pool(session, contxt.NameLabel, contxt.NameDescription, contxt.BinaryUrl, contxt.Certificate);

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
                    string objRef = XenAPI.Repository.introduce_remote_pool(session, contxt.NameLabel, contxt.NameDescription, contxt.BinaryUrl, contxt.Certificate);

                    if (PassThru)
                    {
                        XenAPI.Repository obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Repository.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                    }
                }

            });
        }

        private void ProcessRecordForget(string repository)
        {
            if (!ShouldProcess(repository, "Repository.forget"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenRepositoryActionForgetDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Repository.async_forget(session, repository);

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
                    XenAPI.Repository.forget(session, repository);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        #endregion
    }

    public enum XenRepositoryAction
    {
        Introduce,
        IntroduceBundle,
        IntroduceRemotePool,
        Forget
    }

    public class XenRepositoryActionIntroduceDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

        [Parameter]
        public string NameLabel { get; set; }

        [Parameter]
        public string NameDescription { get; set; }

        [Parameter]
        public string BinaryUrl { get; set; }

        [Parameter]
        public string SourceUrl { get; set; }

        [Parameter]
        public bool Update { get; set; }

        [Parameter]
        public string GpgkeyPath { get; set; }
      
    }

    public class XenRepositoryActionIntroduceBundleDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

        [Parameter]
        public string NameLabel { get; set; }

        [Parameter]
        public string NameDescription { get; set; }
  
    }

    public class XenRepositoryActionIntroduceRemotePoolDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

        [Parameter]
        public string NameLabel { get; set; }

        [Parameter]
        public string NameDescription { get; set; }

        [Parameter]
        public string BinaryUrl { get; set; }

        [Parameter]
        public string Certificate { get; set; }
    
    }

    public class XenRepositoryActionForgetDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

    }

}
