/**
 * @file ipinfoid.h
 * Copyright 2012, 2013 MinGW.org project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef _IPINFOID_H
#define _IPINFOID_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Routing and Remote Access Services
 */

/*--- Router Management Reference - Router Information Enumeration Types */
#define IP_IN_FILTER_INFO 0xFFFF0001
#define IP_OUT_FILTER_INFO 0xFFFF0002
#define IP_GLOBAL_INFO 0xFFFF0003
#define IP_INTERFACE_STATUS_INFO 0xFFFF0004
#define IP_ROUTE_INFO 0xFFFF0005
#define IP_PROT_PRIORITY_INFO 0xFFFF0006
#define IP_ROUTER_DISC_INFO 0xFFFF0007
#define IP_DEMAND_DIAL_FILTER_INFO 0xFFFF0009
#define IP_MCAST_HEARBEAT_INFO 0xFFFF000A
#define IP_MCAST_BOUNDARY_INFO 0xFFFF000B
#define IP_IPINIP_CFG_INFO 0xFFFF000C
#define IP_IFFILTER_INFO 0xFFFF000D
#define IP_MCAST_LIMIT_INFO 0xFFFF000E
/*#define IP_DEMAND_DIAL_FILTER_INFO_V6 */
/*#define IP_FILTER_ENABLE_INFO_V6 */
/*#define IP_IN_FILTER_INFO_V6 */
/*#define IP_IFFILTER_INFO_V6 */
/*#define IP_OUT_FILTER_INFO_V6 */
/*#define IPV6_GLOBAL_INFO */
/*#define IPV6_ROUTE_INFO */

#endif
