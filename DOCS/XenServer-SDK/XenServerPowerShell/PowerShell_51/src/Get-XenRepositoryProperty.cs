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
    [Cmdlet(VerbsCommon.Get, "XenRepositoryProperty", SupportsShouldProcess = false)]
    public class GetXenRepositoryProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Repository Repository { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Repository> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenRepositoryProperty XenProperty { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string repository = ParseRepository();

            switch (XenProperty)
            {
                case XenRepositoryProperty.Uuid:
                    ProcessRecordUuid(repository);
                    break;
                case XenRepositoryProperty.NameLabel:
                    ProcessRecordNameLabel(repository);
                    break;
                case XenRepositoryProperty.NameDescription:
                    ProcessRecordNameDescription(repository);
                    break;
                case XenRepositoryProperty.BinaryUrl:
                    ProcessRecordBinaryUrl(repository);
                    break;
                case XenRepositoryProperty.SourceUrl:
                    ProcessRecordSourceUrl(repository);
                    break;
                case XenRepositoryProperty.Update:
                    ProcessRecordUpdate(repository);
                    break;
                case XenRepositoryProperty.Hash:
                    ProcessRecordHash(repository);
                    break;
                case XenRepositoryProperty.UpToDate:
                    ProcessRecordUpToDate(repository);
                    break;
                case XenRepositoryProperty.GpgkeyPath:
                    ProcessRecordGpgkeyPath(repository);
                    break;
                case XenRepositoryProperty.Origin:
                    ProcessRecordOrigin(repository);
                    break;
                case XenRepositoryProperty.Certificate:
                    ProcessRecordCertificate(repository);
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

        private void ProcessRecordUuid(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_uuid(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameLabel(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_name_label(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNameDescription(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_name_description(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordBinaryUrl(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_binary_url(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordSourceUrl(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_source_url(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordUpdate(string repository)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Repository.get_update(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHash(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_hash(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordUpToDate(string repository)
        {
            RunApiCall(()=>
            {
                    bool obj = XenAPI.Repository.get_up_to_date(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordGpgkeyPath(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_gpgkey_path(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordOrigin(string repository)
        {
            RunApiCall(()=>
            {
                    origin obj = XenAPI.Repository.get_origin(session, repository);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordCertificate(string repository)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Repository.get_certificate(session, repository);

                        WriteObject(obj, true);
            });
        }

        #endregion
    }

    public enum XenRepositoryProperty
    {
        Uuid,
        NameLabel,
        NameDescription,
        BinaryUrl,
        SourceUrl,
        Update,
        Hash,
        UpToDate,
        GpgkeyPath,
        Origin,
        Certificate
    }

}
