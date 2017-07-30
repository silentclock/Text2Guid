
// Text2GuidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Text2Guid.h"
#include "Text2GuidDlg.h"
#include "afxdialogex.h"
#include "md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// Implementation
	CBrush m_brBk;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BMP_AUTHOR);
	{
		BITMAP	bm;
		bitmap.GetBitmap(&bm);
		this->SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
	}
	m_brBk.CreatePatternBrush(&bitmap);
	bitmap.DeleteObject();

	return TRUE;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd == this)
	{
		return m_brBk;
	}

	return hbr;
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CText2GuidDlg dialog

UINT Text2GuidThread(LPVOID pParam) {
#define MAGIC_MODE_NUMBER 997
#define TEXT2GUID_SEPARATOR TEXT("##")
	CText2GuidDlg* pDlg = (CText2GuidDlg*)pParam;
	if (pDlg && (!pDlg->m_bEncrypting)) {
		pDlg->m_bEncrypting = TRUE;

		CString strInputOriginal, strMagic;
		int nMagic = 0;

		pDlg->m_edtInput.GetWindowText(strInputOriginal);
		pDlg->m_edtMagic.GetWindowText(strMagic);
		nMagic = _tstoi(strMagic);
		nMagic = nMagic % MAGIC_MODE_NUMBER + 2; //2-998

		std::string result;
		std::wstring resultW;
		CString strInput = strInputOriginal;
		for (int ii = 0;ii < nMagic - 1;ii++) {
			result = GetMD5(strInput, strInput.GetLength() * sizeof(TCHAR));
#ifdef UNICODE
			{
				int nLen = (int)result.length();
				resultW.resize(nLen, L'\0');
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)result.c_str(), nLen, (LPWSTR)resultW.c_str(), nLen);
				strInput.Format(TEXT("%s%s%s"), resultW.c_str(), TEXT2GUID_SEPARATOR, strInputOriginal);
			}
#else
			strInput.Format(TEXT("%s%s%s"), result, TEXT2GUID_SEPARATOR, strInput);
#endif // UNICODE
		}

		result = GetMD5Guid(strInput, strInput.GetLength() * sizeof(TCHAR));

		pDlg->m_bEncrypting = FALSE;
		::SetWindowTextA(pDlg->m_edtGuid.GetSafeHwnd(), result.c_str());
		pDlg->m_btnEncrypt.EnableWindow(TRUE);
	}

	return 0;
}

CText2GuidDlg::CText2GuidDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXT2GUID_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_bEncrypting = FALSE;
}

void CText2GuidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_edtInput);
	DDX_Control(pDX, IDC_EDIT_GUID, m_edtGuid);
	DDX_Control(pDX, IDC_EDIT_MAGIC, m_edtMagic);
	DDX_Control(pDX, IDC_BTN_ENCRYPT, m_btnEncrypt);
}

BEGIN_MESSAGE_MAP(CText2GuidDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ENCRYPT, &CText2GuidDlg::OnBnClickedBtnEncrypt)
END_MESSAGE_MAP()


// CText2GuidDlg message handlers

BOOL CText2GuidDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

void CText2GuidDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
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

void CText2GuidDlg::OnPaint()
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
HCURSOR CText2GuidDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CText2GuidDlg::OnBnClickedBtnEncrypt()
{
	this->m_btnEncrypt.EnableWindow(FALSE);
	this->m_edtGuid.SetWindowText(TEXT("calculating ..."));

	AfxBeginThread(Text2GuidThread, this);
}


void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->EndDialog(TRUE);

	CDialogEx::OnLButtonDown(nFlags, point);
}
