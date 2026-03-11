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
    [Cmdlet(VerbsCommon.Add, "XenCluster", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.Cluster))]
    [OutputType(typeof(void))]
    public class AddXenCluster : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Cluster Cluster { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Cluster> Ref { get; set; }

        [Parameter(ParameterSetName = "Uuid", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        public Guid Uuid { get; set; }


        [Parameter]
        public KeyValuePair<string, string> OtherConfig
        {
            get { return _otherConfig; }
            set
            {
                _otherConfig = value;
                _otherConfigIsSpecified = true;
            }
        }
        private KeyValuePair<string, string> _otherConfig;
        private bool _otherConfigIsSpecified;

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string cluster = ParseCluster();

            if (_otherConfigIsSpecified)
                ProcessRecordOtherConfig(cluster);

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

                        var obj = XenAPI.Cluster.get_record(session, cluster);
                        if (obj != null)
                            obj.opaque_ref = cluster;
                        WriteObject(obj, true);

                    }
                });

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseCluster()
        {
            string cluster = null;

            if (Cluster != null)
                cluster = (new XenRef<XenAPI.Cluster>(Cluster)).opaque_ref;
            else if (Uuid != Guid.Empty)
            {
                var xenRef = XenAPI.Cluster.get_by_uuid(session, Uuid.ToString());
                if (xenRef != null)
                    cluster = xenRef.opaque_ref;
            }
            else if (Ref != null)
                cluster = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Cluster', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Cluster));
            }

            return cluster;
        }

        private void ProcessRecordOtherConfig(string cluster)
        {
            if (!ShouldProcess(cluster, "Cluster.add_to_other_config"))
                return;

            RunApiCall(()=>
            {
                    XenAPI.Cluster.add_to_other_config(session, cluster, OtherConfig.Key, OtherConfig.Value);

            });
        }

        #endregion
    }
}
