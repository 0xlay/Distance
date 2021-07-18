#include "ComputerInfo.hpp"

using namespace boost::json;

namespace Distance
{
    template <class T>
    void extract(const object& obj, T& t, string_view key);

    void tag_invoke(value_from_tag, value& val, const ComputerInfo& info);

    ComputerInfo tag_invoke(value_to_tag<ComputerInfo>, const value& val);



    ComputerInfo::ComputerInfo()
        : ram_{ 0 }
    { }



    ComputerInfo::ComputerInfo(
        const char* localName,
        const char* ip,
        const RAM& ram, const CPU& cpu
    )
        : ram_(ram), cpu_(cpu), localName_(localName), ip_(ip)
    { }



    ComputerInfo& ComputerInfo::operator=(ComputerInfo&& other) noexcept
    {
        std::swap(this->ram_, other.ram_);
        std::swap(this->cpu_, other.cpu_);
        std::swap(this->localName_, other.localName_);
        std::swap(this->ip_, other.ip_);
        return *this;
    }



    ComputerInfo::ComputerInfo(ComputerInfo&& other) noexcept
        : ram_(other.ram_), cpu_(other.cpu_), localName_(other.localName_), ip_(other.ip_)
    {}



    void ComputerInfo::updateRAM()
    {
        MEMORYSTATUSEX mem{ 0 };
        mem.dwLength = sizeof(mem);

        if (!GlobalMemoryStatusEx(&mem))
        {
            throw xstar::WinAPIException("GlobalMemoryStatusEx", GetLastError());
        }

        ram_ = {
            mem.dwMemoryLoad,
            mem.ullTotalPhys,
            mem.ullAvailPhys
        };
    }



    void ComputerInfo::updateCPU()
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        switch (sysInfo.wProcessorArchitecture)
        {
        case PROCESSOR_ARCHITECTURE_AMD64:
            cpu_.architecture = "x64";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            cpu_.architecture = "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            cpu_.architecture = "ARM64";
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            cpu_.architecture = "IA64";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            cpu_.architecture = "x86";
            break;
        default:
            cpu_.architecture = "UNKNOWN";
            break;
        }

        HKEY key;
        auto status = RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            subKey.data(),
            0,
            KEY_QUERY_VALUE,
            &key
        );
        if (status != ERROR_SUCCESS)
        {
            throw xstar::WinAPIException("RegOpenKeyExA", status);
        }

        DWORD bufSize = MAX_PATH;
        char buf[MAX_PATH + 1]{0};
        status = RegGetValueA(
            key,
            nullptr,
            "ProcessorNameString",
            RRF_RT_REG_SZ,
            nullptr,
            buf,
            &bufSize
        );
        if (status != ERROR_SUCCESS)
        {
            RegCloseKey(key);
            throw xstar::WinAPIException("RegGetValueA", status);
        }

        cpu_.name = buf;
        RegCloseKey(key);
    }



    void ComputerInfo::updateLocalName()
    {
        char buf[MAX_COMPUTERNAME_LENGTH + 2]{ 0 };
        if (gethostname(buf, MAX_COMPUTERNAME_LENGTH + 1))
        {
            throw xstar::WinAPIException("gethostname", WSAGetLastError());
        }
        localName_ = buf;
    }



    void ComputerInfo::updateIP()
    {
        if (localName_.empty())
            updateLocalName();

        const auto info = gethostbyname(localName_.c_str());
        if (!info)
        {
            throw xstar::WinAPIException("gethostbyname", WSAGetLastError());
        }

        struct in_addr addr{0};
        std::stringstream ipList;

        for (int i = 0; info->h_addr_list[i] != 0; ++i)
        {
            addr.S_un.S_addr = *(u_long*)info->h_addr_list[i];
            ipList << '\n' << inet_ntoa(addr);
        }

        ip_ = ipList.str();
    }



    void ComputerInfo::updateInfo()
    {
        updateRAM();
        updateCPU();
        updateLocalName();
        updateIP();
    }



    RAM ComputerInfo::getMemory() const
    {
        return ram_;
    }



    CPU ComputerInfo::getCPU() const
    {
        return cpu_;
    }



    const char* ComputerInfo::getName() const
    {
        return localName_.c_str();
    }



    const char* ComputerInfo::getIP() const
    {
        return ip_.c_str();
    }



    std::string ComputerInfo::packJson()
    {
        return serialize(value_from(*this));
    }



    ComputerInfo& ComputerInfo::UnpackJson(const std::string& info)
    {
        value val = parse(info);
        *this = std::move(value_to<ComputerInfo>(val));
        return *this;
    }



    template <class T>
    void extract(const object& obj, T& t, string_view key)
    {
        t = value_to<T>(obj.at(key));
    }



    // Overloaded for serialization
    void tag_invoke(value_from_tag, value& val, const ComputerInfo& info)
    {
        RAM ram = info.getMemory();
        CPU cpu = info.getCPU();
        val =
        {
            { "ram.MemoryLoad", ram.MemoryLoad},
            { "ram.TotalMemory", ram.TotalMemory},
            { "ram.AvailMemory", ram.AvailMemory},

            { "cpu.architecture", cpu.architecture },
            { "cpu.name", cpu.name },

            { "localName", info.getName()},
            { "ip", info.getIP()}
        };
    }



    // Overloaded for deserialize
    ComputerInfo tag_invoke(value_to_tag<ComputerInfo>, const value& val)
    {
        RAM ram;
        CPU cpu;
        std::string localName;
        std::string ip;

        const object& obj = val.as_object();

        extract(obj, ram.AvailMemory, "ram.MemoryLoad");
        extract(obj, ram.TotalMemory, "ram.TotalMemory");
        extract(obj, ram.AvailMemory, "ram.AvailMemory");

        extract(obj, cpu.architecture, "cpu.architecture");
        extract(obj, cpu.name, "cpu.name");

        extract(obj, localName, "localName");
        extract(obj, ip, "ip");

        return ComputerInfo(localName.c_str(), ip.c_str(), ram, cpu);
    }

} // Distance
