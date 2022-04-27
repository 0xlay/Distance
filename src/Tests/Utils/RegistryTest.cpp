#include "pch.h"
#include "Utils/include/Registry.hpp"
#include "fixture/RegitstryTest.hpp"


using namespace Distance::Utils;
void NotifyCallback(HANDLE event, int, std::string);


TEST_F(RegistryTest, AddKey)
{
    ErrorCode error;
    error = registry->addKey(L"Software\\Distance");
    if (error)
    {
        ASSERT_EQ(true, false) << "[Registry.addKey] failed! Error code: " << error;
    }
}

TEST_F(RegistryTest, SetNotifyChangeData)
{
    ErrorCode error;
    error = registry->setNotifyChangeData(
        RegNotifyFilter::LastSet,
        false,
        std::function(NotifyCallback),
        100,
        std::string("Data was added!")
    );
    if (error)
    {
        ASSERT_EQ(true, false) << "[Registry.setNotifyChangeData] failed! Error code: " << error;
    }
}

TEST_F(RegistryTest, SetAndGetData)
{
    ErrorCode error;
    uint32_t initialValue = 100;
    uint32_t resultValue;
    
    error = registry->setData(initialValue, L"Test");
    if (error)
    {
        ASSERT_EQ(true, false) << "[Registry.setData] failed! Error code: " << error;
    }

    error = registry->getData(resultValue, L"Test");
    if (error)
    {
        ASSERT_EQ(true, false) << "[Registry.getData] failed! Error code: " << error;
    }

    EXPECT_EQ(initialValue, resultValue);
}

TEST_F(RegistryTest, DeleteField)
{
    ErrorCode error;
    error = registry->deleteField(L"Test");
    if (error)
    {
        ASSERT_EQ(true, false) << "[Registry.deleteField] failed! Error code: " << error;
    }
}

TEST_F(RegistryTest, DeleteKey)
{
    ErrorCode error;
    error = registry->deleteKey(L"Software\\Distance");
    if (error)
    {
        ASSERT_EQ(true, false) << "[Registry.deleteKey] failed! Error code: " << error;
    }
}

void NotifyCallback(HANDLE event, int val, std::string str)
{
    if (WaitForSingleObject(event, 1000) == WAIT_OBJECT_0)
    {
        ASSERT_EQ(val, 100);
        ASSERT_STREQ(str.c_str(), "Data was added!");
    }
    else
    {
        ASSERT_EQ(true, false) << "[Registry.setNotifyChangeData]: failed!" << std::endl; 
    }
}
