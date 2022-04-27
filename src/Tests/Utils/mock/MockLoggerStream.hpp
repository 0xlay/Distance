#pragma once
#include "pch.h"
#include "Utils/include/Logger.hpp"

class MockLoggerStream : public Distance::Utils::LoggerStream
{
public:
    // MOCK_METHOD(void, writeString, (const std::string& str, std::string& out));
    MOCK_METHOD(void, write_impl, (std::wstring str), (override));
    MOCK_METHOD(void, flush, (), (override));
    MOCK_METHOD(bool, isOpen, (), (const, override));
};