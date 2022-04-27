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
// File: NetfilterModule.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "NetfilterModule.hpp"

namespace Distance::Module
{

    Netfilter::Netfilter()
        : sessionName_ (L"DistanceNetfilterSession"),
          sessionDesc_ (L"WFP session"),
          providerName_(L"DistanceNetfilterProvider"),
          providerDesc_(L"WFP provider"),
          sublayerName_(L"DistanceNetfilterSublayer"),
          sublayerDesc_(L"WFP sublayer"),
          filterName_  (L"DistanceNetfilter"),
          filterDesc_  (L"WFP filter")
    {
        //
        // Init session
        //

        session_.displayData.name = &sessionName_[0];
        session_.displayData.description = &sessionDesc_[0];

        //
        // Init provider
        //

        provider_.providerKey = Guid_Netfilter_Provider;
        provider_.displayData.name = &providerName_[0];
        provider_.displayData.description = &providerDesc_[0];

        //
        // Init sublayer
        //

        sublayer_.subLayerKey = Guid_Netfilter_Sublayer;
        sublayer_.displayData.name = &sublayerName_[0];
        sublayer_.displayData.description = &sublayerDesc_[0];
        sublayer_.flags = 0; /* reset on reboot */
        sublayer_.weight = 0xFFFF; /* max weight */

    }

    void Netfilter::run()
    {
        DWORD error = FwpmEngineOpen0(nullptr,
                                      RPC_C_AUTHN_WINNT,
                                      nullptr,
                                      &session_,
                                      &engine_);
        if (error)
        {
            throw std::runtime_error("Failed to open session to a filter engine!");
        }

        error = FwpmProviderAdd0(engine_, &provider_, nullptr);
        if (error)
        {
            throw std::runtime_error("Failed to add provider!");
        }

        error = FwpmSubLayerAdd0(engine_, &sublayer_, nullptr);
        if (error)
        {
            throw std::runtime_error("Failed to add sublayer!");
        }

    }

    void Netfilter::stop()
    {
        disableBlockTcpPorts();
        FwpmSubLayerDeleteByKey0(engine_, &Guid_Netfilter_Sublayer);
        FwpmProviderDeleteByKey0(engine_, &Guid_Netfilter_Provider);
        FwpmEngineClose0(engine_);
    }

    void Netfilter::enableBlockTcpPorts(const std::vector<UINT16>& ports)
    {
        if (isBlockSet_)
        {
            disableBlockTcpPorts();
        }

        std::vector<FWPM_FILTER_CONDITION0> conditions;

        FWPM_FILTER_CONDITION0 tmp;
        tmp.fieldKey = FWPM_CONDITION_IP_PROTOCOL;
        tmp.matchType = FWP_MATCH_EQUAL;
        tmp.conditionValue.type = FWP_UINT8;
        tmp.conditionValue.uint8 = IPPROTO_TCP;

        conditions.emplace_back(tmp);

        for (const auto& port : ports)
        {
            tmp.fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
            tmp.matchType = FWP_MATCH_EQUAL;
            tmp.conditionValue.type = FWP_UINT16;
            tmp.conditionValue.uint16 = port;

            conditions.emplace_back(tmp);
        }

        //
        // Init filter
        //

        filter_.providerKey = nullptr;
        filter_.displayData.name = &filterName_[0];
        filter_.displayData.description = &filterDesc_[0];
        filter_.flags = 0; /* reset on reboot */
        filter_.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
        filter_.subLayerKey = Guid_Netfilter_Sublayer;
        filter_.weight.type = FWP_UINT8;
        filter_.weight.uint8 = 1;
        filter_.numFilterConditions = static_cast<UINT32>(conditions.size());
        filter_.filterCondition = &conditions[0];
        filter_.action.type = FWP_ACTION_BLOCK;
        filter_.action.filterType = Guid_Netfilter_Provider; /* or provider */

        if (FwpmFilterAdd0(engine_, &filter_, nullptr, &filterId_))
        {
            throw std::runtime_error("Failed to add filter!");
        }

        isBlockSet_ = true;
    }

    void Netfilter::disableBlockTcpPorts()
    {
        FwpmFilterDeleteById0(engine_, filterId_);
        isBlockSet_ = false;
    }

} // Distance::Module
