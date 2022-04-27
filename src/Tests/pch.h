//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include <string>
#include <array>
#include <tuple>
#include <memory>


#pragma comment(lib, "Utils")
#pragma comment(lib, "Network")


template <typename ...Args>
inline void DbgPrint(Args... arg)
{
    ((std::cerr << arg),...);
    std::cerr << std::endl;
}

#define DBG(...) DbgPrint(__VA_ARGS__);
