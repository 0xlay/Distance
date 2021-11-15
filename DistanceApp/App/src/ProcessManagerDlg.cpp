// ProcessManagerDlg.cpp : implementation file
//

#include "pch.hpp"

#include "DistanceApp.hpp"
#include "ProcessManagerDlg.hpp"
#include "afxdialogex.h"


// ProcessManagerDlg dialog

IMPLEMENT_DYNAMIC(ProcessManagerDlg, CDialogEx)

ProcessManagerDlg::ProcessManagerDlg(
	std::shared_ptr<Distance::Network::DefaultTcpServer> server, 
	size_t clientID,
	CWnd* pParent /*=nullptr*/
)
	: server_(std::move(server)), clientID_(clientID), CDialogEx(IDD_PROCESS_MANAGER_DIALOG, pParent)
{ }

ProcessManagerDlg::~ProcessManagerDlg()
{ }

void ProcessManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_MANAGER_LIST, list_);
}


BEGIN_MESSAGE_MAP(ProcessManagerDlg, CDialogEx)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &ProcessManagerDlg::OnHdnClickProcessManagerList)
	ON_BN_CLICKED(ID_PROCESS_MANAGER_UPDATE, &ProcessManagerDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(ID_PROCESS_MANAGER_TERMINATE, &ProcessManagerDlg::OnBnClickedTerminate)
	ON_BN_CLICKED(ID_PROCESS_MANAGER_SAVE, &ProcessManagerDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_PROCESS_MANAGER_SAVEAS, &ProcessManagerDlg::OnBnClickedSaveAs)
END_MESSAGE_MAP()


BOOL ProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	list_.InsertColumn(0, $T"Process name", LVCFMT_LEFT, 350);
	list_.InsertColumn(1, $T"Process ID", LVCFMT_CENTER, 350);
	list_.InsertColumn(2, $T"Quantity threads", LVCFMT_CENTER, 350);

	return TRUE;
}

void ProcessManagerDlg::OnHdnClickProcessManagerList(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	
	isSortAscending = (phdr->iItem != sortColumn_ ? true, sortColumn_ = phdr->iItem : false);

	list_.SortItemsEx(Compare, reinterpret_cast<DWORD_PTR>(this));
	
	*pResult = 0;
}

void ProcessManagerDlg::OnBnClickedUpdate()
{
	using namespace Distance::Network;
	MessageHeader header{ ServerMsgType::GetProcessList, 0 };

	// Send the query GetProcessList to the client
	if (!server_->send(header, clientID_))
	{
		MessageBox(
			$T"Send message to the client was failed! "
			$T"The client may have been disconnected from the server.",
			$T"Distance", MB_ICONERROR
		);

		EndDialog(FAILED_CONNECTION_TO_CLIENT);
		return;
	}

	// Get process list from the client
	if (!server_->receiveAll(message_, clientID_))
	{
		MessageBox(
			$T"Receive message from the client was failed! "
			$T"The client may have been disconnected from the server.",
			$T"Distance", MB_ICONERROR
		);

		EndDialog(FAILED_CONNECTION_TO_CLIENT);
		return;
	}

	std::string jsonProcessList;
	message_ >> jsonProcessList;

	Json::unpack(jsonProcessList, processList_);

	// Clear list_
	list_.DeleteAllItems();
	
	// Insert data in processList_
	for (const auto& process : processList_)
	{
		// Insert in back
		auto index = list_.InsertItem(list_.GetItemCount(), process.szExeFile, -1);
		list_.SetItemText(index, 1, xstar::to_tstring(process.th32ProcessID).c_str());
		list_.SetItemText(index, 2, xstar::to_tstring(process.cntThreads).c_str());

		list_.SetItemData(index, reinterpret_cast<DWORD_PTR>(&process));
	}
}

void ProcessManagerDlg::OnBnClickedTerminate()
{
	using namespace Distance::Network;

	if (!list_.GetSelectedCount())
	{
		MessageBox($T"", $T"Info", MB_OK | MB_ICONINFORMATION);
		return;
	}

	POSITION pos = list_.GetFirstSelectedItemPosition();

	if (pos)
	{
		int index = list_.GetNextSelectedItem(pos);
		auto process = reinterpret_cast<PROCESSENTRY32*>(list_.GetItemData(index));
		if (!process)
		{
			return;
		}

		message_.setMsgType(ServerMsgType::TerminateProcess);
		message_ << process->th32ProcessID;

		if (!server_->sendAll(message_, clientID_))
		{
			MessageBox(
				$T"Send message to the client was failed! "
				$T"The client may have been disconnected from the server.",
				$T"Distance", MB_ICONERROR
			);

			EndDialog(FAILED_CONNECTION_TO_CLIENT);
			return;
		}

		OnBnClickedUpdate();
	}
}

void ProcessManagerDlg::OnBnClickedSave()
{
	if (!fileName_.Compare($T""))
		OnBnClickedSaveAs();
	else
		saveToFile();
}

void ProcessManagerDlg::OnBnClickedSaveAs()
{
	TCHAR filters[] = $T"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";

	/* FALSE - save to file, TRUE - open file*/
	CFileDialog dlg(FALSE, $T"txt", $T"process_list.txt", OFN_EXPLORER | OFN_OVERWRITEPROMPT, filters);
	if (dlg.DoModal() == IDOK)
	{
		fileName_ = dlg.m_pOFN->lpstrFile;
		saveToFile();
	}
}

void ProcessManagerDlg::saveToFile()
{
	CFile file;
	if (!file.Open(fileName_, CFile::modeWrite | CFile::modeCreate))
	{
		CString msg;
		msg.Format($T"Failed save to file \'%s\' !", fileName_.GetString());
		MessageBox(msg, $T"Error", MB_OK | MB_ICONERROR);
		return;
	}

	constexpr int column = 3;
	constexpr int processNameWidth = 50;
	constexpr int processIdWidth = 30;
	constexpr int quantityThreadsWidth = 30;

	using Table = std::array<std::wstring, column>;
	Table header = { L"Process name", L"Process ID", L"Quantity threads" };
	
	std::wstringstream wss;
	wss.setf(std::ios::left, std::ios::adjustfield);
	
	auto print = [&wss, &processNameWidth, &processIdWidth, &quantityThreadsWidth](const Table& table)
	{
		const auto& [processName, processId, quantityThreads] = table;

		wss << std::setw(processNameWidth)		<< L"| " + processName		<< L'|';
		wss << std::setw(processIdWidth)		<< L' '	 + processId		<< L'|';
		wss << std::setw(quantityThreadsWidth)	<< L' '  + quantityThreads	<< L'|';
		wss << std::endl;
	};

	auto print_line = [&wss, &processNameWidth, &processIdWidth, &quantityThreadsWidth, &column]()
	{
		wss.width(processNameWidth + processIdWidth + quantityThreadsWidth + column);
		wss.fill('-');
		wss << '-' << '\n';
		wss.fill(' ');
	};


	print_line();
	print(header);
	print_line();
	
	for (const auto& item : processList_)
		print({ item.szExeFile, std::to_wstring(item.th32ProcessID), std::to_wstring(item.cntThreads) });

	print_line();


	std::wstring processesTable = wss.str();
	file.Write(processesTable.c_str(), static_cast<UINT>(processesTable.length() * sizeof(wchar_t)));
	
	file.Close();
}



int CALLBACK Compare(LPARAM lhs, LPARAM rhs, LPARAM pThis)
{
	auto dlgProcessManager = reinterpret_cast<ProcessManagerDlg*>(pThis);
	CString lhsItem = dlgProcessManager->list_.GetItemText(static_cast<int>(lhs), dlgProcessManager->sortColumn_);
	CString rhsItem = dlgProcessManager->list_.GetItemText(static_cast<int>(rhs), dlgProcessManager->sortColumn_);

	return lhsItem.Compare(rhsItem);
}