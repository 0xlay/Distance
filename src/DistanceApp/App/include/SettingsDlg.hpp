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
// File: SettingsDlg.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#pragma once


namespace Distance
{

    class SettingsDlg : public CDialogEx
    {
        DECLARE_DYNAMIC(SettingsDlg)

    public:
        SettingsDlg(CWnd* pParent = nullptr);
        virtual ~SettingsDlg();

        // Dialog Data
#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_SETTINGS_DIALOG };
#endif

        afx_msg void OnBnClickedSettingsSave();
        afx_msg void OnBnClickedSettingsCancel();

    protected:
        void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
        BOOL OnInitDialog() override;
        DECLARE_MESSAGE_MAP()

    private:
        CStringW port_;
        CComboBox protocols_;
    };

} // Distance