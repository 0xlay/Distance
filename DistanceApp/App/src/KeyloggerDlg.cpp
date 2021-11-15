// KeyloggerDlg.cpp : implementation file
//
#include "pch.hpp"

#include "DistanceApp.hpp"
#include "KeyloggerDlg.hpp"
#include "afxdialogex.h"


// KeyloggerDlg dialog

IMPLEMENT_DYNAMIC(KeyloggerDlg, CDialogEx)

KeyloggerDlg::KeyloggerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEYLOGGER_DIALOG, pParent)
{

}

KeyloggerDlg::~KeyloggerDlg()
{
}

void KeyloggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KeyloggerDlg, CDialogEx)
	ON_BN_CLICKED(ID_KEYLOGGER_UPDATE, &KeyloggerDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(ID_KEYLOGGER_SAVE, &KeyloggerDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_KEYLOGGER_SAVEAS, &KeyloggerDlg::OnBnClickedSaveAs)
END_MESSAGE_MAP()


// KeyloggerDlg message handlers


void KeyloggerDlg::OnBnClickedUpdate()
{
	// TODO: Add your control notification handler code here
}


void KeyloggerDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
}


void KeyloggerDlg::OnBnClickedSaveAs()
{
	// TODO: Add your control notification handler code here
}
