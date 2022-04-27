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
// File: NetfilterModule.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "Module.hpp"
#include <fwpmu.h>
#include <vector>

// {46B0B7E9-6882-4C9B-BCCF-82939F699371}
constexpr  GUID Guid_Netfilter_Provider =
{ 0x46b0b7e9, 0x6882, 0x4c9b, { 0xbc, 0xcf, 0x82, 0x93, 0x9f, 0x69, 0x93, 0x71 } };

// {9689932B-E562-499B-9178-E4570B63D228}
constexpr GUID Guid_Netfilter_Sublayer =
{ 0x9689932b, 0xe562, 0x499b, { 0x91, 0x78, 0xe4, 0x57, 0xb, 0x63, 0xd2, 0x28 } };

// {DC120751-05AE-434B-AB58-25A561C12326}
constexpr  GUID Guid_Netfilter_Callout =
{ 0xdc120751, 0x5ae, 0x434b, { 0xab, 0x58, 0x25, 0xa5, 0x61, 0xc1, 0x23, 0x26 } };

// {BA808E78-0D65-4DB3-B0F7-5C1CC4F40201}
constexpr GUID Guid_Netfilter =
{ 0xba808e78, 0xd65, 0x4db3, { 0xb0, 0xf7, 0x5c, 0x1c, 0xc4, 0xf4, 0x2, 0x1 } };


namespace Distance::Module
{

    class Netfilter : public IModule
    {
    public:
        Netfilter();

        void run() override;
        void stop() override;

        void enableBlockTcpPorts(const std::vector<UINT16>& ports);
        void disableBlockTcpPorts();

    private:
        HANDLE engine_{};
        bool isBlockSet_ = false;

        FWPM_SESSION0 session_{};
        std::wstring sessionName_;
        std::wstring sessionDesc_;

        FWPM_PROVIDER0 provider_{};
        std::wstring providerName_;
        std::wstring providerDesc_;

        FWPM_SUBLAYER0 sublayer_{};
        std::wstring sublayerName_;
        std::wstring sublayerDesc_;

        FWPM_FILTER0 filter_{};
        UINT64 filterId_{};
        std::wstring filterName_;
        std::wstring filterDesc_;

    };

} // Distance::Module
