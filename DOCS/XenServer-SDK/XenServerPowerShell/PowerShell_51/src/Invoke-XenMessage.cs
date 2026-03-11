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
    [Cmdlet(VerbsLifecycle.Invoke, "XenMessage", SupportsShouldProcess = true)]
    public class InvokeXenMessage : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Message Message { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Message> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }


        [Parameter(Mandatory = true)]
        public XenMessageAction XenAction { get; set; }

        #endregion

        public override object GetDynamicParameters()
        {
            switch (XenAction)
            {
                case XenMessageAction.DestroyMany:
                    _context = new XenMessageActionDestroyManyDynamicParameters();
                    return _context;
                default:
                    return null;
            }
        }

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string message = ParseMessage();

            switch (XenAction)
            {
                case XenMessageAction.DestroyMany:
                    ProcessRecordDestroyMany(message);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseMessage()
        {
            string message = null;

            if (Message != null)
                message = (new XenRef<XenAPI.Message>(Message)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Message.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    message = xenRef.opaque_ref;
            }
            else if (Ref != null)
                message = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Message', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Message));
            }

            return message;
        }

        private void ProcessRecordDestroyMany(string message)
        {
            if (!ShouldProcess(message, "Message.destroy_many"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenMessageActionDestroyManyDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Message.async_destroy_many(session, contxt.Messages);

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
                    XenAPI.Message.destroy_many(session, contxt.Messages);

                    if (PassThru)
                        WriteWarning("-PassThru can only be used with -Async for this cmdlet. Ignoring.");
                }

            });
        }

        #endregion
    }

    public enum XenMessageAction
    {
        DestroyMany
    }

    public class XenMessageActionDestroyManyDynamicParameters : IXenServerDynamicParameter
    {
        [Parameter]
        public SwitchParameter Async { get; set; }

        [Parameter]
        public List<XenRef<XenAPI.Message>> Messages { get; set; }
 
    }

}
