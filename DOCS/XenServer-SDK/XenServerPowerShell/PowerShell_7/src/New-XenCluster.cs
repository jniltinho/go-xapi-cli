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
    [Cmdlet(VerbsCommon.New, "XenCluster", DefaultParameterSetName = "Hashtable", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.Cluster))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class NewXenClusterCommand : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "Hashtable", Mandatory = true)]
        public Hashtable HashTable { get; set; }

        [Parameter(ParameterSetName = "Record", Mandatory = true)]
        public XenAPI.Cluster Record { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.PIF> PIF { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string ClusterStack { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool PoolAutoJoin { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public double TokenTimeout { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public double TokenTimeoutCoefficient { get; set; }

        protected override bool GenerateAsyncParam
        {
            get { return true; }
        }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();
            if (Record != null)
            {
                ClusterStack = Record.cluster_stack;
                PoolAutoJoin = Record.pool_auto_join;
                TokenTimeout = Record.token_timeout;
                TokenTimeoutCoefficient = Record.token_timeout_coefficient;
            }
            else if (HashTable != null)
            {
                ClusterStack = Marshalling.ParseString(HashTable, "cluster_stack");
                PoolAutoJoin = Marshalling.ParseBool(HashTable, "pool_auto_join");
                TokenTimeout = Marshalling.ParseDouble(HashTable, "token_timeout");
                TokenTimeoutCoefficient = Marshalling.ParseDouble(HashTable, "token_timeout_coefficient");
            }
            if (!ShouldProcess(session.Url, "Cluster.create"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.Cluster.async_create(session, PIF, ClusterStack, PoolAutoJoin, TokenTimeout, TokenTimeoutCoefficient);

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
                    string objRef = XenAPI.Cluster.create(session, PIF, ClusterStack, PoolAutoJoin, TokenTimeout, TokenTimeoutCoefficient);

                    if (PassThru)
                    {
                        XenAPI.Cluster obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Cluster.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                    }
                }

            });

            UpdateSessions();
        }

        #endregion
   }
}
