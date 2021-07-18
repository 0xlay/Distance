#ifndef _DISTANCE_LOGGER_HPP_
#define _DISTANCE_LOGGER_HPP_

#include <fstream>
#include <chrono>
#include <tchar.h>
#include <stdexcept>

namespace Distance
{

#ifdef _UNICODE
    using tstring = std::wstring;
    using tstringstream = std::wstringstream;
#else
    using tstring = std::string;
    using tstringstream = std::stringstream;
#endif

    class Logger
    {
    public:
        Logger(const tstring& path);

        void write(const std::string& str);
        void writeln(const std::string& str);

    private:
        std::ofstream file_;
        tstring path_;
    };

    Logger& operator << (Logger& logger, const std::string& str);

} // namespace Distance
#endif // _DISTANCE_LOGGER_HPP_
