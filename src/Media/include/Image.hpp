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
// File: Image.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Media.exe
//
//------------------------------------------------------------------------------

#pragma once
#include <vector>
#include <string>
#include <fstream>


namespace Distance::Media
{

    class Image final
    {
    public:
        Image(size_t sizeInBytes)
        {
            mem_.resize(sizeInBytes);
        }

        Image(std::vector<char> bytes)
            : mem_(std::move(bytes))
        {}

        Image(const std::string& bytes)
            : mem_(bytes.cbegin(), bytes.cend())
        {}

        [[nodiscard]] const char* get() const
        {
            return &mem_.at(0);
        }
        
        [[nodiscard]] char* get()
        {
            return &mem_.at(0);
        }

        const std::vector<char>& data() const
        {
            return mem_;
        }
        
        [[nodiscard]] size_t size() const noexcept
        {
            return mem_.size();
        }

    private:
        std::vector<char> mem_;
    };

    inline std::ofstream& operator<<(std::ofstream& stream, const Image& image)
    {
        stream.write(image.get(), image.size()).flush();
        return stream;
    }

    inline std::ifstream& operator>>(std::ifstream& stream, Image& image)
    {
        stream.read(image.get(), image.size());
        return stream;
    }

} // Distance::Media
