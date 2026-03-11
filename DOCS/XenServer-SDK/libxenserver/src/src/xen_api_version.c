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


#include "xen/api/xen_api_version.h"

const char *
xen_api_version_to_string(xen_api_version version)
{
    switch (version)
    {
        case xen_api_version_1_1:
            return "1.1";
        case xen_api_version_1_2:
            return "1.2";
        case xen_api_version_1_3:
            return "1.3";
        case xen_api_version_1_4:
            return "1.4";
        case xen_api_version_1_5:
            return "1.5";
        case xen_api_version_1_6:
            return "1.6";
        case xen_api_version_1_7:
            return "1.7";
        case xen_api_version_1_8:
            return "1.8";
        case xen_api_version_1_9:
            return "1.9";
        case xen_api_version_1_10:
            return "1.10";
        case xen_api_version_2_0:
            return "2.0";
        case xen_api_version_2_1:
            return "2.1";
        case xen_api_version_2_2:
            return "2.2";
        case xen_api_version_2_3:
            return "2.3";
        case xen_api_version_2_4:
            return "2.4";
        case xen_api_version_2_5:
            return "2.5";
        case xen_api_version_2_6:
            return "2.6";
        case xen_api_version_2_7:
            return "2.7";
        case xen_api_version_2_8:
            return "2.8";
        case xen_api_version_2_9:
            return "2.9";
        case xen_api_version_2_10:
            return "2.10";
        case xen_api_version_2_11:
            return "2.11";
        case xen_api_version_2_12:
            return "2.12";
        case xen_api_version_2_13:
            return "2.13";
        case xen_api_version_2_14:
            return "2.14";
        case xen_api_version_2_15:
            return "2.15";
        case xen_api_version_2_20:
            return "2.20";
        case xen_api_version_2_21:
            return "2.21";
        default:
            return "Unknown";
    }
}


xen_api_version
xen_api_version_from_int(int64_t major_version, int64_t minor_version)
{
    if (major_version == (int64_t)1 && minor_version == (int64_t)1)
        return xen_api_version_1_1;
    if (major_version == (int64_t)1 && minor_version == (int64_t)2)
        return xen_api_version_1_2;
    if (major_version == (int64_t)1 && minor_version == (int64_t)3)
        return xen_api_version_1_3;
    if (major_version == (int64_t)1 && minor_version == (int64_t)4)
        return xen_api_version_1_4;
    if (major_version == (int64_t)1 && minor_version == (int64_t)5)
        return xen_api_version_1_5;
    if (major_version == (int64_t)1 && minor_version == (int64_t)6)
        return xen_api_version_1_6;
    if (major_version == (int64_t)1 && minor_version == (int64_t)7)
        return xen_api_version_1_7;
    if (major_version == (int64_t)1 && minor_version == (int64_t)8)
        return xen_api_version_1_8;
    if (major_version == (int64_t)1 && minor_version == (int64_t)9)
        return xen_api_version_1_9;
    if (major_version == (int64_t)1 && minor_version == (int64_t)10)
        return xen_api_version_1_10;
    if (major_version == (int64_t)2 && minor_version == (int64_t)0)
        return xen_api_version_2_0;
    if (major_version == (int64_t)2 && minor_version == (int64_t)1)
        return xen_api_version_2_1;
    if (major_version == (int64_t)2 && minor_version == (int64_t)2)
        return xen_api_version_2_2;
    if (major_version == (int64_t)2 && minor_version == (int64_t)3)
        return xen_api_version_2_3;
    if (major_version == (int64_t)2 && minor_version == (int64_t)4)
        return xen_api_version_2_4;
    if (major_version == (int64_t)2 && minor_version == (int64_t)5)
        return xen_api_version_2_5;
    if (major_version == (int64_t)2 && minor_version == (int64_t)6)
        return xen_api_version_2_6;
    if (major_version == (int64_t)2 && minor_version == (int64_t)7)
        return xen_api_version_2_7;
    if (major_version == (int64_t)2 && minor_version == (int64_t)8)
        return xen_api_version_2_8;
    if (major_version == (int64_t)2 && minor_version == (int64_t)9)
        return xen_api_version_2_9;
    if (major_version == (int64_t)2 && minor_version == (int64_t)10)
        return xen_api_version_2_10;
    if (major_version == (int64_t)2 && minor_version == (int64_t)11)
        return xen_api_version_2_11;
    if (major_version == (int64_t)2 && minor_version == (int64_t)12)
        return xen_api_version_2_12;
    if (major_version == (int64_t)2 && minor_version == (int64_t)13)
        return xen_api_version_2_13;
    if (major_version == (int64_t)2 && minor_version == (int64_t)14)
        return xen_api_version_2_14;
    if (major_version == (int64_t)2 && minor_version == (int64_t)15)
        return xen_api_version_2_15;
    if (major_version == (int64_t)2 && minor_version == (int64_t)20)
        return xen_api_version_2_20;
    if (major_version == (int64_t)2 && minor_version == (int64_t)21)
        return xen_api_version_2_21;
    return xen_api_unknown_version;
}
