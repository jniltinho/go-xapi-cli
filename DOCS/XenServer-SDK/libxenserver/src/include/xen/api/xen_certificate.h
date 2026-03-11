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


#ifndef XEN_CERTIFICATE_H
#define XEN_CERTIFICATE_H

#include <xen/api/xen_certificate_decl.h>
#include <xen/api/xen_certificate_type.h>
#include <xen/api/xen_certificate_xen_certificate_record_map.h>
#include <xen/api/xen_common.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>


/*
 * The Certificate class.
 * 
 * An X509 certificate used for TLS connections.
 */


/**
 * Free the given xen_certificate.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_certificate_free(xen_certificate certificate);


typedef struct xen_certificate_set
{
    size_t size;
    xen_certificate *contents[];
} xen_certificate_set;

/**
 * Allocate a xen_certificate_set of the given size.
 */
extern xen_certificate_set *
xen_certificate_set_alloc(size_t size);

/**
 * Free the given xen_certificate_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_certificate_set_free(xen_certificate_set *set);


typedef struct xen_certificate_record
{
    xen_certificate handle;
    char *uuid;
    char *name;
    enum xen_certificate_type type;
    struct xen_host_record_opt *host;
    time_t not_before;
    time_t not_after;
    char *fingerprint;
    char *fingerprint_sha256;
    char *fingerprint_sha1;
} xen_certificate_record;

/**
 * Allocate a xen_certificate_record.
 */
extern xen_certificate_record *
xen_certificate_record_alloc(void);

/**
 * Free the given xen_certificate_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_certificate_record_free(xen_certificate_record *record);


typedef struct xen_certificate_record_opt
{
    bool is_record;
    union
    {
        xen_certificate handle;
        xen_certificate_record *record;
    } u;
} xen_certificate_record_opt;

/**
 * Allocate a xen_certificate_record_opt.
 */
extern xen_certificate_record_opt *
xen_certificate_record_opt_alloc(void);

/**
 * Free the given xen_certificate_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_certificate_record_opt_free(xen_certificate_record_opt *record_opt);


typedef struct xen_certificate_record_set
{
    size_t size;
    xen_certificate_record *contents[];
} xen_certificate_record_set;

/**
 * Allocate a xen_certificate_record_set of the given size.
 */
extern xen_certificate_record_set *
xen_certificate_record_set_alloc(size_t size);

/**
 * Free the given xen_certificate_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_certificate_record_set_free(xen_certificate_record_set *set);


typedef struct xen_certificate_record_opt_set
{
    size_t size;
    xen_certificate_record_opt *contents[];
} xen_certificate_record_opt_set;

/**
 * Allocate a xen_certificate_record_opt_set of the given size.
 */
extern xen_certificate_record_opt_set *
xen_certificate_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_certificate_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_certificate_record_opt_set_free(xen_certificate_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_record(xen_session *session, xen_certificate_record **result, xen_certificate certificate);


/**
 * Get a reference to the Certificate instance with the specified
 * UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_by_uuid(xen_session *session, xen_certificate *result, char *uuid);


/**
 * Get the uuid field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_uuid(xen_session *session, char **result, xen_certificate certificate);


/**
 * Get the name field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_name(xen_session *session, char **result, xen_certificate certificate);


/**
 * Get the type field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_type(xen_session *session, enum xen_certificate_type *result, xen_certificate certificate);


/**
 * Get the host field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_host(xen_session *session, xen_host *result, xen_certificate certificate);


/**
 * Get the not_before field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_not_before(xen_session *session, time_t *result, xen_certificate certificate);


/**
 * Get the not_after field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_not_after(xen_session *session, time_t *result, xen_certificate certificate);


/**
 * Get the fingerprint field of the given Certificate.
 * Minimum allowed role: read-only.
 * Deprecated since 24.19.0.
 */
extern bool
xen_certificate_get_fingerprint(xen_session *session, char **result, xen_certificate certificate);


/**
 * Get the fingerprint_sha256 field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_fingerprint_sha256(xen_session *session, char **result, xen_certificate certificate);


/**
 * Get the fingerprint_sha1 field of the given Certificate.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_fingerprint_sha1(xen_session *session, char **result, xen_certificate certificate);


/**
 * Return a list of all the Certificates known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_all(xen_session *session, struct xen_certificate_set **result);


/**
 * Return a map of Certificate references to Certificate records for
 * all Certificates known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_certificate_get_all_records(xen_session *session, xen_certificate_xen_certificate_record_map **result);


#endif
