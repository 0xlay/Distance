
// MainDlg.cpp : implementation file
//
#include "pch.hpp"

#include "framework.hpp"

#include "DistanceApp.hpp"
#include "MainDlg.hpp"
#include "SettingsDlg.hpp"
#include "ScreenshotDlg.hpp"
#include "ProcessManagerDlg.hpp"
#include "PowerManagerDlg.hpp"
#include "NetfilterDlg.hpp"
#include "KeyloggerDlg.hpp"
#include "SystemInfoDlg.hpp"

#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class AboutDlg : public CDialogEx
{
public:
	AboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

AboutDlg::AboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialogEx)
END_MESSAGE_MAP()


// MainDlg dialog



MainDlg::MainDlg(
	std::shared_ptr<Distance::Network::DefaultTcpServer> server,
	Distance::Network::Message& msg, 
	CWnd* pParent /*=nullptr*/
)
	: CDialogEx(IDD_DISTANCEAPP_DIALOG, pParent), server_(server), message_(msg), counterPC_(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINICON);
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
	ON_BN_CLICKED(IDC_BUTTON_LIST_UPDATE, &MainDlg::OnBnClickedButtonListUpdate)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_MANAGER, &MainDlg::OnBnClickedButtonProcessManager)
	ON_BN_CLICKED(IDC_BUTTON_POWER_MANAGER, &MainDlg::OnBnClickedButtonPowerManager)
	ON_BN_CLICKED(IDC_BUTTON_SCREENSHOT, &MainDlg::OnBnClickedButtonScreenshot)
	ON_BN_CLICKED(IDC_BUTTON_KEYLOGGER, &MainDlg::OnBnClickedButtonKeylogger)
	ON_BN_CLICKED(IDC_BUTTON_NETFILTER, &MainDlg::OnBnClickedButtonNetfilter)
	ON_BN_CLICKED(IDC_BUTTON_SYSINFO, &MainDlg::OnBnClickedButtonSysinfo)
END_MESSAGE_MAP()


// MainDlg message handlers

BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	images_.Create(32, 32, ILC_COLOR, 0, 0);
	images_.Add(AfxGetApp()->LoadIcon(IDI_PC_ICON));
	listPC_.SetImageList(&images_, LVSIL_NORMAL);
	// listPC_.InsertItem(listPC_.GetItemCount() /*push back*/, $T"PC0", 0/*icon index*/);
	// listPC_.InsertItem(listPC_.GetItemCount() /*push back*/, $T"PC1", 0/*icon index*/);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

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
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void MainDlg::addPcToList()
{
	while (true)
	{
		if (server_->clientsQuantity() > listPC_.GetItemCount())
		{
			xstar::tostringstream oss;
			oss << "PC" << counterPC_++;
			listPC_.InsertItem(listPC_.GetItemCount() /*push back*/, oss.str().c_str(), 0/*icon index*/);
			continue;
		}
		break;
	}
}

void MainDlg::removePcFromList()
{
	if (server_->clientsQuantity() && listPC_.GetSelectionMark() < server_->clientsQuantity())
	{
		auto sockets = server_->getSockets();
		sockets->erase(sockets->begin() + listPC_.GetSelectionMark());
		listPC_.DeleteItem(listPC_.GetSelectionMark());
		--counterPC_;
	}
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
		MessageBox(L"Cool!");
	}
	else if (response == ID_SETTINGS_CANCEL)
	{

	}
}


void MainDlg::OnBnClickedButtonListUpdate()
{
	addPcToList();
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
	KeyloggerDlg dlgKeylogger;
	dlgKeylogger.DoModal();
}


void MainDlg::OnBnClickedButtonNetfilter()
{
	NetfilterDlg dlgNetfilter;
	dlgNetfilter.DoModal();
}


void MainDlg::OnBnClickedButtonSysinfo()
{
	SystemInfoDlg dlgSystemInfo(server_, listPC_.GetSelectionMark());
	if (dlgSystemInfo.DoModal() == FAILED_CONNECTION_TO_CLIENT)
	{
		removePcFromList();
	}
}
