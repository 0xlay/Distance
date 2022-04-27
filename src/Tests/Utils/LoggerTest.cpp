#include "pch.h"
#include "Utils/include/Logger.hpp"
#include "mock/MockLoggerStream.hpp"

using namespace Distance::Utils;

using testing::_;
using testing::Return;

TEST(LoggerTest, WriteToLoggerStream)
{
    auto mockLoggerStream = std::make_unique<MockLoggerStream>();

    std::wostringstream wss;
    wss << "[" << CurrentTime(L"%c %Z") << "]: hello";

    EXPECT_CALL(*mockLoggerStream, write_impl(wss.str()))
        .Times(1)
        .WillOnce(Return());

    EXPECT_CALL(*mockLoggerStream, isOpen())
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(*mockLoggerStream, flush())
        .Times(1)
        .WillOnce(Return());

    Logger::invoke().configure(std::move(mockLoggerStream));

    EXPECT_NO_THROW(
        Logger::invoke().write("hello")
    );
}
