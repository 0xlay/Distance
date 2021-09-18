#include "Logger.hpp"

namespace fs = std::filesystem;

namespace Distance
{

    Logger* Logger::logger_{};
    std::mutex Logger::mutex_{};

    Logger* Logger::make()
    {
        std::lock_guard lock(mutex_);

        if (!logger_)
            logger_ = new Logger();

        return logger_;
    }

    void Logger::configure(const tstring& path, const tstring& fileName)
    {
        std::lock_guard lock(mutex_);
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



    tstring PathInTimeFormat(const tstring& path, const tstring& fileName, const tchar* timeFormat)
    {
        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);

#pragma warning(push)
#pragma warning(disable : 4996)
        tstringstream ss;
        ss << path << fileName << std::put_time(std::localtime(&time), timeFormat) << _T(".log");
        return ss.str();
#pragma warning(pop)
    }

} // namespace Distance
