
// Text2GuidDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CText2GuidDlg dialog
class CText2GuidDlg : public CDialogEx
{
// Construction
public:
	CText2GuidDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXT2GUID_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnEncrypt();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtInput;
	CEdit m_edtGuid;
	CEdit m_edtMagic;
	CButton m_btnEncrypt;
	BOOL m_bEncrypting;
};
