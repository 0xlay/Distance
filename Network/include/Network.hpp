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


//
// Boost library
//

#include <boost/asio.hpp>
#include <boost/json.hpp>


//
// XStar library
//

#include "tstring.hpp"
#pragma comment(lib, "user_space")