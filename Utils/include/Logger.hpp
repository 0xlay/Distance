#pragma once

#include <fstream>
#include <mutex>
#include "tstring.hpp"


namespace Distance::Utils
{
    /*
    * @brief The PathInTimeFormat function adds the current time and ".log" extension to file name
    * 
    * @param[in] path - path to folder, which store log files
    * @param[in] fileName - log file name
    * @param[in] timeFormat - time format (https://en.cppreference.com/w/cpp/io/manip/put_time)
    * 
    * @return path to log file
    */
    [[nodiscard]] xstar::tstring PathInTimeFormat(
        xstar::tstring_view path, xstar::tstring_view fileName, xstar::tstring_view timeFormat
    );

    /*
    * @brief The CurrentTime function gets the current time
    *
    * @param[in] timeFormat - time format (https://en.cppreference.com/w/cpp/io/manip/put_time)
    *
    * @return time as string
    */
    [[nodiscard]] std::string CurrentTime(std::string_view timeFormat);
    [[nodiscard]] std::wstring CurrentTime(std::wstring_view timeFormat);



    /*
    * @brief The LoggerStream structure is the interface, which must implement each, 
    * who want use the Logger
    */
    struct LoggerStream
    {
        template <typename ... Args>
        void write(Args&& ... args)
        {
            std::stringstream ss;
            (ss << ... << args);
            write_impl(ss.str());
        };
        virtual void flush() = 0;
        virtual bool isOpen() const = 0;
        virtual ~LoggerStream() = default;

    protected:
        virtual void write_impl(std::string str) = 0;
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
        void write_impl(std::string str) override;

    private:
        std::ostream& stream_{ std::cout };
    };

    /*
    * @brief The LoggerFileStream class implements file stream for the Logger
    */
    class LoggerFileStream : public LoggerStream
    {
    public:
        LoggerFileStream(xstar::tstring_view path, xstar::tstring_view fileName = $T"distance");

        void flush() override;
        [[nodiscard]] bool isOpen() const override;

    private:
        void write_impl(std::string str) override;

    private:
        xstar::tstring path_;
        std::ofstream file_;
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

        void configure(std::unique_ptr<LoggerStream> stream);

        template <typename ... Args>
        void write(Args&& ... args)
        {
            std::lock_guard lock(mutex_);

            std::stringstream ss;
            ss << "[" << CurrentTime("%c %Z") << "]: ";
            logStream_->write(ss.str());

            write_impl(args...);

            logStream_->flush();
        }

        template <typename ... Args>
        void writeln(Args&& ... args)
        {
            std::lock_guard lock(mutex_);

            std::stringstream ss;
            ss << "[" << CurrentTime("%c %Z") << "]: ";
            logStream_->write(ss.str());

            write_impl(args...);

            logStream_->write('\n');
            logStream_->flush();
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

        static inline Logger* logger_{};
        static inline std::mutex mutex_;

        friend class _internal::LoggerDestructor;
        static inline _internal::LoggerDestructor destructor_;
    };



#define LOG_INFO(...) Distance::Utils::Logger::invoke().writeln(__VA_ARGS__);

} // namespace Distance::Utils