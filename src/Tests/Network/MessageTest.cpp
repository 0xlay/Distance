#include "pch.h"
#include "Network/include/Message.hpp"

using namespace Distance::Network;

struct NetworkMessageTest : testing::Test
{
    Message msg;

    NetworkMessageTest() : msg(MsgType::GetSysInfo, 10) {}
};

TEST_F(NetworkMessageTest, GetMsgType)
{
    EXPECT_EQ(msg.getMsgType(), MsgType::GetSysInfo);
}

TEST_F(NetworkMessageTest, SetMsgType)
{
    msg.setMsgType(MsgType::GetSysInfo);
    EXPECT_EQ(msg.getMsgType(), MsgType::GetSysInfo);
}

TEST_F(NetworkMessageTest, ResizeMsg)
{
    constexpr int newSize = 100;
    msg.resize(newSize);
    EXPECT_EQ(msg.size(), newSize);
}

TEST_F(NetworkMessageTest, SizeMsg)
{
    EXPECT_NE(msg.size(), msg.capacity());
}

TEST_F(NetworkMessageTest, CapacityMsg)
{
    EXPECT_EQ(msg.capacity(), msg.getMemory().size());
}

TEST_F(NetworkMessageTest, WriteString_and_ReadString)
{
    msg.clear();
    std::string str1 = "Hello, ";
    std::string str2 = "world!";
    std::string strRes;
    
    msg << str1 << str2;
    msg >> strRes;

    EXPECT_EQ((str1 + str2), strRes);
}

TEST_F(NetworkMessageTest, WriteWString_and_ReadWString)
{
    msg.clear();
    std::wstring str1 = L"Hello, ";
    std::wstring str2 = L"world!";
    std::wstring strRes;

    msg << str1 << str2;
    msg >> strRes;

    EXPECT_EQ((str1 + str2), strRes);
}

TEST_F(NetworkMessageTest, WriteVector_and_ReadVector)
{
    msg.clear();
    std::vector<char> data = { '1', '2', '3'};
    std::vector<char> dataRes;

    msg << data;
    msg >> dataRes;

    EXPECT_EQ(data, dataRes);
}

TEST_F(NetworkMessageTest, WritePrimitive_and_ReadPrimitive)
{
    msg.clear();
    int intA = 10, intB;
    char charA = 'A', charB;
    float floatA = 10.0f, floatB;
    double doubleA = 10.0, doubleB;

    msg << intA;
    msg >> intB;
    
    msg << charA;
    msg >> charB;

    msg << floatA;
    msg >> floatB;

    msg << doubleA;
    msg >> doubleB;

    EXPECT_EQ(intA, intB);
    EXPECT_EQ(charA, charB);
    EXPECT_EQ(floatA, floatB);
    EXPECT_EQ(doubleA, doubleB);
}

TEST_F(NetworkMessageTest, WriteArray_and_ReadArray)
{
    msg.clear();
    std::array<int, 3> arrayA = { 1, 2, 3 };
    std::array<int, 3> arrayB;

    msg << arrayA;
    msg >> arrayB;
    
    for (int i = 0; i < arrayA.size(); ++i)
        EXPECT_EQ(arrayA[i], arrayB[i]);
}

TEST_F(NetworkMessageTest, WritePOD_and_ReadPOD)
{
    msg.clear();
    struct MyStruct
    {
        int a{}, b{};
        bool operator == (const MyStruct& rhs) const
        {
            return (a == rhs.a && b == rhs.b);
        }
    };

    MyStruct structA{ 10, 12 };
    MyStruct structB;
    
    msg << structA;
    msg >> structB;

    EXPECT_EQ(structA, structB);
}

TEST_F (NetworkMessageTest, WriteDifferent_and_ReadDifferent)
{
    msg.clear();
    std::string source = "Get [1] object!";
    std::string result;

    msg << "Get [" << std::to_string(1) << "] object!";
    msg >> result;

    EXPECT_EQ(result, source);
}
