#include "Logger.hpp"

namespace fs = std::filesystem;

namespace Distance
{

    //--------------------------------------------------------------------------
    // The LoggerConsoleStream implementation
    //--------------------------------------------------------------------------

    void LoggerConsoleStream::write_impl(std::string str)
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

    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    // The LoggerFileStream implementation
    //--------------------------------------------------------------------------

    LoggerFileStream::LoggerFileStream(tstring_view path, tstring_view fileName)
    {
        if (file_.is_open())
            file_.close();

        if (!fs::is_directory(path))
        {
            if (!fs::create_directory(path))
                throw std::runtime_error("Logger was failing to create folder...");
        }

        path_ = PathInTimeFormat(path, fileName, _T("%F_%H-%M-%S"));

        file_.open(path_, std::ios::out);
        if (!file_.is_open())
            throw std::runtime_error("Logger was failing to open folder...");
    }

    void LoggerFileStream::write_impl(std::string str)
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

    tstring PathInTimeFormat(
        tstring_view path, tstring_view fileName, tstring_view timeFormat
    )
    {
        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);

#pragma warning(push)
#pragma warning(disable : 4996)
        tstringstream ss;
        ss << path << fileName 
        << std::put_time(std::localtime(&time), timeFormat.data()) << _T(".log");
        return ss.str();
#pragma warning(pop)
    }

    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    // The Logger implementation
    //--------------------------------------------------------------------------

    Logger* Logger::logger_{};
    std::mutex Logger::mutex_{};

    Logger* Logger::make()
    {
        std::lock_guard lock(mutex_);

        if (!logger_)
            logger_ = new Logger();

        return logger_;
    }

    void Logger::configure(std::unique_ptr<LoggerStream> stream)
    {
        std::lock_guard lock(mutex_);
        logStream_ = std::move(stream);
    }

    //--------------------------------------------------------------------------

} // namespace Distance
