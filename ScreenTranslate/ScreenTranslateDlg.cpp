
// ScreenTranslateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenTranslate.h"
#include "ScreenTranslateDlg.h"
#include "afxdialogex.h"
#include <gdiplus.h>
#include <algorithm>
#include <list>
#include <fstream>
#include "Property.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <regex>
#include <sstream>
//#define DUMP

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScreenTranslateDlg dialog



CScreenTranslateDlg::CScreenTranslateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenTranslateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_lineIt = m_characters.end();
}

void CScreenTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PROGRAMLIST, m_programs);
    DDX_Control(pDX, IDC_EDIT_CHAR, m_char);
}

BEGIN_MESSAGE_MAP(CScreenTranslateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CScreenTranslateDlg::OnBnClickedButtonRefresh)
    ON_BN_CLICKED(IDC_BUTTON2, &CScreenTranslateDlg::OnBnClickedButton2)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_NEXT, &CScreenTranslateDlg::OnBnClickedButtonNext)
    ON_BN_CLICKED(IDC_BUTTON_CONFIRM, &CScreenTranslateDlg::OnBnClickedButtonConfirm)
    ON_BN_CLICKED(IDC_BUTTON_SHOW_CHAR, &CScreenTranslateDlg::OnBnClickedButtonShowChar)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CScreenTranslateDlg message handlers

BOOL CScreenTranslateDlg::OnInitDialog()
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

void CScreenTranslateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScreenTranslateDlg::OnPaint()
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

#ifdef DUMP

        if (m_lineIt != m_characters.end())
        {

            auto pDC = GetDC();
            pDC->SetStretchBltMode(HALFTONE);

            int shiftx = 0;
            int shifty = 0;
            for (auto lineIt = m_characters.begin(); lineIt <= m_lineIt; ++lineIt)
            {
                shifty += 24;
                shiftx = 0;
                
                for (auto charIt = lineIt->begin(); ; ++charIt)
                {
                    if (lineIt == m_lineIt && charIt >= m_charIt)
                        break;
                    if (charIt == lineIt->end())
                        break;
                    shiftx += 12;
                    for (auto& point : *charIt)
                    {
                        pDC->SetPixel(point.first + shiftx, - point.second + 100 + shifty, 0);
                    }
                    wchar_t str[2] = L" ";
                    str[0] = m_charmap[*charIt];
                    pDC->TextOut(shiftx, shifty + 100, str, 1);
                }
            }
           
            ReleaseDC(pDC);
        }
#endif
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScreenTranslateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CALLBACK WndEnumProc(HWND h, LPARAM p)
{
    CScreenTranslateDlg* pThis = reinterpret_cast<CScreenTranslateDlg*>(p);
    DWORD proccess_ID;
    DWORD exStyle = CWnd::FromHandle(h)->GetStyle();
    DWORD requiredStyle = WS_VISIBLE;
    if ((exStyle & requiredStyle) != requiredStyle)
        return true;

    GetWindowThreadProcessId(h, &proccess_ID);
    if (proccess_ID == 7928)
    {
        int kkk = 0;
    }
    wchar_t buf[80];
    GetWindowText(h, buf, 80);
    int index = pThis->m_programs.GetCount();
    pThis->m_programs.InsertString(index, buf);
    pThis->m_programs.SetItemData(index, reinterpret_cast<DWORD_PTR>(h));
    //std::wstring title(buf);
    //pThis->m_processTitleMap.push_back(std::make_pair(proccess_ID, title));

    return TRUE;
}


void CScreenTranslateDlg::OnBnClickedButtonRefresh()
{
    // TODO: Add your control notification handler code here
    m_programs.ResetContent();
    EnumWindows(WndEnumProc, (LPARAM)this);
}


int CaptureAnImage(HWND hWnd, std::vector<std::vector<COLORREF>>& matrix)
{
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;

    // Retrieve the handle to a display device context for the client 
    // area of the window. 
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hWnd);

    // Create a compatible DC which is used in a BitBlt from the window DC
    hdcMemDC = CreateCompatibleDC(hdcWindow);

    if (!hdcMemDC)
    {
        MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
        goto done;
    }

    // Get the client area for size calculation
    RECT rcClient;
    RECT rcWindow;
    GetClientRect(hWnd, &rcClient);
    GetWindowRect(hWnd, &rcWindow);

    //rcClient.right = rcClient.left + 400;
    //rcClient.bottom = rcClient.top + 400;

    //This is the best stretch mode
    SetStretchBltMode(hdcWindow, HALFTONE);

    ////The source DC is the entire screen and the destination DC is the current window (HWND)
    //if (!StretchBlt(hdcWindow,
    //    0, 0,
    //    rcClient.right, rcClient.bottom,
    //    hdcScreen,
    //    0, 0,
    //    GetSystemMetrics(SM_CXSCREEN),
    //    GetSystemMetrics(SM_CYSCREEN),
    //    SRCCOPY))
    //{
    //    MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
    //    goto done;
    //}

    // Create a compatible bitmap from the Window DC
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

    if (!hbmScreen)
    {
        MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
        goto done;
    }

    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC, hbmScreen);

    // Bit block transfer into our compatible memory DC.
    if (!BitBlt(hdcMemDC,
        0, 0,
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
        hdcScreen,
        rcWindow.left , rcWindow.top ,
        SRCCOPY))
    {
        MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
        goto done;
    }
    
    // Get the BITMAP from the HBITMAP
    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

    

    matrix.resize(bmpScreen.bmHeight, std::vector<COLORREF>(bmpScreen.bmWidth));

    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
    // have greater overhead than HeapAlloc.
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char *lpbitmap = (char *)GlobalLock(hDIB);

    // Gets the "bits" from the bitmap and copies them into a buffer 
    // which is pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // A file is created, this is where we will save the screen capture.
    //HANDLE hFile = CreateFile(L"captureqwsx.bmp",
    //    GENERIC_WRITE,
    //    0,
    //    NULL,
    //    CREATE_ALWAYS,
    //    FILE_ATTRIBUTE_NORMAL, NULL);

    //// Add the size of the headers to the size of the bitmap to get the total file size
    //DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

    ////Offset to where the actual bitmap bits start.
    //bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER);

    ////Size of the file
    //bmfHeader.bfSize = dwSizeofDIB;

    ////bfType must always be BM for Bitmaps
    //bmfHeader.bfType = 0x4D42; //BM   

    //DWORD dwBytesWritten = 0;
    //WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    //WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    //WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    //CBitmap bmp;
    //bmp.Attach(hbmScreen);
    //bmp.

    for (int w = 0; w < bmpScreen.bmWidth; w++)
    {
        for (int h = 0; h < bmpScreen.bmHeight; h++)
        {
            char* buf = lpbitmap + h * bmpScreen.bmWidth + w;
            //union MyUnion
            //{
            //    char buf[4];
            //    DWORD dw;
            //};
            //MyUnion my;
            //my.buf[3] = buf[3];
            //my.buf[2] = buf[0];
            //my.buf[1] = buf[1];
            //my.buf[0] = buf[2];
            matrix[h][w] = reinterpret_cast<DWORD*>(lpbitmap)[h * bmpScreen.bmWidth + w];
        }
    }

    //Unlock and Free the DIB from the heap
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    //Close the handle for the file that was created
    //CloseHandle(hFile);

    //Clean up
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    //ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hWnd, hdcWindow);

    return 0;
}

int GetDelta(const MATRIX& baseline, const MATRIX& current, MATRIX& diff)
{
    size_t width = current[0].size();
    size_t height = current.size();
    diff.resize(height, vector<COLORREF>(width, 0));
    int delta = 0;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            auto c1 = baseline[y][x];
            auto c2 = current[y][x];
            if (c1 != c2)
            {
                delta++;
                diff[y][x] = c2;
            }
            else
            {
                diff[y][x] = 0;
            }
        }
    }
    return delta;
}

void GetDifference(MATRIX& diff, int& left, int& right, int& top, int& bottom)
{
    size_t width = diff[0].size();
    size_t height = diff.size();
    std::vector<int> rows(height, 0);
    std::vector<int> cols(width, 0);
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (diff[y][x] != 0)
            {
                rows[y]++;
                cols[x]++;
            }
        }
    }

    int rowMax = *std::max_element(rows.begin(), rows.end());
    int colMax = *std::max_element(cols.begin(), cols.end());
    int rowAverage = rowMax / 2;
    int colAverage = colMax / 2;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (diff[y][x] != 0 && (rows[y] < rowAverage || cols[x] < colAverage))
            {
                diff[y][x] = 0;
            }
        }
    }

    for (int& r : rows)
        r = 0;
    for (int& c : cols)
        c = 0;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (diff[y][x] != 0)
            {
                rows[y]++;
                cols[x]++;
            }
        }
    }

    auto firstRow = std::find_if(rows.begin(), rows.end(), [](int v)
    {
        return v > 0;
    });

    auto lastRow = std::find_if(firstRow, rows.end(), [](int v)
    {
        return v == 0;
    });

    auto firstCol = std::find_if(cols.begin(), cols.end(), [](int v)
    {
        return v > 0;
    });

    auto lastCol = std::find_if(firstCol, cols.end(), [](int v)
    {
        return v == 0;
    });

    left = std::distance(cols.begin(), firstCol);
    right = std::distance(cols.begin(), lastCol);
    top = std::distance(rows.begin(), firstRow);
    bottom = std::distance(rows.begin(), lastRow);
}

void RemoveNoise(const MATRIX& current, MATRIX& diff, int left, int right, int top, int bottom)
{
    for (int x = left; x < right; x++)
    {
        for (int y = top; y < bottom; y++)
        {
            auto c = current[y][x];
            if (c != 0)
            {
                c &= 0x00ffffff;
                auto r = (c & 0xff0000) >> 16;
                auto g = (c & 0xff00) >> 8;
                auto b = c & 0xff;

                if (r + g + b > 200)
                    ;// pDC->SetPixel(x + 0, bottom - y + 100, 0);
                else
                    c = 0;

                diff[y][x] = c;
            }
        }
    }
}

void Split(MATRIX& diff, int left, int right, int top, int bottom, std::vector<Line_T>& characters)
{
    characters.clear();
    Line_T line;
    int bottomOfLastLine = -1;
    int maxHeightOfCharacter = 10;
    // split characters
    vector<vector<int>> visited(bottom - top, vector<int>(right - left, 0));
    for (int y = top; y < bottom; y++)
    {
        for (int x = left; x < right; x++)
        {
            auto c = diff[y][x];
            if (visited[y - top][x - left]) continue;

            visited[y - top][x - left] = 1;
            if (c != 0)
            {
                int bottomOfCurrentLine = y;
                if (bottomOfLastLine == -1)
                {
                    bottomOfLastLine = bottomOfCurrentLine;
                }

                if (bottomOfCurrentLine - bottomOfLastLine > maxHeightOfCharacter)
                {
                    // new line:
                    std::sort(line.begin(), line.end());
                    characters.emplace_back(std::move(line));
                    bottomOfLastLine = bottomOfCurrentLine;
                }
                std::vector<std::pair<int, int>> character;
                std::vector<pair<int, int>> stack;
                stack.push_back(make_pair(x, y));
                while (!stack.empty())
                {
                    auto point = *stack.rbegin();
                    stack.pop_back();
                    character.push_back(point);
                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -3; j <= 3; j++)
                        {
                            if (i == 0 && j == 0) continue;
                            if (i != 0 && j != 0) continue;
                            if (point.first + i < left || point.first + i >= right) continue;
                            if (point.second + j < top || point.second + j >= bottom) continue;
                            int xx = point.first - left + i;
                            int yy = point.second - top + j;

                            if (!visited[yy][xx])
                            {
                                visited[yy][xx] = 1;
                                auto cc = diff[point.second + j][point.first + i];
                                if (cc != 0)
                                {
                                    stack.push_back(make_pair(point.first + i, point.second + j));
                                }
                            }
                        }
                    }
                }

                if (character.size() < 100)
                {
                    std::sort(character.begin(), character.end());
                    line.emplace_back(std::move(character));
                }
            }
        }
    }

    if (!line.empty())
    {
        std::sort(line.begin(), line.end());
        characters.emplace_back(std::move(line));
    }

    for (auto& l : characters)
    {
        for (auto& character : l)
        {
            if (!character.empty())
            {
                auto origin = character[0];

                for (auto& point : character)
                {
                    if (point.first < origin.first)
                        origin.first = point.first;
                    if (point.second < origin.second)
                        origin.second = point.second;
                }

                for (auto& point : character)
                {
                    point.first -= origin.first;
                    point.second -= origin.second;
                }
            }
        }
    }
}

std::vector<std::wstring> CScreenTranslateDlg::Parse(const MATRIX& baseline, const MATRIX& current)
{
    vector<wstring> results;

    MATRIX diff;
    auto delta = GetDelta(baseline, current, diff);
    size_t width = current[0].size();
    size_t height = current.size();

    if (delta > 10000)
    {
        int left, right, top, bottom;
        GetDifference(diff, left, right, top, bottom);

        // remove noise
        if (left < right && top < bottom)
        {
            RemoveNoise(current, diff, left, right, top, bottom);

            std::vector<Line_T> characters;
            Split(diff, left, right, top, bottom, characters);
            m_characters.swap(characters);
#ifdef DUMP
            m_lineIt = m_characters.begin();
            m_charIt = m_lineIt->begin();
#endif

            
            for (auto& l : m_characters)
            {
                wstring line;
                for (auto& ch : l)
                {
                    wchar_t c;
                    if (ReadCharacter(ch, c))
                    {
                        if (c != L' ')
                            line.push_back(c);
                    }
                    else
                    {
                        //SaveCharacter(ch, ' ');
                    }
                        
                }
                results.push_back(line);
            }
        }
    }
    return results;
}



void CScreenTranslateDlg::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    //SetTimer(0, 5000, nullptr);
    int curSel = m_programs.GetCurSel();
    if (curSel != -1)
    {
        HWND hwnd = reinterpret_cast<HWND>(m_programs.GetItemData(curSel));
        
        CaptureAnImage(hwnd, m_lastmatrix);

        KillTimer(0);
        SetTimer(0, 1000, nullptr);

      /*  if (m_lastmatrix.size() != 0 && m_currentmatrix.size() == m_lastmatrix.size() && m_lastmatrix[0].size() == m_currentmatrix[0].size())
        {
            auto& baseline = m_lastmatrix;
            auto& current = m_currentmatrix;
            auto result = Parse(baseline, current);


            Property prop;
            prop.Parse(result);

            int k = 0;

            m_lastmatrix.clear();
            m_currentmatrix.clear();
        }*/
        
    }
}



void CScreenTranslateDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnTimer(nIDEvent);

    if (nIDEvent == 0)
    {
        int curSel = m_programs.GetCurSel();
        if (curSel != -1)
        {
            HWND hwnd = reinterpret_cast<HWND>(m_programs.GetItemData(curSel));

            CaptureAnImage(hwnd, m_currentmatrix);

            if (m_lastmatrix.size() != 0 && m_currentmatrix.size() == m_lastmatrix.size() && m_lastmatrix[0].size() == m_currentmatrix[0].size())
            {
                auto& baseline = m_lastmatrix;
                auto& current = m_currentmatrix;
                auto result = Parse(baseline, current);

                if (m_lastParsedText != result)
                {
                    m_lastParsedText = result;
                    Property prop;
                    prop.Parse(result);

                    int k = 0;

                }
            }

        }
    }
}


void CScreenTranslateDlg::OnBnClickedButtonShowChar()
{
    // TODO: Add your control notification handler code here

    LoadCharMap();
    std::multimap<wchar_t, std::unordered_map<Char_T, wchar_t, Char_Hash>::iterator> reverseMap;
    for (auto it = m_charmap.begin(); it != m_charmap.end(); ++it)
    {
        reverseMap.insert(std::make_pair(it->second, it));
    }

    auto pDC = GetDC();
    pDC->SetStretchBltMode(HALFTONE);

    int shiftx = 0;
    int shifty = 0;
    for (auto& it : reverseMap)
    {
            shiftx += 12;
            for (auto& point : it.second->first)
            {
                pDC->SetPixel(point.first + shiftx, -point.second + 100 + shifty, 0);
            }
            wchar_t str[2] = L" ";
            str[0] = it.first;
            pDC->TextOut(shiftx, shifty + 110, str, 1);
        
    }

    ReleaseDC(pDC);
}


void CScreenTranslateDlg::OnBnClickedButtonNext()
{
    // TODO: Add your control notification handler code here
    
    m_charIt++;
    if (m_charIt == m_lineIt->end())
    {
        m_lineIt++;
        if (m_lineIt == m_characters.end())
        {
            m_lineIt = m_characters.begin();
        }
        m_charIt = m_lineIt->begin();
    }
    OnBnClickedButtonShowChar();
}


void CScreenTranslateDlg::OnBnClickedButtonConfirm()
{
    // TODO: Add your control notification handler code here
    wchar_t str[100] = L"";
    m_char.GetWindowText(str, 100);
    if (str[0] != 0 && str[0] != L' ' && !m_charIt->empty())
    {
        SaveCharacter(*m_charIt, str[0]);
    }
}


int CScreenTranslateDlg::Char_Hash::operator()(const Char_T& character)
{
    return character.size();
}

void CScreenTranslateDlg::LoadCharMap()
{
    m_charmap.clear();
    std::wifstream ifs("out.txt");
    size_t size;
    wchar_t dummy;
    wchar_t ch;
    ifs >> std::noskipws;
    do
    {
        ifs >> size;
        if (!ifs.good())
            break;
        ifs >> dummy;
        ifs >> ch;
        std::vector<std::pair<int, int>> points;
        points.resize(size);
        for (size_t i = 0; i < size; i++)
        {
            ifs >> dummy;
            ifs >> points[i].first;
            ifs >> dummy;
            ifs >> points[i].second;
        }
        ifs >> dummy;
        m_charmap.insert(make_pair(points, ch));
    } while (ifs.good());
    ifs.close();
}

bool CScreenTranslateDlg::ReadCharacter(std::vector<std::pair<int, int>>& character, wchar_t& c)
{
    if (m_charmap.empty())
    {
        LoadCharMap();
    }
    auto it = m_charmap.find(character);
    if (it != m_charmap.end())
    {
        c = it->second;
        return true;
    }
    return false;
}

void CScreenTranslateDlg::SaveCharacter(std::vector<std::pair<int, int>>& character, wchar_t c)
{
    m_charmap.insert(make_pair(character, c));
    std::wofstream ofs("out.txt", std::ofstream::app);
    ofs << character.size() << L',' << c;
    for (auto& point : character)
    {
        ofs << L',' << point.first << L'-' << point.second;
    }
    ofs << L"\n";
    ofs.close();
}






