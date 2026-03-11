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


#ifndef XEN_VM_SECUREBOOT_READINESS_H
#define XEN_VM_SECUREBOOT_READINESS_H


#include <xen/api/xen_common.h>


enum xen_vm_secureboot_readiness
{
    /**
     * VM's firmware is not UEFI
     */
    XEN_VM_SECUREBOOT_READINESS_NOT_SUPPORTED,

    /**
     * Secureboot is disabled on this VM
     */
    XEN_VM_SECUREBOOT_READINESS_DISABLED,

    /**
     * Secured boot is enabled on this VM and its
     * NVRAM.EFI-variables are empty
     */
    XEN_VM_SECUREBOOT_READINESS_FIRST_BOOT,

    /**
     * Secured boot is enabled on this VM and PK, KEK, db and dbx
     * are defined in its EFI variables
     */
    XEN_VM_SECUREBOOT_READINESS_READY,

    /**
     * Secured boot is enabled on this VM and PK, KEK, db but not
     * dbx are defined in its EFI variables
     */
    XEN_VM_SECUREBOOT_READINESS_READY_NO_DBX,

    /**
     * Secured boot is enabled on this VM and PK is not defined in
     * its EFI variables
     */
    XEN_VM_SECUREBOOT_READINESS_SETUP_MODE,

    /**
     * Secured boot is enabled on this VM and the certificates
     * defined in its EFI variables are incomplete
     */
    XEN_VM_SECUREBOOT_READINESS_CERTS_INCOMPLETE,

    /**
     * Unknown to this SDK version.
     */
    XEN_VM_SECUREBOOT_READINESS_UNDEFINED
};


typedef struct xen_vm_secureboot_readiness_set
{
    size_t size;
    enum xen_vm_secureboot_readiness contents[];
} xen_vm_secureboot_readiness_set;

/**
 * Allocate a xen_vm_secureboot_readiness_set of the given size.
 */
extern xen_vm_secureboot_readiness_set *
xen_vm_secureboot_readiness_set_alloc(size_t size);

/**
 * Free the given xen_vm_secureboot_readiness_set.  The given set must
 * have been allocated by this library.
 */
extern void
xen_vm_secureboot_readiness_set_free(xen_vm_secureboot_readiness_set *set);


/**
 * Return the name corresponding to the given code.  This string must
 * not be modified or freed.
 */
extern const char *
xen_vm_secureboot_readiness_to_string(enum xen_vm_secureboot_readiness val);


/**
 * Return the correct code for the given string, or set the session
 * object to failure and return an undefined value if the given string does
 * not match a known code.
 */
extern enum xen_vm_secureboot_readiness
xen_vm_secureboot_readiness_from_string(xen_session *session, const char *str);


#endif
