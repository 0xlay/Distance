#include "ScanCodes.hpp"


namespace
{
    constexpr int OnShiftKey = 0x80;
    constexpr int OffShiftKey = 0x0;
    const wchar_t* EnglishKeyboardLayout = L"00000409";
}


namespace Distance
{

    VKeyCode MakeCodeToVKeyCode(PS2_Set1_Make code)
    {
        static std::unordered_map<PS2_Set1_Make, VKeyCode> ScanCodesTable
        {
            {0x1E, 'A'}, {0x30, 'B'}, {0x2E, 'C'}, {0x20, 'D'}, {0x12, 'E'},
            {0x21, 'F'}, {0x22, 'G'}, {0x23, 'H'}, {0x17, 'I'}, {0x24, 'J'},
            {0x25, 'K'}, {0x26, 'L'}, {0x32, 'M'}, {0x31, 'N'}, {0x18, 'O'},
            {0x19, 'P'}, {0x10, 'Q'}, {0x13, 'R'}, {0x1F, 'S'}, {0x14, 'T'},
            {0x16, 'U'}, {0x2F, 'V'}, {0x11, 'W'}, {0x2D, 'X'}, {0x15, 'Y'},
            {0x2C, 'Z'},

            {0x02, '1'}, {0x03, '2'}, {0x04, '3'}, {0x05, '4'}, {0x06, '5'},
            {0x07, '6'}, {0x08, '7'}, {0x09, '8'}, {0x0A, '9'}, {0x0B, '0'},

            {0x1C, VK_RETURN}, {0x01, VK_ESCAPE}, {0x0E, VK_BACK}, {0x0F, VK_TAB},
            {0x39, VK_SPACE}, {0x3A, VK_CAPITAL}, {0x5D, 0x93/*context menu*/},

            /*{0xD1, VK_LCONTROL}, {0xE01D, VK_RCONTROL},*/
            {0x1D, VK_CONTROL},
            {0x2A, VK_LSHIFT}, {0x36, VK_RSHIFT},
            /*{0x38, VK_LMENU}, {0xE38, VK_RMENU},*/
            {0x38, VK_MENU},
            {0x5B, VK_LWIN}, {0x5C, VK_RWIN},

            {0xE037, VK_SNAPSHOT}, {0x46, VK_SCROLL}, {0xE05F, VK_SLEEP},
            {0xE052, VK_INSERT}, {0xE047, VK_HOME}, {0xE049, VK_PRIOR},
            {0xE053, VK_DELETE}, {0xE04F, VK_END}, {0xE051, VK_NEXT},
            {0xE04D, VK_RIGHT}, {0xE04B, VK_LEFT}, {0xE050, VK_DOWN},
            {0xE048, VK_UP},

            {0x45, VK_NUMLOCK}, {0xE035, VK_SEPARATOR}, {0x37, VK_MULTIPLY},
            {0x4A, VK_SUBTRACT}, {0x4E, VK_ADD}, {0xE01C, VK_RETURN},
            {0x4F, VK_NUMPAD1}, {0x50, VK_NUMPAD2}, {0x51, VK_NUMPAD3},
            {0x4B, VK_NUMPAD4}, {0x4C, VK_NUMPAD5}, {0x4D, VK_NUMPAD6},
            {0x47, VK_NUMPAD7}, {0x48, VK_NUMPAD8}, {0x49, VK_NUMPAD9},
            {0x52, VK_NUMPAD0}, {0x53, VK_OEM_PERIOD},

            {0xE11D45, VK_PAUSE}, {0xE19DC5, VK_PAUSE},
            {0xE046, VK_CANCEL}, {0xE0C6, VK_CANCEL},

            {0x0C, VK_OEM_MINUS}, {0x0D, VK_OEM_PLUS},

            {0x1A, VK_OEM_4}, {0x1B, VK_OEM_6}, {0x2B, VK_OEM_5}, {0x27, VK_OEM_1},
            {0x28, VK_OEM_7}, {0x29, VK_OEM_3},
            {0x33, VK_OEM_COMMA}, {0x7E, VK_OEM_COMMA},
            {0x34, VK_OEM_PERIOD}, {0x35, VK_OEM_2},

            {0x3B, VK_F1}, {0x3C, VK_F2}, {0x3D, VK_F3}, {0x3E, VK_F4}, {0x3F, VK_F5},
            {0x40, VK_F6}, {0x41, VK_F7}, {0x42, VK_F8}, {0x43, VK_F9}, {0x44, VK_F10},
            {0x57, VK_F11}, {0x58, VK_F12}, {0x64, VK_F13}, {0x65, VK_F14},
            {0x66, VK_F15}, {0x67, VK_F16}, {0x68, VK_F17}, {0x69, VK_F18},
            {0x6A, VK_F19}, {0x6B, VK_F20}, {0x6C, VK_F21}, {0x6D, VK_F22},
            {0x6E, VK_F23}, {0x76, VK_F24},

            {0xF2, VK_HANGUL}, {0xF1, VK_HANJA}, {0x78, VK_KANA}
        };

        return ScanCodesTable[code];
    }



    DWORD GetActiveKeyboardLayout(HKEY KeyToLocales)
    {
        DWORD layout = 0;
        DWORD size = 4;

        auto lstat = RegQueryValueExW(
            KeyToLocales,
            _T("InputLocale"),
            nullptr,
            nullptr,
            (BYTE*)&layout,
            &size
        );
        if (lstat != ERROR_SUCCESS)
        {
            layout = KeyboardLayouts::EN;
        }

        return layout;
    }



    std::wstring VKeyToStringW(VKeyCode VKey, HKEY KeyToLocales)
    {
        std::wstring FKeyName = FunctionalKeyToStringW(VKey);
        if (!FKeyName.empty())
            return FKeyName;

        std::wstring SKeyName = StandardKeyToStringW(VKey, KeyToLocales);
        if (!SKeyName.empty())
            return SKeyName;
        return L"";
    }



    const std::wstring& FunctionalKeyToStringW(VKeyCode VKey)
    {
        static std::unordered_map<VKeyCode, std::wstring> keys
        {
            {VK_BACK, L" ${BACKSPACE} "},
            {VK_TAB, L" ${TAB} "},
            {VK_CLEAR, L" ${CLEAR} "},
            {VK_RETURN, L" ${ENTER}\n"},
            {VK_SHIFT, L" ${SHIFT} "},
            {VK_CONTROL, L" ${CTRL} "},
            {VK_MENU, L" ${ALT} "},
            {VK_PAUSE, L" ${PAUSE} "},
            {VK_CAPITAL, L" ${CAPS LOCK} "},
            {VK_ESCAPE, L" ${ESC} "},
            {VK_SPACE, L" ${SPACE} "},
            {VK_PRIOR, L" ${PAGE UP} "},
            {VK_NEXT, L" ${PAGE DOWN} "},
            {VK_END, L" ${END} "},
            {VK_HOME, L" ${HOME} "},
            {VK_LEFT, L" ${LEFT ARROW} "},
            {VK_UP, L" ${UP ARROW} "},
            {VK_RIGHT, L" ${RIGHT ARROW} "},
            {VK_DOWN, L" ${DOWN ARROW} " },
            {VK_SELECT, L" ${SELECT} " },
            {VK_PRINT, L" ${PRINT} " },
            {VK_EXECUTE, L" ${EXECUTE} " },
            {VK_SNAPSHOT, L" ${PRINT SCREEN} " },
            {VK_INSERT, L" ${INS} " },
            {VK_DELETE, L" ${DEL} "},
            {VK_HELP, L" ${HELP} "},
            {VK_LWIN, L" ${L_WIN} "},
            {VK_RWIN, L" ${R_WIN} "},
            {VK_APPS, L" ${APPS KEY} "},
            {VK_SLEEP, L" ${SLEEP} "},
            {VK_F1, L" ${F1} "},
            {VK_F2, L" ${F2} "},
            {VK_F3, L" ${F3} "},
            {VK_F4, L" ${F4} "},
            {VK_F5, L" ${F5} "},
            {VK_F6, L" ${F6} "},
            {VK_F7, L" ${F7} "},
            {VK_F8, L" ${F8} "},
            {VK_F9, L" ${F9} "},
            {VK_F10, L" ${F10} "},
            {VK_F11, L" ${F11} "},
            {VK_F12, L" ${F12} "},
            {VK_F13, L" ${F13} "},
            {VK_F14, L" ${F14} "},
            {VK_F15, L" ${F15} "},
            {VK_F16, L" ${F16} "},
            {VK_F17, L" ${F17} "},
            {VK_F18, L" ${F18} "},
            {VK_F19, L" ${F19} "},
            {VK_F20, L" ${F20} "},
            {VK_F21, L" ${F21} "},
            {VK_F22, L" ${F22} "},
            {VK_F23, L" ${F23} "},
            {VK_F24, L" ${F24} "},
            {VK_NUMLOCK, L" ${NUM LOCK} "},
            {VK_SCROLL, L" ${SCROLL LOCK} "},
            {VK_LSHIFT, L" ${L_SHIFT} "},
            {VK_RSHIFT, L" ${R_SHIFT} "},
            {VK_LCONTROL, L" ${L_CTRL} "},
            {VK_RCONTROL, L" ${R_CTRL} "},
            {VK_LMENU, L" ${L_ALT} "},
            {VK_RMENU, L" ${R_ALT} "},
            {VK_BROWSER_BACK, L" ${BROWSER BACK} "},
            {VK_BROWSER_FORWARD, L" ${BROWSER FORWARD} "},
            {VK_BROWSER_REFRESH, L" ${BROWSER REFRESH} "},
            {VK_BROWSER_STOP, L" ${BROWSER STOP} "},
            {VK_BROWSER_SEARCH, L" ${BROWSER SEARCH} "},
            {VK_BROWSER_FAVORITES, L" ${BROWSER FAVORITES} "},
            {VK_BROWSER_HOME, L" ${BROWSER START} "},
            {VK_VOLUME_MUTE, L" ${VOLUME MUTE} "},
            {VK_VOLUME_DOWN, L" ${VOLUME DOWN} "},
            {VK_VOLUME_UP, L" ${VOLUME UP} "},
            {VK_MEDIA_NEXT_TRACK, L" ${NEXT TRACK} "},
            {VK_MEDIA_PREV_TRACK, L" ${PREV TRACK} "},
            {VK_MEDIA_PLAY_PAUSE, L" ${PLAY/PAUSA MEDIA} "},
            {VK_LAUNCH_MAIL, L" ${START MAIL} "},
            {VK_LAUNCH_MEDIA_SELECT, L" ${SELECT MEDIA} "},
            {VK_ATTN, L" ${ATTN KEY} "},
            {VK_CRSEL, L" ${CRSEL KEY} "},
            {VK_EXSEL, L" ${EXSEL KEY} "},
            {VK_EREOF, L" ${ERASE EOF} "},
            {VK_PLAY, L" ${PLAY} "},
            {VK_ZOOM, L" ${ZOOM} "},
            {VK_PA1, L" ${PA1 KEY}"}
        };
        return keys[VKey];
    }



    std::wstring StandardKeyToStringW(VKeyCode VKey, HKEY KeyToLocales)
    {
        WCHAR name[2] = { 0 };
        BYTE kbState[256] = { 0 };

        SHORT isShiftPressed = GetKeyState(VK_SHIFT) & 0x8000;
        SHORT isCapsLockPressed = GetKeyState(VK_CAPITAL) & 0x0001;


        if (isShiftPressed)
            kbState[VK_SHIFT] = ::OnShiftKey;

        ToUnicodeEx(
            VKey,
            MapVirtualKey(VKey, MAPVK_VK_TO_VSC),
            kbState,
            name,
            2,
            0,
            LoadKeyboardLayoutW(::EnglishKeyboardLayout, KLF_ACTIVATE)
        );

        kbState[VK_SHIFT] = ::OffShiftKey;


        DWORD layout = GetActiveKeyboardLayout(KeyToLocales);
        switch (LOWORD(layout))
        {
        case KeyboardLayouts::UA:
            name[0] = _towlower_l(name[0], _get_current_locale());
            name[0] = TranslateToUA(name[0]);
            break;
        case KeyboardLayouts::RU:
            name[0] = _towlower_l(name[0], _get_current_locale());
            name[0] = TranslateToRU(name[0]);
            break;
        }

        if (isCapsLockPressed && isShiftPressed)
            name[0] = _towlower_l(name[0], _get_current_locale());
        else if (isCapsLockPressed)
            name[0] = _towupper_l(name[0], _get_current_locale());

        return name;
    }



    wchar_t TranslateToUA(wchar_t symbol)
    {
        static std::unordered_map<wchar_t, wchar_t> alphabet
        {
            {L'q', L'й'}, {L'w', L'ц'}, {L'e', L'у'}, {L'r', L'к'}, {L't', L'е'},
            {L'y', L'н'}, {L'u', L'г'}, {L'i', L'ш'}, {L'o', L'щ'}, {L'p', L'з'},
            {L'a', L'ф'}, {L's', L'і'}, {L'd', L'в'}, {L'f', L'а'}, {L'g', L'п'},
            {L'h', L'р'}, {L'j', L'о'}, {L'k', L'л'}, {L'l', L'д'}, {L'z', L'я'},
            {L'x', L'ч'}, {L'c', L'с'}, {L'v', L'м'}, {L'b', L'и'}, {L'n', L'т'},
            {L'm', L'ь'},

            {L';', L'ж'},  {L'/', L'.'}, {L'`', L'\''}, {L'[', L'х'}, {L']', L'ї'},
            {L'\'', L'є'}, {L',', L'б'}, {L'.', L'ю'}
        };

        auto result = alphabet[symbol];
        return result ? result : symbol;
    }



    wchar_t TranslateToRU(wchar_t symbol)
    {
        static std::unordered_map<wchar_t, wchar_t> alphabet
        {
            {L'q', L'й'}, {L'w', L'ц'}, {L'e', L'у'}, {L'r', L'к'}, {L't', L'е'},
            {L'y', L'н'}, {L'u', L'г'}, {L'i', L'ш'}, {L'o', L'щ'}, {L'p', L'з'},
            {L'a', L'ф'}, {L's', L'ы'}, {L'd', L'в'}, {L'f', L'а'}, {L'g', L'п'},
            {L'h', L'р'}, {L'j', L'о'}, {L'k', L'л'}, {L'l', L'д'},	{L'z', L'я'},
            {L'x', L'ч'}, {L'c', L'с'}, {L'v', L'м'}, {L'b', L'и'}, {L'n', L'т'},
            {L'm', L'ь'},

            {L';', L'ж'},  {L'/', L'.'}, {L'`', L'ё'}, {L'[', L'х'}, {L']', L'ъ'},
            {L'\'', L'э'}, {L',', L'б'}, {L'.', L'ю'}
        };

        auto result = alphabet[symbol];
        return result ? result : symbol;
    }

} // Distance