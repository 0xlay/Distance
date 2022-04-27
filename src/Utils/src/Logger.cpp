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
// File: Logger.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Utils.lib
//
//------------------------------------------------------------------------------

#include "Logger.hpp"
#include <filesystem>


namespace fs = std::filesystem;



namespace Distance::Utils
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // The LoggerConsoleStream implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    void LoggerConsoleStream::write_impl(std::wstring str)
    {
        stream_ << str;
    }

    void LoggerConsoleStream::flush()
    {
        stream_.flush();
    }

    bool LoggerConsoleStream::isOpen() const
    {
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The LoggerFileStream implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    LoggerFileStream::LoggerFileStream(
        std::wstring_view path, std::wstring_view fileName
    )
    {
        if (file_.is_open())
            file_.close();

        if (!fs::is_directory(path))
        {
            if (!fs::create_directory(path))
                throw std::runtime_error("Logger was failing to create folder...");
        }

        std::wostringstream fullPath;
        fullPath << path << fileName << CurrentTime(L"%F_%H-%M-%S") << L".log";

        file_.open(fullPath.str(), std::ios::out);
        if (!file_.is_open())
            throw std::runtime_error("Logger was failing to open folder...");
    }

    void LoggerFileStream::write_impl(std::wstring str)
    {
        file_ << str;
    }

    void LoggerFileStream::flush()
    {
        file_.flush();
    }

    bool LoggerFileStream::isOpen() const
    {
        return file_.is_open();
    }

    std::wstring CurrentTime(std::wstring_view timeFormat)
    {
        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::wostringstream ss;

#pragma warning(push)
#pragma warning(disable : 4996)
        ss << std::put_time(std::localtime(&time), timeFormat.data());
#pragma warning(pop)
        return ss.str();
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The Logger implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    namespace _internal
    {

        void LoggerDestructor::init(Logger* logger)
        {
            logger_ = logger;
        }

        LoggerDestructor::~LoggerDestructor()
        {
            delete logger_;
        }

    } // namespace Distance::Utils::_internal

    Logger& Logger::invoke()
    {
        std::lock_guard lock(mutex_);
        if (!logger_)
        {
            logger_ = new Logger();
            destructor_.init(logger_);
        }
        return *logger_;
    }

    void Logger::configure(
        std::unique_ptr<LoggerStream> stream, 
        std::wstring_view timeFormat
    )
    {
        std::lock_guard lock(mutex_);
        logStream_ = std::move(stream);
        timeFormat_ = timeFormat;
    }

} // namespace Distance::Utils
