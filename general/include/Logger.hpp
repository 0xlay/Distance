#ifndef _DISTANCE_LOGGER_HPP_
#define _DISTANCE_LOGGER_HPP_

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <tchar.h>
#include <stdexcept>
#include <mutex>

namespace Distance
{

#ifdef _UNICODE
    using tstring = std::wstring;
    using tstringstream = std::wstringstream;
    using tstring_view = std::wstring_view;
    using tchar = wchar_t;
#else
    using tstring = std::string;
    using tstringstream = std::stringstream;
    using tstring_view = std::string_view;
    using tchar = char;
#endif

    //--------------------------------------------------------------------------

    struct LoggerStream
    {
        template <typename ... Args>
        void write(Args ... args) 
        {
            std::stringstream ss;
            ((ss << args), ...);
            write_impl(ss.str());
        };
        virtual void flush() = 0;
        virtual bool isOpen() const = 0;
        virtual ~LoggerStream() = default;
    
    protected:
        virtual void write_impl(std::string str) = 0;
    };



    class LoggerConsoleStream : public LoggerStream
    {
    public:
        void flush() override;
        bool isOpen() const override;

    private:
        void write_impl(std::string str) override;
        
    private:
        std::ostream& stream_{ std::cout };
    };



    class LoggerFileStream : public LoggerStream
    {
    public:
        LoggerFileStream(tstring_view path, tstring_view fileName = _T("distance"));
        
        void flush() override;
        bool isOpen() const override;

    private:
        void write_impl(std::string str) override;

    private:
        tstring path_;
        std::ofstream file_;
    };

    tstring PathInTimeFormat(
        tstring_view path, tstring_view fileName, tstring_view timeFormat
    );

    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------

    /**
    * @brief The Logger's a thread safe.
    */
    class Logger
    {
    public:
        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator = (const Logger&) = delete;
        Logger& operator = (Logger&&) = delete;

        static Logger* make();

        void configure(std::unique_ptr<LoggerStream> stream);
        
        template <typename ... Args>
        void write(Args ... args)
        {
            std::lock_guard lock(mutex_);
            write_impl(args...);
            logStream_->flush();
        }

        template <typename ... Args>
        void writeln(Args ... args)
        {
            std::lock_guard lock(mutex_);
            write_impl(args...);
            logStream_->write('\n');
            logStream_->flush();
        }

    private:
        Logger() = default;
        
        ~Logger()
        {
            delete logger_;
        }

        template <typename ... Args>
        void write_impl(Args... args)
        {
            logStream_->write(args...);
        }

    private:
        std::unique_ptr<LoggerStream> logStream_;
        
        static std::mutex mutex_;
        static Logger* logger_;
    };

    //--------------------------------------------------------------------------

} // namespace Distance
#endif // _DISTANCE_LOGGER_HPP_
