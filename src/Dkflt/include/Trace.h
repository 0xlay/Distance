//------------------------------------------------------------------------------
// ╔═══╗╔══╗╔═══╗╔════╗╔═══╗╔═╗─╔╗╔═══╗╔═══╗
// ╚╗╔╗║╚╣─╝║╔═╗║║╔╗╔╗║║╔═╗║║║╚╗║║║╔═╗║║╔══╝
// ─║║║║─║║─║╚══╗╚╝║║╚╝║║─║║║╔╗╚╝║║║─╚╝║╚══╗
// ─║║║║─║║─╚══╗║──║║──║╚═╝║║║╚╗║║║║─╔╗║╔══╝
// ╔╝╚╝║╔╣─╗║╚═╝║──║║──║╔═╗║║║─║║║║╚═╝║║╚══╗
// ╚═══╝╚══╝╚═══╝──╚╝──╚╝─╚╝╚╝─╚═╝╚═══╝╚═══╝
// 
// Copyright Ⓒ 0xlay. All rights reserved.
// 
// This source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
// 
// File: Trace.hpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------

#pragma once

#define WPP_CONTROL_GUIDS \
    WPP_DEFINE_CONTROL_GUID(DkfltTraceGUID, \
        (B09BB75B, 08FD, 4133, ADC0, C70043F84152),               \
        WPP_DEFINE_BIT(TRACE_DRIVER)   /* bit  0 = 0x00000001 */ \
        WPP_DEFINE_BIT(TRACE_DEVICE)   /* bit  1 = 0x00000002 */ \
    )


#define WPP_LEVEL_FLAGS_LOGGER(lvl,flags) \
           WPP_LEVEL_LOGGER(flags)

#define WPP_LEVEL_FLAGS_ENABLED(lvl, flags) \
           (WPP_LEVEL_ENABLED(flags) && WPP_CONTROL(WPP_BIT_ ## flags).Level >= lvl)


//
// This comment block is scanned by the trace preprocessor to define the 
// TraceEvents function.
//
// begin_wpp config
//
// FUNC TraceEvents(LEVEL, FLAGS, MSG, ...);
//
// FUNC TraceDriverInfo{LEVEL=TRACE_LEVEL_INFORMATION, FLAGS=TRACE_DRIVER}(MSG, ...);
//
// FUNC TraceDriverError{LEVEL=TRACE_LEVEL_ERROR, FLAGS=TRACE_DRIVER}(MSG, ...);
//
// FUNC TraceDeviceInfo{LEVEL=TRACE_LEVEL_INFORMATION, FLAGS=TRACE_DEVICE}(MSG, ...);
//
// FUNC TraceDeviceError{LEVEL=TRACE_LEVEL_ERROR, FLAGS=TRACE_DEVICE}(MSG, ...);
//
// end_wpp
//
