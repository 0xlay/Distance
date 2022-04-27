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
// File: ProcessManagerDlg.hpp
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

    class ProcessManagerDlg : public CDialogEx
    {
        DECLARE_DYNAMIC(ProcessManagerDlg)

    public:
        ProcessManagerDlg(
            std::shared_ptr<Network::DefaultTcpServer> server,
            size_t clientID,
            CWnd* pParent = nullptr
        );
        ~ProcessManagerDlg();

        afx_msg void OnHdnClickProcessManagerList(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnBnClickedUpdate();
        afx_msg void OnBnClickedTerminate();
        afx_msg void OnBnClickedSave();
        afx_msg void OnBnClickedSaveAs();

#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_PROCESS_MANAGER_DIALOG };
#endif

    protected:
        void DoDataExchange(CDataExchange* pDX) override;
        BOOL OnInitDialog() override;
        DECLARE_MESSAGE_MAP()

    private:
        friend int CALLBACK Compare(LPARAM lhs, LPARAM rhs, LPARAM pThis);
        void saveToFile();

    private:
        Network::Message message_;
        std::shared_ptr<Network::DefaultTcpServer> server_;
        CListCtrl list_;
        size_t clientID_;

        Network::Json::ProcessList processList_;

        CStringW fileName_;

        int sortColumn_{};
        bool isSortAscending{ true };
    };

    int CALLBACK Compare(LPARAM lhs, LPARAM rhs, LPARAM pThis);

} // Distance