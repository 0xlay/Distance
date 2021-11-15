#ifndef _DISTANCE_SCAN_CODES_HPP_
#define _DISTANCE_SCAN_CODES_HPP_

#include <Windows.h>
#include <Tchar.h>
#include <string>
#include <locale.h>
#include <unordered_map>


namespace Distance
{
    enum KeyboardLayouts
    {
        EN = 0x409,
        UA = 0x422,
        RU = 0x419
    };

    using PS2_Set1_Make = DWORD;
    using VKeyCode = DWORD;

    VKeyCode MakeCodeToVKeyCode(PS2_Set1_Make code);

    DWORD GetActiveKeyboardLayout(HKEY KeyToLocales);

    std::wstring VKeyToStringW(VKeyCode VKey, HKEY KeyToLocales);

    const std::wstring& FunctionalKeyToStringW(VKeyCode VKey);

    std::wstring StandardKeyToStringW(VKeyCode VKey, HKEY KeyToLocales);

    wchar_t TranslateToUA(wchar_t symbol);

    wchar_t TranslateToRU(wchar_t symbol);
}

#endif // _DISTANCE_SCAN_CODES_HPP_