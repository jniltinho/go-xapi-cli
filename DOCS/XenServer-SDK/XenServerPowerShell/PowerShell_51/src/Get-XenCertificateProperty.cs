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
    [Cmdlet(VerbsCommon.Get, "XenCertificateProperty", SupportsShouldProcess = false)]
    public class GetXenCertificateProperty : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(ParameterSetName = "XenObject", Mandatory = true, ValueFromPipeline = true, Position = 0)]
        public XenAPI.Certificate Certificate { get; set; }

        [Parameter(ParameterSetName = "Ref", Mandatory = true, ValueFromPipelineByPropertyName = true, Position = 0)]
        [Alias("opaque_ref")]
        public XenRef<XenAPI.Certificate> Ref { get; set; }


        [Parameter(Mandatory = true)]
        public XenCertificateProperty XenProperty { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();

            string certificate = ParseCertificate();

            switch (XenProperty)
            {
                case XenCertificateProperty.Uuid:
                    ProcessRecordUuid(certificate);
                    break;
                case XenCertificateProperty.Name:
                    ProcessRecordName(certificate);
                    break;
                case XenCertificateProperty.Type:
                    ProcessRecordType(certificate);
                    break;
                case XenCertificateProperty.Host:
                    ProcessRecordHost(certificate);
                    break;
                case XenCertificateProperty.NotBefore:
                    ProcessRecordNotBefore(certificate);
                    break;
                case XenCertificateProperty.NotAfter:
                    ProcessRecordNotAfter(certificate);
                    break;
                case XenCertificateProperty.Fingerprint:
                    ProcessRecordFingerprint(certificate);
                    break;
                case XenCertificateProperty.FingerprintSha256:
                    ProcessRecordFingerprintSha256(certificate);
                    break;
                case XenCertificateProperty.FingerprintSha1:
                    ProcessRecordFingerprintSha1(certificate);
                    break;
            }

            UpdateSessions();
        }

        #endregion

        #region Private Methods

        private string ParseCertificate()
        {
            string certificate = null;

            if (Certificate != null)
                certificate = (new XenRef<XenAPI.Certificate>(Certificate)).opaque_ref;
            else if (Ref != null)
                certificate = Ref.opaque_ref;
            else
            {
                ThrowTerminatingError(new ErrorRecord(
                    new ArgumentException("At least one of the parameters 'Certificate', 'Ref', 'Uuid' must be set"),
                    string.Empty,
                    ErrorCategory.InvalidArgument,
                    Certificate));
            }

            return certificate;
        }

        private void ProcessRecordUuid(string certificate)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Certificate.get_uuid(session, certificate);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordName(string certificate)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Certificate.get_name(session, certificate);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordType(string certificate)
        {
            RunApiCall(()=>
            {
                    certificate_type obj = XenAPI.Certificate.get_type(session, certificate);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordHost(string certificate)
        {
            RunApiCall(()=>
            {
                    string objRef = XenAPI.Certificate.get_host(session, certificate);

                        XenAPI.Host obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.Host.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNotBefore(string certificate)
        {
            RunApiCall(()=>
            {
                    DateTime obj = XenAPI.Certificate.get_not_before(session, certificate);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordNotAfter(string certificate)
        {
            RunApiCall(()=>
            {
                    DateTime obj = XenAPI.Certificate.get_not_after(session, certificate);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordFingerprint(string certificate)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Certificate.get_fingerprint(session, certificate);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordFingerprintSha256(string certificate)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Certificate.get_fingerprint_sha256(session, certificate);

                        WriteObject(obj, true);
            });
        }

        private void ProcessRecordFingerprintSha1(string certificate)
        {
            RunApiCall(()=>
            {
                    string obj = XenAPI.Certificate.get_fingerprint_sha1(session, certificate);

                        WriteObject(obj, true);
            });
        }

        #endregion
    }

    public enum XenCertificateProperty
    {
        Uuid,
        Name,
        Type,
        Host,
        NotBefore,
        NotAfter,
        Fingerprint,
        FingerprintSha256,
        FingerprintSha1
    }

}
