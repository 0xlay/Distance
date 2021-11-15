// NetfilterDlg.cpp : implementation file
//

#include "pch.hpp"

#include "DistanceApp.hpp"
#include "NetfilterDlg.hpp"
#include "afxdialogex.h"


// NetfilterDlg dialog

IMPLEMENT_DYNAMIC(NetfilterDlg, CDialogEx)

NetfilterDlg::NetfilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NETFILTER_DIALOG, pParent)
	, log_(_T(""))
{

}

NetfilterDlg::~NetfilterDlg()
{
}

void NetfilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NETFILTER_EDIT, log_);
}


BEGIN_MESSAGE_MAP(NetfilterDlg, CDialogEx)
	ON_BN_CLICKED(ID_NETFILTER_UPDATE, &NetfilterDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(ID_NETFILTER_SAVE, &NetfilterDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_NETFILTER_SAVEAS, &NetfilterDlg::OnBnClickedSaveAs)
END_MESSAGE_MAP()


// NetfilterDlg message handlers


void NetfilterDlg::OnBnClickedUpdate()
{
	// TODO: Add your control notification handler code here
}


void NetfilterDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
}


void NetfilterDlg::OnBnClickedSaveAs()
{
	// TODO: Add your control notification handler code here
}
