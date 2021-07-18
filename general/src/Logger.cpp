#include "Logger.hpp"

using namespace Distance;


Logger::Logger(const tstring& path)
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

#pragma warning(push)
#pragma warning(disable : 4996)
    tstringstream ss;
    ss << path << time << _T(".log");
    path_ = ss.str();
#pragma warning(pop)

    file_.open(path_);
    if (!file_.is_open())
        throw std::runtime_error("failed opened file...");
}



Logger& Distance::operator << (Logger& logger, const std::string& str)
{
    logger.write(str);
    return logger;
}



void Logger::write(const std::string& str)
{
    file_ << str;
    file_.flush();
}



void Logger::writeln(const std::string& str)
{
    file_ << str << '\n';
    file_.flush();
}
