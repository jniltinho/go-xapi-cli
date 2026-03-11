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
    [Cmdlet(VerbsCommon.Set, "XenTask", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class SetXenTask : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Task Task { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Task> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }

        [Parameter(ParameterSetName = "Name", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("name_label")]
        public string Name { get; set; }


        protected override bool GenerateAsyncParam
        {
            get
            {
                return _otherConfigIsSpecified;
            }
        }

        [Parameter]
        public task_status_type Status
        {
            get { return _status; }
            set
            {
                _status = value;
                _statusIsSpecified = true;
            }
        }
        private task_status_type _status;
        private bool _statusIsSpecified;

        [Parameter]
        public double Progress
        {
            get { return _progress; }
            set
            {
                _progress = value;
                _progressIsSpecified = true;
            }
        }
        private double _progress;
        private bool _progressIsSpecified;

        [Parameter]
        public string Result
        {
            get { return _result; }
            set
            {
                _result = value;
                _resultIsSpecified = true;
            }
        }
        private string _result;
        private bool _resultIsSpecified;

        [Parameter]
        public string[] ErrorInfo
        {
            get { return _errorInfo; }
            set
            {
                _errorInfo = value;
                _errorInfoIsSpecified = true;
            }
        }
        private string[] _errorInfo;
        private bool _errorInfoIsSpecified;

        [Parameter]
        public Hashtable OtherConfig
        {
            get { return _otherConfig; }
            set
            {
                _otherConfig = value;
                _otherConfigIsSpecified = true;
            }
        }
        private Hashtable _otherConfig;
        private bool _otherConfigIsSpecified;

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string task = ParseTask();

            if (_statusIsSpecified)
                ProcessRecordStatus(task);
            if (_progressIsSpecified)
                ProcessRecordProgress(task);
            if (_resultIsSpecified)
                ProcessRecordResult(task);
            if (_errorInfoIsSpecified)
                ProcessRecordErrorInfo(task);
            if (_otherConfigIsSpecified)
                ProcessRecordOtherConfig(task);

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

                        var obj = XenAPI.Task.get_record(session, task);
                        if (obj != null)
                            obj.opaque_ref = task;
                        WriteObject(obj, true);

                    }
                });

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseTask()
        {
            string task = null;

            if (Task != null)
                task = (new XenRef<XenAPI.Task>(Task)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Task.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    task = xenRef.opaque_ref;
            }
            else if (Name != null)
            {
                var xenRefs = XenAPI.Task.get_by_name_label(session, Name);
                if (xenRefs.Count == 1)
                    task = xenRefs[0].opaque_ref;
                else if (xenRefs.Count > 1)
                    ThrowTerminatingError(new ErrorRecord(
                        new ArgumentException(string.Format("More than one XenAPI.Task with name label {0} exist", Name)),
                        string.Empty,
                        ErrorCategory.InvalidArgument,
                        Name));
            }
            else if (Ref != null)
                task = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Task', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Task));
            }

            return task;
        }

        private void ProcessRecordStatus(string task)
        {
            if (!ShouldProcess(task, "Task.set_status"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Task.set_status(session, task, Status);

            });
        }

        private void ProcessRecordProgress(string task)
        {
            if (!ShouldProcess(task, "Task.set_progress"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Task.set_progress(session, task, Progress);

            });
        }

        private void ProcessRecordResult(string task)
        {
            if (!ShouldProcess(task, "Task.set_result"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Task.set_result(session, task, Result);

            });
        }

        private void ProcessRecordErrorInfo(string task)
        {
            if (!ShouldProcess(task, "Task.set_error_info"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Task.set_error_info(session, task, ErrorInfo);

            });
        }

        private void ProcessRecordOtherConfig(string task)
        {
            if (!ShouldProcess(task, "Task.set_other_config"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Task.async_set_other_config(session, task, CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(OtherConfig));

                }
                else
                {
                    XenAPI.Task.set_other_config(session, task, CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(OtherConfig));

                }

            });
        }

        #endregion
    }
}
