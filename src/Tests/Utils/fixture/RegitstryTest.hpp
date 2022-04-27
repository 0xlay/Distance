#pragma once
#include "pch.h"
#include "Utils/include/Registry.hpp"


struct RegistryTest : testing::Test
{
    static inline  Distance::Utils::Registry* registry = nullptr;

    static void SetUpTestCase()
    {
        registry = new  Distance::Utils::Registry(
            HKEY_CURRENT_USER,
            Distance::Utils::RegistryOption::NonVolatile,
            Distance::Utils::RegistryAccess::AllAccess
        );
    }

    static void TearDownTestCase()
    {
        delete registry;
    }

};
