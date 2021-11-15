#pragma once

#include <tstring.hpp>
// #include <mutex>


namespace Distance::Network
{

    /*
    * The Settings struct is the interface that describes methods, which must
    * implements all settings classes
    */
    struct Settings
    {
        virtual void save(std::fstream& file) = 0;
        virtual void load(std::fstream& file) = 0;
        virtual ~Settings() = default;
    };

    /*
    * @param file - reference to an opened file stream
    * @return true if the file is empty, otherwise false
    */
    [[nodiscard]] inline bool isFileEmpty(std::fstream& file)
    {
        return file.peek() == std::fstream::traits_type::eof();
    }

    class Configurator;
    namespace _internal
    {
        /*
        * @brief The ConfiguratorDestructor class acts as destructor for the
        * Configurator class, which is a singleton
        */
        class ConfiguratorDestructor
        {
        public:
            void init(Configurator* configurator);
            ~ConfiguratorDestructor();

        private:
            Configurator* configurator_;
        };
    }

    /*
    * @brief The Configurator class is the interface for loading and saving settings
    * in the configuration file
    */
    class Configurator
    {
    public:
        Configurator(const Configurator&) = delete;
        Configurator(Configurator&&) = delete;
        Configurator& operator=(const Configurator&) = delete;
        Configurator& operator=(Configurator&&) = delete;

        [[nodiscard]] static Configurator& invoke();
        void open(xstar::tstring_view path);
        void loadSettings(Settings* settings);
        void saveSettings(Settings* settings);

    private:
        Configurator() = default;
        ~Configurator() = default;

    private:
        xstar::tstring path_;
        std::fstream file_;

        static inline std::mutex mutex_;
        static inline Configurator* configurator_{};

        friend class _internal::ConfiguratorDestructor;
        static inline _internal::ConfiguratorDestructor destructor_;
    };

} // Distance::Network
