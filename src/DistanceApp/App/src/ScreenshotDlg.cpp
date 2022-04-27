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
// File: ScreenshotDlg.cpp
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
#include "ScreenshotDlg.hpp"
#include "afxdialogex.h"
#include <filesystem>


std::wstring GenerateFileName(std::wstring_view path,
                              std::wstring_view fileName,
                              std::wstring_view timeFormat,
                              std::wstring_view extension);


namespace Distance
{

    IMPLEMENT_DYNAMIC(ScreenshotDlg, CDialogEx)

    ScreenshotDlg::ScreenshotDlg(
        std::shared_ptr<Distance::Network::DefaultTcpServer> server,
        size_t clientID,
        CWnd* pParent /*=nullptr*/
    )
        : folderName_(L"screenshots\\"), server_(std::move(server)),
          clientID_(clientID),
          CDialogEx(IDD_SCREENSHOT_DIALOG, pParent)
    { }

    void ScreenshotDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(ScreenshotDlg, CDialogEx)
        ON_BN_CLICKED(ID_SCREENSHOT_CREATE, &ScreenshotDlg::OnBnClickedCreate)
        ON_BN_CLICKED(ID_SCREENSHOT_OPEN_FOLDER, &ScreenshotDlg::OnBnClickedOpenFolder)
    END_MESSAGE_MAP()

    BOOL ScreenshotDlg::OnInitDialog()
    {
        CDialogEx::OnInitDialog();
        return TRUE;
    }

    void ScreenshotDlg::OnBnClickedCreate()
    {
        using namespace Distance::Network;
        message_.clear();
        message_.setMsgType(MsgType::GetScreenshot);
        
        // Send the query GetProcessList to the client
        if (!server_->send(message_, clientID_))
        {
            MessageBoxW(L"Send message to the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance", MB_ICONERROR);

            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }

        // Get process list from the client
        if (!server_->receive(message_, clientID_))
        {
            MessageBoxW(L"Receive message from the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance", MB_ICONERROR);

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

        std::vector<char> pngImage;
        message_ >> pngImage;

        // Create folder
        if (!std::filesystem::is_directory(folderName_))
        {
            if (!std::filesystem::create_directory(folderName_))
            {
                MessageBoxW(L"Failed to create \"screenshots\" directory!",
                            L"Distance", MB_ICONERROR);
                return;
            }
        }

        // Save screenshot to file
        CFile file;
        if (file.Open(::GenerateFileName(folderName_, 
                                         L"screen-", L"%F_%H-%M-%S", L".png").c_str(),
                                         CFile::modeCreate | CFile::modeReadWrite))
        {
            file.Write(pngImage.data(), static_cast<UINT>(pngImage.size()));
            file.Close();
        }
        else
        {
            MessageBoxW(L"Failed to open file, for save screenshots!",
                        L"Distance", MB_ICONERROR);
        }
    }

    void ScreenshotDlg::OnBnClickedOpenFolder()
    {
        STARTUPINFO startupInfo{};
        PROCESS_INFORMATION processInfo{};
        startupInfo.cb = sizeof(startupInfo);

        TCHAR pathToFolder[MAX_PATH];
        if (!GetCurrentDirectory(MAX_PATH, pathToFolder))
        {
            MessageBoxW(L"Failed to get the path to the current directory!",
                        L"Distance",
                        MB_ICONERROR);
        }

        std::wostringstream commandLine;
        commandLine << L"-e " << pathToFolder << L"\\" << folderName_;

        if (!CreateProcessW(L"C:\\Windows\\explorer.exe",
                            const_cast<LPWSTR>(commandLine.str().c_str()),
                            nullptr,
                            nullptr,
                            FALSE,
                            0,
                            nullptr,
                            nullptr,
                            &startupInfo,
                            &processInfo))
        {
            MessageBoxW(L"Failed to open folder!", L"Distance", MB_ICONERROR);
        }

        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
    }

} // Distance


std::wstring GenerateFileName(std::wstring_view path, 
                              std::wstring_view fileName, 
                              std::wstring_view timeFormat,
                              std::wstring_view extension)
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

#pragma warning(push)
#pragma warning(disable : 4996)
    std::wostringstream woss;
    woss << path << fileName
         << std::put_time(std::localtime(&time), timeFormat.data()) 
         << extension;
    return woss.str();
#pragma warning(pop)
}
