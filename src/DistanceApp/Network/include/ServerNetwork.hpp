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
// File: ServerNetwork.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#pragma once


//
//	Win API
//

#include <SDKDDKVer.h> // need _WIN32_WINNT for BOOST


//
// Standard library
//

#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <fstream>
#include <mutex>
#include <functional>

//
// Boost library
//

#include <boost/asio.hpp>
#include <boost/json.hpp>


//
// XStar library
//

#include "tstring.hpp"