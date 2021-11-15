#include <iostream>
#include <NetfilterModule.hpp>


using namespace Distance;


NetfilterModule::NetfilterModule(const char* path)
    : Engine_(nullptr), Logfile_(nullptr), calloutAddId_(0), filterId_(0)
{
    driverCallout_.reset(new xstar::StartWinDriver(L"NetFilter"));

    InitWfp();

    Logfile_ = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );
    if (Logfile_ == INVALID_HANDLE_VALUE)
    {
        throw xstar::WinAPIException();
    }

    buffer_ = std::make_unique<char[]>(PAGE_SIZE);
}



NetfilterModule::~NetfilterModule()
{
    UnInitWfp();
    CloseHandle(Logfile_);
}



char* NetfilterModule::getInfo()
{
    ZeroMemory(buffer_.get(), PAGE_SIZE);
    DWORD bytes;
    if (!ReadFile(
        Logfile_, 
        buffer_.get(), 
        PAGE_SIZE, 
        &bytes, 
        nullptr
    ))
    {
        throw xstar::WinAPIException();
    }
    return buffer_.get();
}



void NetfilterModule::InitWfp()
{
    //--------------------------------------------------------------------------
    // Open engine
    //--------------------------------------------------------------------------

    auto status = FwpmEngineOpen0(
        nullptr,
        RPC_C_AUTHN_WINNT,
        nullptr,
        nullptr,
        &Engine_
    );
    if (status)
        throw xstar::WinAPIException("FwpmEngineOpen0", status);

    //--------------------------------------------------------------------------
    // Add callout
    //--------------------------------------------------------------------------

    std::wstring calloutName = L"NetFilterCallout";
    std::wstring calloutDescription = L"NetFilter callout";

    FWPM_CALLOUT0 callout{ 0 };
    callout.calloutKey = NETFILTER_ACTION_CALLOUT_SPYING_AND_TERMINATING;
    callout.flags = 0;
    callout.displayData.name = const_cast<wchar_t*>(calloutName.c_str());
    callout.displayData.description = const_cast<wchar_t*>(calloutDescription.c_str());
    callout.applicableLayer = FWPM_LAYER_ALE_AUTH_CONNECT_V4;

    status = FwpmCalloutAdd0(Engine_, &callout, nullptr, &calloutAddId_);
    if (status)
        throw xstar::WinAPIException("FwpmCalloutAdd0", status);

    //--------------------------------------------------------------------------
    // Create sublayer
    //--------------------------------------------------------------------------

    std::wstring sublayerName = L"NetFilterLayer";
    std::wstring sublayerDescription = L"NetFilter layer";

    FWPM_SUBLAYER0 sublayer{ 0 };
    sublayer.displayData.name = const_cast<wchar_t*>(sublayerName.c_str());
    sublayer.displayData.description = const_cast<wchar_t*>(sublayerDescription.c_str());
    sublayer.subLayerKey = NETFILTER_SUB_LAYER;
    sublayer.flags = 0;
    sublayer.weight = 0xFFFF;

    status = FwpmSubLayerAdd0(Engine_, &sublayer, nullptr);
    if (status)
        throw xstar::WinAPIException("FwpmSubLayerAdd0", status);

    //--------------------------------------------------------------------------
    // Add conditions and filter
    //--------------------------------------------------------------------------

    const UINT32 numConditions = 1;
    FWPM_FILTER_CONDITION0 condition[1];

    condition[0].fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
    condition[0].matchType = FWP_MATCH_LESS_OR_EQUAL;
    condition[0].conditionValue.type = FWP_UINT16;
    condition[0].conditionValue.uint16 = 65000; // 80 default HTTP port

    FWPM_FILTER0 filter{0};
    std::wstring filterName = L"NetFilter";
    std::wstring filterDescription = L"NetFilter is a filter";

    filter.providerKey = nullptr;
    filter.displayData.name = const_cast<wchar_t*>(filterName.c_str());
    filter.displayData.description = const_cast<wchar_t*>(filterDescription.c_str());
    filter.flags = 0;
    filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
    filter.subLayerKey = NETFILTER_SUB_LAYER;
    filter.weight.type = FWP_EMPTY;
    filter.numFilterConditions = numConditions;
    filter.filterCondition = &condition[0];
    filter.action.type = FWP_ACTION_CALLOUT_TERMINATING;
    filter.action.calloutKey = NETFILTER_ACTION_CALLOUT_SPYING_AND_TERMINATING;


    status = FwpmFilterAdd(Engine_, &filter, nullptr, &filterId_);
    if (status)
        throw xstar::WinAPIException("FwpmFilterAdd", status);
}



void NetfilterModule::UnInitWfp()
{
    if (Engine_ == NULL)
        return;

    if (filterId_ != 0)
    {
        FwpmFilterDeleteById0(Engine_, filterId_);
        FwpmSubLayerDeleteByKey0(Engine_, &NETFILTER_SUB_LAYER);
    }

    if (calloutAddId_ != 0)
    {
        FwpmCalloutDeleteById0(Engine_, calloutAddId_);
    }

    FwpmEngineClose0(Engine_);
}
