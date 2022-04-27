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
// File: Logger.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Utils.lib
//
//------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>
#include <mutex>
#include <cassert>
#include "Convert.hpp"


namespace Distance::Utils
{
    /*
    * @brief The CurrentTime function gets the current time
    *
    * @param[in] timeFormat - time format (https://en.cppreference.com/w/cpp/io/manip/put_time)
    *
    * @return time as string
    */
    [[nodiscard]] std::wstring CurrentTime(std::wstring_view timeFormat);



    namespace _internal
    {

        /*
        * @brief The IfStringThenConvertToWString function converts to wstring if value is string
        * 
        * @param[in] value - value which needs to converts. It has random type
        * 
        * @return string which was converted to wstring
        */
        template <typename T>
        [[nodiscard]] std::wstring IfStringThenConvertToWString(T value)
        {
            if constexpr 
            (std::is_same<std::string, T>::value || std::is_same<const char*, T>::value)
            {
                return StringToWString(value);
            }
            else if constexpr
            (std::is_same<std::wstring, T>::value || std::is_same<const wchar_t*, T>::value)
            {
                return value; // copy-elision optimization
            }
            else if constexpr
            (std::is_same<char, T>::value || std::is_same<wchar_t, T>::value)
            {
                return std::wstring(1, static_cast<wchar_t>(value));
            }
            else
            {
                return std::to_wstring(value);
            }
        }

    } // Distance::Utils::_internal



    /*
    * @brief The LoggerStream structure is the interface, which must implement each, 
    * who want use the Logger
    */
    struct LoggerStream
    {
        template <typename ... Args>
        void write(Args&& ... args)
        {
            using namespace _internal;
            std::wostringstream wss;
            ((wss << IfStringThenConvertToWString(std::move(args))), ...);
            write_impl(wss.str());
        };
        virtual void flush() = 0;
        virtual bool isOpen() const = 0;
        virtual ~LoggerStream() = default;

    protected:
        virtual void write_impl(std::wstring str) = 0;
    };



    /*
    * @brief The LoggerConsoleStream class implements console stream for the Logger
    */
    class LoggerConsoleStream : public LoggerStream
    {
    public:
        void flush() override;
        [[nodiscard]] bool isOpen() const override;

    private:
        void write_impl(std::wstring str) override;

    private:
        std::wostream& stream_{ std::wcout };
    };

    /*
    * @brief The LoggerFileStream class implements file stream for the Logger
    */
    class LoggerFileStream : public LoggerStream
    {
    public:
        LoggerFileStream(
            std::wstring_view path, std::wstring_view fileName = L"distance"
        );

        void flush() override;
        [[nodiscard]] bool isOpen() const override;

    private:
        void write_impl(std::wstring str) override;

    private:
        std::wstring path_;
        std::wofstream file_;
    };



    class Logger;
    namespace _internal
    {
        class LoggerDestructor
        {
        public:
            void init(Logger* logger);
            ~LoggerDestructor();

        private:
            Logger* logger_{};
        };
    }

    /*
    * @brief The Logger class records errors that occur to a file. He is a thread safe.
    */
    class Logger
    {
    public:
        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator = (const Logger&) = delete;
        Logger& operator = (Logger&&) = delete;

        [[nodiscard]] static Logger& invoke();

        void configure(
            std::unique_ptr<LoggerStream> stream, 
            std::wstring_view timeFormat = L"%c %Z"
        );

        template <typename ... Args>
        void write(Args&& ... args)
        {
            std::lock_guard lock(mutex_);

            std::wostringstream wss;
            wss << "[" << CurrentTime(timeFormat_) << "]: ";

            write_impl(wss.str(), args...);

            logStream_->flush();
        }

        template <typename ... Args>
        void writeln(Args&& ... args)
        {
            write(args..., '\n');
        }

    private:
        Logger() = default;
        ~Logger() = default;

        template <typename ... Args>
        void write_impl(Args&&... args)
        {
            logStream_->write(args...);
        }

    private:
        std::unique_ptr<LoggerStream> logStream_;
        std::wstring timeFormat_;

        static inline Logger* logger_{};
        static inline std::mutex mutex_;

        friend class _internal::LoggerDestructor;
        static inline _internal::LoggerDestructor destructor_;
    };

    template <typename ... Args>
    void LOG_INFO(Args ... args)
    {
        Logger::invoke().writeln(args...);
    }

#ifdef _DEBUG
    template <typename ... Args>
    void LOG_DEBUG_INFO(Args ... args)
    {
        Logger::invoke().writeln(args...);
    }
#else
    #define LOG_DEBUG_INFO(...)
#endif

} // namespace Distance::Utils