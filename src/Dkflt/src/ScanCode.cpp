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
// File: ScanCode.cpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------

#include "ScanCode.hpp"
#include <wdm.h>

#ifdef ALLOC_DATA_PRAGMA
#pragma data_seg("PAGEDATA")

namespace VKeyName
{
    constexpr wchar_t A[]                 = L"A";
    constexpr wchar_t B[]                 = L"B";
    constexpr wchar_t C[]                 = L"C";
    constexpr wchar_t D[]                 = L"D";
    constexpr wchar_t E[]                 = L"E";
    constexpr wchar_t F[]                 = L"F";
    constexpr wchar_t G[]                 = L"G";
    constexpr wchar_t H[]                 = L"H";
    constexpr wchar_t I[]                 = L"I";
    constexpr wchar_t J[]                 = L"J";
    constexpr wchar_t K[]                 = L"K";
    constexpr wchar_t L[]                 = L"L";
    constexpr wchar_t M[]                 = L"M";
    constexpr wchar_t N[]                 = L"N";
    constexpr wchar_t O[]                 = L"O";
    constexpr wchar_t P[]                 = L"P";
    constexpr wchar_t Q[]                 = L"Q";
    constexpr wchar_t R[]                 = L"R";
    constexpr wchar_t S[]                 = L"S";
    constexpr wchar_t T[]                 = L"T";
    constexpr wchar_t U[]                 = L"U";
    constexpr wchar_t V[]                 = L"V";
    constexpr wchar_t W[]                 = L"W";
    constexpr wchar_t X[]                 = L"X";
    constexpr wchar_t Y[]                 = L"Y";
    constexpr wchar_t Z[]                 = L"Z";
    constexpr wchar_t One[]               = L"1";
    constexpr wchar_t Two[]               = L"2";
    constexpr wchar_t Three[]             = L"3";
    constexpr wchar_t Four[]              = L"4";
    constexpr wchar_t Five[]              = L"5";
    constexpr wchar_t Six[]               = L"6";
    constexpr wchar_t Seven[]             = L"7";
    constexpr wchar_t Eight[]             = L"8";
    constexpr wchar_t Nine[]              = L"9";
    constexpr wchar_t Zero[]              = L"0";
    constexpr wchar_t Separator[]         = L"/";
    constexpr wchar_t Multiply[]          = L"*";
    constexpr wchar_t Add[]               = L"+";
    constexpr wchar_t Subtract[]          = L"-";
    constexpr wchar_t Backspace[]         = L"${BACKSPACE}$";
    constexpr wchar_t Tab[]               = L"${TAB}$";
    constexpr wchar_t Clear[]             = L"${CLEAR}$";
    constexpr wchar_t Enter[]             = L"${ENTER}$";
    constexpr wchar_t Shift[]             = L"${SHIFT}$";
    constexpr wchar_t Ctrl[]              = L"${CTRL}$";
    constexpr wchar_t Alt[]               = L"${ALT}$";
    constexpr wchar_t Pause[]             = L"${PAUSE}$";
    constexpr wchar_t CapsLock[]          = L"${CAPS_LOCK}$";
    constexpr wchar_t Esc[]               = L"${ESC}$";
    constexpr wchar_t Space[]             = L"${SPACE}$";
    constexpr wchar_t PageUp[]            = L"${PAGE UP}$";
    constexpr wchar_t PageDown[]          = L"${PAGE DOWN}$";
    constexpr wchar_t End[]               = L"${END}$";
    constexpr wchar_t Home[]              = L"${HOME}$";
    constexpr wchar_t LeftArrow[]         = L"${LEFT ARROW}$";
    constexpr wchar_t UpArrow[]           = L"${UP ARROW}$";
    constexpr wchar_t RightArrow[]        = L"${RIGHT ARROW}$";
    constexpr wchar_t DownArrow[]         = L"${DOWN ARROW}$";
    constexpr wchar_t Select[]            = L"${SELECT}$";
    constexpr wchar_t Print[]             = L"${PRINT}$";
    constexpr wchar_t Execute[]           = L"${EXECUTE}$";
    constexpr wchar_t PrintScreen[]       = L"${PRINT SCREEN}$";
    constexpr wchar_t Insert[]            = L"${INS}$";
    constexpr wchar_t Delete[]            = L"${DEL}$";
    constexpr wchar_t Help[]              = L"${HELP}$";
    constexpr wchar_t LeftWin[]           = L"${L_WIN}$";
    constexpr wchar_t RightWin[]          = L"${R_WIN}$";
    constexpr wchar_t Apps[]              = L"${APPS_KEY}$";
    constexpr wchar_t Sleep[]             = L"${SLEEP}$";
    constexpr wchar_t F1[]                = L"${F1}$";
    constexpr wchar_t F2[]                = L"${F2}$";
    constexpr wchar_t F3[]                = L"${F3}$";
    constexpr wchar_t F4[]                = L"${F4}$";
    constexpr wchar_t F5[]                = L"${F5}$";
    constexpr wchar_t F6[]                = L"${F6}$";
    constexpr wchar_t F7[]                = L"${F7}$";
    constexpr wchar_t F8[]                = L"${F8}$";
    constexpr wchar_t F9[]                = L"${F9}$";
    constexpr wchar_t F10[]               = L"${F10}$";
    constexpr wchar_t F11[]               = L"${F11}$";
    constexpr wchar_t F12[]               = L"${F12}$";
    constexpr wchar_t F13[]               = L"${F13}$";
    constexpr wchar_t F14[]               = L"${F14}$";
    constexpr wchar_t F15[]               = L"${F15}$";
    constexpr wchar_t F16[]               = L"${F16}$";
    constexpr wchar_t F17[]               = L"${F17}$";
    constexpr wchar_t F18[]               = L"${F18}$";
    constexpr wchar_t F19[]               = L"${F19}$";
    constexpr wchar_t F20[]               = L"${F20}$";
    constexpr wchar_t F21[]               = L"${F21}$";
    constexpr wchar_t F22[]               = L"${F22}$";
    constexpr wchar_t F23[]               = L"${F23}$";
    constexpr wchar_t F24[]               = L"${F24}$";
    constexpr wchar_t NumLock[]           = L"${NUM LOCK}$";
    constexpr wchar_t ScrollLock[]        = L"${SCROLL LOCK}$";
    constexpr wchar_t LeftShift[]         = L"${L_SHIFT}$";
    constexpr wchar_t RightShift[]        = L"${R_SHIFT}$";
    constexpr wchar_t LeftCtrl[]          = L"${L_CTRL}$";
    constexpr wchar_t RightCtrl[]         = L"${R_CTRL}$";
    constexpr wchar_t LeftAlt[]           = L"${L_ALT}$";
    constexpr wchar_t RightAlt[]          = L"${R_ALT}$";
    constexpr wchar_t BrowserBack[]       = L"${BROWSER BACK}$";
    constexpr wchar_t BrowserRefresh[]    = L"${BROWSER REFRESH}$";
    constexpr wchar_t BrowserStop[]       = L"${BROWSER STOP}$";
    constexpr wchar_t BrowserSearch[]     = L"${BROWSER SEARCH}$";
    constexpr wchar_t BrowserFavorites[]  = L"${BROWSER FAVORITES}$";
    constexpr wchar_t BrowserHome[]       = L"${BROWSER START}$";
    constexpr wchar_t VolumeMute[]        = L"${VOLUME MUTE}$";
    constexpr wchar_t VolumeDown[]        = L"${VOLUME DOWN}$";
    constexpr wchar_t VolumeUp[]          = L"${VOLUME UP}$";
    constexpr wchar_t NextTrack[]         = L"${NEXT TRACK}$";
    constexpr wchar_t PrevTrack[]         = L"${PREV TRACK}$";
    constexpr wchar_t PlayOrPause[]       = L"${PLAY/PAUSE MEDIA}$";
    constexpr wchar_t LaunchMail[]        = L"${START MAIL}$";
    constexpr wchar_t LaunchMediaSelect[] = L"${SELECT MEDIA}$";
    constexpr wchar_t Play[]              = L"${PLAY}$";
    constexpr wchar_t Zoom[]              = L"${Zoom}$";
    constexpr wchar_t Unknown[]           = L"${UNKNOWN KEY}$";
}

#pragma data_seg()
#endif

const wchar_t* ScanCodeToVKeyName(_In_ USHORT ScanCode)
{
    switch (ScanCode)
    {

    //
    // ABC
    //

    case 0x1E:
        return VKeyName::A;
    case 0x30:
        return VKeyName::B;
    case 0x2E:
        return VKeyName::C;
    case 0x20:
        return VKeyName::D;
    case 0x12:
        return VKeyName::E;
    case 0x21:
        return VKeyName::F;
    case 0x22:
        return VKeyName::G;
    case 0x23:
        return VKeyName::H;
    case 0x17:
        return VKeyName::I;
    case 0x24:
        return VKeyName::J;
    case 0x25:
        return VKeyName::K;
    case 0x26:
        return VKeyName::L;
    case 0x32:
        return VKeyName::M;
    case 0x31:
        return VKeyName::N;
    case 0x18:
        return VKeyName::O;
    case 0x19:
        return VKeyName::P;
    case 0x10:
        return VKeyName::Q;
    case 0x13:
        return VKeyName::R;
    case 0x1F:
        return VKeyName::S;
    case 0x14:
        return VKeyName::T;
    case 0x16:
        return VKeyName::U;
    case 0x2F:
        return VKeyName::V;
    case 0x11:
        return VKeyName::W;
    case 0x2D:
        return VKeyName::X;
    case 0x15:
        return VKeyName::Y;
    case 0x2C:
        return VKeyName::Z;
    //
    // Numbers
    //

    case 0x2:
        return VKeyName::One;
    case 0x3:
        return VKeyName::Two;
    case 0x4:
        return VKeyName::Three;
    case 0x5:
        return VKeyName::Four;
    case 0x6:
        return VKeyName::Five;
    case 0x7:
        return VKeyName::Six;
    case 0x8:
        return VKeyName::Seven;
    case 0x9:
        return VKeyName::Eight;
    case 0x0A:
        return VKeyName::Nine;
    case  0x0B:
        return VKeyName::Zero;

    //
    // Other
    //

    case 0x1C:
        return VKeyName::Enter;
    case 0x01:
        return VKeyName::Esc;
    case 0x0E:
        return VKeyName::Backspace;
    case 0x0F:
        return VKeyName::Tab;
    case 0x39:
        return VKeyName::Space;
    case 0x3A:
        return VKeyName::CapsLock;
    case 0xD1:
        return VKeyName::LeftCtrl;
    case 0xE01D:
        return VKeyName::RightCtrl;
    case 0x1D:
        return VKeyName::Ctrl;
    case 0x2A:
        return VKeyName::LeftShift;
    case 0x36:
        return VKeyName::RightShift;
    case 0x5B:
        return VKeyName::LeftWin;
    case 0x5C:
        return VKeyName::RightWin;
    case 0xE037:
        return VKeyName::PrintScreen;
    case 0x46:
        return VKeyName::ScrollLock;
    case 0xE05F:
        return VKeyName::Sleep;
    case 0xE052:
        return VKeyName::Insert;
    case 0xE047:
        return VKeyName::Home;
    case 0xE053:
        return VKeyName::Delete;
    case 0xE04F:
        return VKeyName::End;
    case 0xE051:
        return VKeyName::NextTrack;
    case 0xE04D:
        return VKeyName::RightArrow;
    case 0xE04B:
        return VKeyName::LeftArrow;
    case 0xE050:
        return VKeyName::DownArrow;
    case 0xE048:
        return VKeyName::UpArrow;
    case 0x45:
        return VKeyName::NumLock;
    case 0xE035:
        return VKeyName::Separator;
    case 0x37:
        return VKeyName::Multiply;
    case 0x4A:
        return VKeyName::Subtract;
    case 0x4E:
        return VKeyName::Add;
    case 0xE01C:
        return VKeyName::Enter;
    case 0x4F:
        return VKeyName::One;
    case 0x50:
        return VKeyName::Two;
    case 0x51:
        return VKeyName::Three;
    case 0x4B:
        return VKeyName::Four;
    case 0x4C:
        return VKeyName::Five;
    case 0x4D:
        return VKeyName::Six;
    case 0x47:
        return VKeyName::Seven;
    case 0x48:
        return VKeyName::Eight;
    case 0x49:
        return VKeyName::Nine;
    case 0x52:
        return VKeyName::Zero;
    case 0xE11D45:
    case 0xE19DC5:
        return VKeyName::Pause;
    case 0x0C:
        return VKeyName::Subtract;
    case 0x0D:
        return VKeyName::Add;
    case 0x3B:
        return VKeyName::F1;
    case 0x3C:
        return VKeyName::F2;
    case 0x3D:
        return VKeyName::F3;
    case 0x3E:
        return VKeyName::F4;
    case 0x3F:
        return VKeyName::F5;
    case 0x40:
        return VKeyName::F6;
    case 0x41:
        return VKeyName::F7;
    case 0x42:
        return VKeyName::F8;
    case 0x43:
        return VKeyName::F9;
    case 0x44:
        return VKeyName::F10;
    case 0x57:
        return VKeyName::F11;
    case 0x58:
        return VKeyName::F12;
    case 0x64:
        return VKeyName::F13;
    case 0x65:
        return VKeyName::F14;
    case 0x66:
        return VKeyName::F15;
    case 0x67:
        return VKeyName::F16;
    case 0x68:
        return VKeyName::F17;
    case 0x69:
        return VKeyName::F18;
    case 0x6A:
        return VKeyName::F19;
    case 0x6B:
        return VKeyName::F20;
    case 0x6C:
        return VKeyName::F21;
    case 0x6D:
        return VKeyName::F22;
    case 0x6E:
        return VKeyName::F23;
    case 0x76:
        return VKeyName::F24;
    }

    return VKeyName::Unknown;
}