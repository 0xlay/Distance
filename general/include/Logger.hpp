#ifndef _DISTANCE_LOGGER_HPP_
#define _DISTANCE_LOGGER_HPP_

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
    using tchar = wchar_t;
#else
    using tstring = std::string;
    using tstringstream = std::stringstream;
    using tchar = char;
#endif

    class Logger
    {
    public:
        static Logger* make();

        void configure(const tstring& path, const tstring& fileName = _T("distance"));
        
        template <typename ... Args>
        void write(Args ... args)
        {
            std::lock_guard lock(mutex_);
            write_impl(args...);
            file_.flush();
        }

        template <typename ... Args>
        void writeln(Args ... args)
        {
            std::lock_guard lock(mutex_);
            write_impl(args...);
            file_ << std::endl;
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
            ((file_ << args), ...);
        }

    private:
        std::ofstream file_;
        tstring path_;
        
        static std::mutex mutex_;
        static Logger* logger_;
    };

    tstring PathInTimeFormat(const tstring& path, const tstring& fileName, const tchar* timeFormat);

} // namespace Distance
#endif // _DISTANCE_LOGGER_HPP_
