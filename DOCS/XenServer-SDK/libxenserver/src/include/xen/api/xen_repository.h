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


#ifndef XEN_REPOSITORY_H
#define XEN_REPOSITORY_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_origin.h>
#include <xen/api/xen_repository_decl.h>
#include <xen/api/xen_repository_xen_repository_record_map.h>
#include <xen/api/xen_task_decl.h>


/*
 * The Repository class.
 * 
 * Repository for updates.
 */


/**
 * Free the given xen_repository.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_repository_free(xen_repository repository);


typedef struct xen_repository_set
{
    size_t size;
    xen_repository *contents[];
} xen_repository_set;

/**
 * Allocate a xen_repository_set of the given size.
 */
extern xen_repository_set *
xen_repository_set_alloc(size_t size);

/**
 * Free the given xen_repository_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_repository_set_free(xen_repository_set *set);


typedef struct xen_repository_record
{
    xen_repository handle;
    char *uuid;
    char *name_label;
    char *name_description;
    char *binary_url;
    char *source_url;
    bool update;
    char *hash;
    bool up_to_date;
    char *gpgkey_path;
    enum xen_origin origin;
    char *certificate;
} xen_repository_record;

/**
 * Allocate a xen_repository_record.
 */
extern xen_repository_record *
xen_repository_record_alloc(void);

/**
 * Free the given xen_repository_record, and all referenced values.
 * The given record must have been allocated by this library.
 */
extern void
xen_repository_record_free(xen_repository_record *record);


typedef struct xen_repository_record_opt
{
    bool is_record;
    union
    {
        xen_repository handle;
        xen_repository_record *record;
    } u;
} xen_repository_record_opt;

/**
 * Allocate a xen_repository_record_opt.
 */
extern xen_repository_record_opt *
xen_repository_record_opt_alloc(void);

/**
 * Free the given xen_repository_record_opt, and all referenced values.
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_repository_record_opt_free(xen_repository_record_opt *record_opt);


typedef struct xen_repository_record_set
{
    size_t size;
    xen_repository_record *contents[];
} xen_repository_record_set;

/**
 * Allocate a xen_repository_record_set of the given size.
 */
extern xen_repository_record_set *
xen_repository_record_set_alloc(size_t size);

/**
 * Free the given xen_repository_record_set, and all referenced values.
 * The given set must have been allocated by this library.
 */
extern void
xen_repository_record_set_free(xen_repository_record_set *set);


typedef struct xen_repository_record_opt_set
{
    size_t size;
    xen_repository_record_opt *contents[];
} xen_repository_record_opt_set;

/**
 * Allocate a xen_repository_record_opt_set of the given size.
 */
extern xen_repository_record_opt_set *
xen_repository_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_repository_record_opt_set, and all referenced
 * values. The given set must have been allocated by this library.
 */
extern void
xen_repository_record_opt_set_free(xen_repository_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_record(xen_session *session, xen_repository_record **result, xen_repository repository);


/**
 * Get a reference to the Repository instance with the specified
 * UUID.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_by_uuid(xen_session *session, xen_repository *result, char *uuid);


/**
 * Get all the Repository instances with the given label.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_by_name_label(xen_session *session, struct xen_repository_set **result, char *label);


/**
 * Get the uuid field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_uuid(xen_session *session, char **result, xen_repository repository);


/**
 * Get the name/label field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_name_label(xen_session *session, char **result, xen_repository repository);


/**
 * Get the name/description field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_name_description(xen_session *session, char **result, xen_repository repository);


/**
 * Get the binary_url field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_binary_url(xen_session *session, char **result, xen_repository repository);


/**
 * Get the source_url field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_source_url(xen_session *session, char **result, xen_repository repository);


/**
 * Get the update field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_update(xen_session *session, bool *result, xen_repository repository);


/**
 * Get the hash field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_hash(xen_session *session, char **result, xen_repository repository);


/**
 * Get the up_to_date field of the given Repository.
 * Minimum allowed role: read-only.
 * Deprecated since 23.18.0.
 */
extern bool
xen_repository_get_up_to_date(xen_session *session, bool *result, xen_repository repository);


/**
 * Get the gpgkey_path field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_gpgkey_path(xen_session *session, char **result, xen_repository repository);


/**
 * Get the origin field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_origin(xen_session *session, enum xen_origin *result, xen_repository repository);


/**
 * Get the certificate field of the given Repository.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_certificate(xen_session *session, char **result, xen_repository repository);


/**
 * Set the name/label field of the given Repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_set_name_label(xen_session *session, xen_repository repository, char *label);


/**
 * Set the name/description field of the given Repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_set_name_description(xen_session *session, xen_repository repository, char *description);


/**
 * Add the configuration for a new remote repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_introduce(xen_session *session, xen_repository *result, char *name_label, char *name_description, char *binary_url, char *source_url, bool update, char *gpgkey_path);


/**
 * Add the configuration for a new remote repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_introduce_async(xen_session *session, xen_task *result, char *name_label, char *name_description, char *binary_url, char *source_url, bool update, char *gpgkey_path);


/**
 * Add the configuration for a new bundle repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_introduce_bundle(xen_session *session, xen_repository *result, char *name_label, char *name_description);


/**
 * Add the configuration for a new bundle repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_introduce_bundle_async(xen_session *session, xen_task *result, char *name_label, char *name_description);


/**
 * Add the configuration for a new remote pool repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_introduce_remote_pool(xen_session *session, xen_repository *result, char *name_label, char *name_description, char *binary_url, char *certificate);


/**
 * Add the configuration for a new remote pool repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_introduce_remote_pool_async(xen_session *session, xen_task *result, char *name_label, char *name_description, char *binary_url, char *certificate);


/**
 * Remove the repository record from the database.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_forget(xen_session *session, xen_repository self);


/**
 * Remove the repository record from the database.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_forget_async(xen_session *session, xen_task *result, xen_repository self);


/**
 * Set the file name of the GPG public key of the repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_set_gpgkey_path(xen_session *session, xen_repository self, char *value);


/**
 * Set the file name of the GPG public key of the repository.
 * Minimum allowed role: pool-operator.
 */
extern bool
xen_repository_set_gpgkey_path_async(xen_session *session, xen_task *result, xen_repository self, char *value);


/**
 * Return a list of all the Repositorys known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_all(xen_session *session, struct xen_repository_set **result);


/**
 * Return a map of Repository references to Repository records for all
 * Repositorys known to the system.
 * Minimum allowed role: read-only.
 */
extern bool
xen_repository_get_all_records(xen_session *session, xen_repository_xen_repository_record_map **result);


#endif
