#ifndef _DISTANCE_COMPUTER_INFO_HPP_
#define _DISTANCE_COMPUTER_INFO_HPP_


#include <boost/json.hpp>
#include <Windows.h>
#include <string>
#include "winapi_exception.hpp"


namespace Distance
{

    struct RAM
    {
        DWORD MemoryLoad;      /*percent*/
        DWORDLONG TotalMemory; /*bytes*/
        DWORDLONG AvailMemory; /*bytes*/
    };

    struct CPU
    {
        std::string architecture;
        std::string name;
    };



    class ComputerInfo
    {
    public:
        ComputerInfo();
        ComputerInfo(
            const char* localName,
            const char* ip,
            const RAM& ram,
            const CPU& cpu
        );

        ComputerInfo(ComputerInfo&& other) noexcept;
        ComputerInfo& operator = (ComputerInfo&& other) noexcept;

        void updateRAM();
        void updateCPU();
        void updateLocalName();
        void updateIP();
        void updateInfo();

        [[nodiscard]] RAM getMemory() const;
        [[nodiscard]] CPU getCPU() const;
        [[nodiscard]] const char* getName() const;
        [[nodiscard]] const char* getIP() const;
        
        std::string packJson();
        ComputerInfo& UnpackJson(const std::string& info);

    private:
        RAM ram_;
        CPU cpu_;
        std::string localName_;
        std::string ip_;

        const std::string_view subKey = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
    };

} // Distance

#endif // _DISTANCE_COMPUTER_INFO_HPP_