#include "pch.hpp"

#include "DistanceApp.hpp"
#include "ScreenshotDlg.hpp"
#include "afxdialogex.h"

#include <filesystem>


xstar::tstring GenerateFileName(
    xstar::tstring_view path,
    xstar::tstring_view fileName,
    xstar::tstring_view timeFormat
);

IMPLEMENT_DYNAMIC(ScreenshotDlg, CDialogEx)

ScreenshotDlg::ScreenshotDlg(
    std::shared_ptr<Distance::Network::DefaultTcpServer> server,
    size_t clientID,
    CWnd* pParent /*=nullptr*/
)
    : folderName_($T"screenshots\\"), server_(std::move(server)), clientID_(clientID), 
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
    MessageHeader header{ ServerMsgType::GetScreenshot, 0 };

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

    std::vector<std::byte> bmpImage;
    message_ >> bmpImage;

    // Create folder
    if (!std::filesystem::is_directory(folderName_))
    {
        if (!std::filesystem::create_directory(folderName_))
        {
            MessageBox(
                $T"Failed to create \"screenshots\" directory!", 
                $T"Distance", 
                MB_ICONERROR
            );
			return;
        }
    }

    // Save screenshot to file
    CFile file;
    if (file.Open(
        GenerateFileName(folderName_, $T"screen-", $T"%F_%H-%M-%S").c_str(), 
        CFile::modeCreate | CFile::modeReadWrite
    ))
    {
        file.Write(bmpImage.data(), static_cast<UINT>(bmpImage.size()));
        file.Close();
    }
    else
    {
        MessageBox(
            $T"Failed to open file, for save screenshots!", 
            $T"Distance", 
            MB_ICONERROR
        );
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
        MessageBox(
            $T"Failed to get the path to the current directory!", 
            $T"Distance", 
            MB_ICONERROR
        );
    }

    xstar::tostringstream commandLine;
    commandLine << $T"-e " << pathToFolder << $T"\\" << folderName_;
    
    if (!CreateProcess(
        $T"C:\\Windows\\explorer.exe",
        const_cast<LPTSTR>(commandLine.str().c_str()),
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &startupInfo,
        &processInfo
    ))
		MessageBox($T"Failed to open folder!", $T"Distance", MB_ICONERROR);
    
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
}



xstar::tstring GenerateFileName(
    xstar::tstring_view path, 
    xstar::tstring_view fileName, 
    xstar::tstring_view timeFormat
)
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

#pragma warning(push)
#pragma warning(disable : 4996)
    xstar::tstringstream ss;
    ss << path << fileName
        << std::put_time(std::localtime(&time), timeFormat.data()) << _T(".bmp");
    return ss.str();
#pragma warning(pop)
}
