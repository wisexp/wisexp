
// ScreenTranslateDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <unordered_map>


typedef std::vector<std::vector<COLORREF>> MATRIX;
typedef std::vector<std::pair<int, int>> Char_T;
typedef std::vector<Char_T> Line_T;

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

    
    bool ReadCharacter(std::vector<std::pair<int, int>>& character, wchar_t& c);
    void SaveCharacter(std::vector<std::pair<int, int>>& character, wchar_t c);
    void LoadCharMap();
    std::vector<std::wstring> Parse(const MATRIX& baseline, const MATRIX& current);
    
    MATRIX m_lastmatrix;
    MATRIX m_currentmatrix;
    MATRIX m_diff;
    std::vector<Line_T> m_characters;
    std::vector<Line_T>::iterator m_lineIt;
    Line_T::iterator m_charIt;
    std::vector<std::wstring> m_lastParsedText;
    //size_t m_width;
    //size_t m_height;
    int m_left, m_right, m_top, m_bottom;
    afx_msg void OnBnClickedButtonNext();
    afx_msg void OnBnClickedButtonConfirm();
    struct Char_Hash
    {
        int operator()(const Char_T& character);
    };
    std::unordered_map<Char_T, wchar_t, Char_Hash> m_charmap;
    
    CEdit m_char;
    afx_msg void OnBnClickedButtonShowChar();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

