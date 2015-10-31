
// ScreenTranslateDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>


// CScreenTranslateDlg dialog
class CScreenTranslateDlg : public CDialogEx
{
// Construction
public:
	CScreenTranslateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SCREENTRANSLATE_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_programs;
    afx_msg void OnBnClickedButtonRefresh();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    std::vector<std::vector<COLORREF>> m_matrix;
};
