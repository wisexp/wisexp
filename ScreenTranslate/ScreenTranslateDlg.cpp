
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
#include <assert.h>
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

HWND m_hwnd;

void CScreenTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PROGRAMLIST, m_programs);
    DDX_Control(pDX, IDC_EDIT_CHAR, m_char);
    DDX_Control(pDX, IDC_EDIT1, m_cord);
    DDX_Control(pDX, IDC_BUTTON_START, m_startButton);
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
    ON_WM_KEYDOWN()
    ON_BN_CLICKED(IDC_BUTTON_START, &CScreenTranslateDlg::OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_START2, &CScreenTranslateDlg::OnBnClickedButtonStart2)
    ON_WM_HOTKEY()
END_MESSAGE_MAP()


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

    if (wcscmp(buf, L"Belzebub") == 0)
    {
        m_hwnd = h;
    }

    return TRUE;
}



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

    EnumWindows(WndEnumProc, (LPARAM)this);
    
    RegisterHotKey(
        m_hWnd,
        1,
        MOD_CONTROL|MOD_ALT,
        'S');
    
    m_state = State::Stopped;

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
        m_restartRequired = true;
      /*  int curSel = m_programs.GetCurSel();
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
        */
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

void MoveMouse(HWND hWnd, POINT pt)
{
    RECT rcWindow;
    GetWindowRect(hWnd, &rcWindow);
    long fScreenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
    long fScreenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;
    pt.x += rcWindow.left;
    pt.y += rcWindow.top;

    // http://msdn.microsoft.com/en-us/library/ms646260(VS.85).aspx
    // If MOUSEEVENTF_ABSOLUTE value is specified, dx and dy contain normalized absolute coordinates between 0 and 65,535.
    // The event procedure maps these coordinates onto the display surface.
    // Coordinate (0,0) maps onto the upper-left corner of the display surface, (65535,65535) maps onto the lower-right corner.
    float fx = pt.x * (65535.0f / fScreenWidth);
    float fy = pt.y * (65535.0f / fScreenHeight);
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
    input.mi.dx = fx;
    input.mi.dy = fy;

    SendInput(1, &input, sizeof(INPUT));
}

void LeftClick()
{
    INPUT input = { 0 };

    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}



void KeyEvent(WORD key, bool isExt, bool isDown = true)
{
    INPUT input = { 0 };

    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = isDown ? 0 //keydown 
        : KEYEVENTF_KEYUP;
    input.ki.wVk = key;
    input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
    if (isExt)
        input.ki.dwFlags += KEYEVENTF_EXTENDEDKEY;
    ::BringWindowToTop(m_hwnd);
    SendInput(1, &input, sizeof(INPUT));
}

void KeyPress(WORD key, bool ext = false)
{
    KeyEvent(key, ext);
    Sleep(100);
    KeyEvent(key, ext, false);
}


void CScreenTranslateDlg::OnBnClickedButtonNext()
{
    // TODO: Add your control notification handler code here
    wchar_t cord[100];
    m_cord.GetWindowText(cord, 99);
    
    POINT pt;

    swscanf_s(cord, L"%d,%d", &pt.x, &pt.y);
    MoveMouse(m_hwnd, pt);
    //LeftClick();
    
    //m_charIt++;
    //if (m_charIt == m_lineIt->end())
    //{
    //    m_lineIt++;
    //    if (m_lineIt == m_characters.end())
    //    {
    //        m_lineIt = m_characters.begin();
    //    }
    //    m_charIt = m_lineIt->begin();
    //}
    //OnBnClickedButtonShowChar();
}


void CScreenTranslateDlg::OnBnClickedButtonConfirm()
{
    // TODO: Add your control notification handler code here
    //::SendMessage(m_hwnd, WM_KEYDOWN, (WPARAM)'A', 0x001e0001);
    //::SendMessage(m_hwnd, WM_KEYUP, (WPARAM)'A', 0x001e0001);
    //return;
    NSleep(1000);
    wchar_t cord[100];
    m_cord.GetWindowText(cord, 99);

    POINT pt;

    swscanf_s(cord, L"%d,%d", &pt.x, &pt.y);
    MoveMouse(m_hwnd, pt);
    KeyEvent(VK_CONTROL, false);
    LeftClick();
    KeyEvent(VK_CONTROL, false, false);
    return;
    
    NSleep(1000);

    KeyPress(VK_ESCAPE);
    NSleep(100);
    KeyPress(VK_ESCAPE);
    NSleep(100);
    KeyPress(VK_ESCAPE);
    NSleep(100);
    KeyPress(VK_UP, true);
    NSleep(100);
    KeyPress(VK_UP, true);
    NSleep(100);
    KeyPress(VK_RETURN);

    NSleep(4000);
    KeyPress(VK_DOWN, true);
    NSleep(100);
    KeyPress(VK_DOWN, true);
    NSleep(100);
    KeyPress(VK_RETURN);
    NSleep(100);
    KeyPress(VK_RETURN);

   /* wchar_t str[100] = L"";
    m_char.GetWindowText(str, 100);
    if (str[0] != 0 && str[0] != L' ' && !m_charIt->empty())
    {
        SaveCharacter(*m_charIt, str[0]);
    }*/
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


void CScreenTranslateDlg::NSleep(DWORD t)
{
    auto start = GetTickCount();
    auto end = start + t;
    while (true)
    {

        auto ret = MsgWaitForMultipleObjects(0, nullptr, true, t, QS_ALLEVENTS);

        if (ret == WAIT_OBJECT_0)
        {
            MSG msg;
            while (::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        auto current = GetTickCount();
        if (end <= current)
            break;
        t = end - current;
    }
}






void CScreenTranslateDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void ReadStatics(Property& prop, const std::string& filename)
{
    std::wifstream ifs(filename);
    do
    {
        wchar_t buf[80] = { 0 };
        ifs.getline(buf, 80, L':');
        if (!ifs.good())
            break;
        int value = 0;
        ifs >> value;
        wstring key = buf[0] == L'\n' ? buf + 1 : buf;
        auto it = prop.m_properties.find(key);
        if (it != prop.m_properties.end())
            it->second.second = value;
        else
        {
            assert(false);
        }
    } while (ifs.good());
    ifs.close();
}
void DumpStatics(Property& prop, const std::string& filename)
{
    std::wofstream ofs(filename);
    TRACE(L"================== BEST PROPERTY ==================\r\n");
    for (auto it : prop.m_properties)
    {
        ofs << it.first << L":\t" << it.second.second << std::endl;
        TRACE(L"BEST PROPERTY: %s: %d\r\n", it.first.c_str(), it.second.second);
    }
    TRACE(L"================== BEST PROPERTY ==================\r\n");
    ofs.close();
}

void Statics(char* name, Property& prop)
{
    static Property sp;
    static bool initialized = false;
    std::string file("Statics_");
    file.append(name);
    file.append(".txt");

    if (!initialized)
    {
        ReadStatics(sp, file);
        initialized = true;
    }

    auto it = sp.m_properties.begin();
    auto cit = prop.m_properties.begin();
    bool updated = false;
    for (; cit != prop.m_properties.end(); ++cit, ++it)
    {
        if (it->second < cit->second)
        {
            it->second = cit->second;
            updated = true;
        }
    }

    if (updated)
    {
        DumpStatics(sp, file);
    }
}

bool IsAddingMaximumMagic(Property& prop)
{
    static int bestMagic = 60;
    auto magic = prop.m_properties[L"Magic"].second;
    if (magic > bestMagic)
    {
        bestMagic = magic;
        return true;
    }
    return false;
}

bool IsGoodSpellWeaverAmulet(Property& prop)
{
    static int bestScore = 20;
    int score = 0;
    if (prop.m_properties[L"MeteorCluster"].second < 3) return false;

    auto v1 = prop.m_properties[L"AllAttributes"].second;
    if (v1 > 15) score += v1 - 15;
    auto v2 = prop.m_properties[L"Spell"].second;
    score += v2 * 20;
    auto v3 = prop.m_properties[L"Magic"].second;
    if (v3 > 50) score += v3 - 50;
    auto v4 = prop.m_properties[L"Mana"].second;
    if (v4 > 60) score += v4 - 60;
    auto v5 = prop.m_properties[L"ResistAll"].second;
    if (v5 > 20) score += v5 - 20;

    bool isGood = false;
    if (score > bestScore * 9 / 10)
    {
        isGood = true;
    }

    if (score > bestScore)
    {
        bestScore = score;
    }

    return isGood;
    
}
bool IsGoodSpellWeaverRing(Property& prop)
{
    static int bestScore = 10;
    static int bestMagicForGolem = 50;
    int score = 0;
    if (prop.m_properties[L"Golem"].second == 3)
    {
        auto magic = prop.m_properties[L"Magic"].second;
        if (magic > bestMagicForGolem)
        {
            bestMagicForGolem = magic;
            return true;
        }
        return false;
    }

    if (prop.m_properties[L"MeteorCluster"].second < 3)
        return false;

    auto v1 = prop.m_properties[L"AllAttributes"].second;
    if (v1 > 15) score += (v1 - 15) * 2;
    auto v3 = prop.m_properties[L"Magic"].second;
    if (v3 > 50) score += v3 - 50;
    auto v4 = prop.m_properties[L"Mana"].second;
    if (v4 > 80) score += v4 - 80;
    auto v5 = prop.m_properties[L"ResistAll"].second;
    if (v5 > 30) score += (v5 - 30) * 2;

    bool isGood = false;
    if (score > bestScore * 9 / 10)
    {
        isGood = true;
    }

    if (score > bestScore)
    {
        bestScore = score;
    }

    return isGood;

}
bool IsGoodFanaticRing(Property& prop)
{
    static int bestScore = 10;
    int score = 0;

    if (prop.m_properties[L"MaxResistAll"].second < 3)
        return false;

    auto v1 = prop.m_properties[L"AllAttributes"].second;
    if (v1 > 15) score += (v1 - 15) * 4;
    auto v3 = prop.m_properties[L"Dexterity"].second;
    if (v3 > 40) score += (v3 - 40)*2;
    auto v4 = prop.m_properties[L"Strength"].second;
    if (v4 > 40) score += (v4 - 40)*2;
    auto v5 = prop.m_properties[L"ResistAll"].second;
    if (v5 > 20) score += (v5 - 20) * 2;
    auto v6 = prop.m_properties[L"StealMana"].second;
    if (v6 > 4) score += (v6 - 4) * 30;
    auto v7 = prop.m_properties[L"CriticalHitDamage"].second;
    score += v7;


    bool isGood = false;
    if (score > bestScore * 4 / 5)
    {
        isGood = true;
    }

    if (score > bestScore)
    {
        bestScore = score;
    }

    return isGood;
}


bool IsGoodFanaticBelt(Property& prop)
{
    static int bestScore = 10;
    int score = 0;

    if (prop.m_properties[L"MaxResistAll"].second < 3)
        return false;

    auto v1 = prop.m_properties[L"AllAttributes"].second;
    if (v1 > 5) score += (v1 - 5) * 40;
    auto v3 = prop.m_properties[L"Dexterity"].second;
    if (v3 > 40) score += (v3 - 40) * 2;
    auto v4 = prop.m_properties[L"Strength"].second;
    if (v4 > 30) score += (v4 - 30) * 2;
    auto v5 = prop.m_properties[L"ResistAll"].second;
    if (v5 > 20) score += (v5 - 20) * 2;
    auto v6 = prop.m_properties[L"Mana"].second;
    if (v6 > 80) score += (v6 - 80);
    auto v7 = prop.m_properties[L"HitPoints"].second;
    if (v7 > 80) score += v7 - 80;


    bool isGood = false;
    if (score > bestScore * 4 / 5)
    {
        isGood = true;
    }

    if (score > bestScore)
    {
        bestScore = score;
    }

    return isGood;
}

bool IsGoodHarBingerBow(Property& prop)
{
    static int bestScore = 100;
    int score = 0;
    auto v2 = prop.m_properties[L"EnhancedDamage"].second;
    if (v2 > 200)
    {
        int k = 0;
    }

    auto v1 = prop.m_properties[L"IAS"].second;
    if (v1 < 30) return false;
    

    if (v2 > 150) score += v2 - 150;
    auto v4 = prop.m_properties[L"AddDamage"].second;
    score += v4 * 2;
    auto v5 = prop.m_properties[L"AddElementDamage"].second;
    if (v5 > 80) score += (v5 - 80) / 2;

    auto v6 = prop.m_properties[L"CriticalHitDamage"].second;
    score += v6*2;

    bool isGood = false;
    if (score > bestScore * 4 / 5)
    {
        isGood = true;
    }

    if (score > bestScore)
    {
        bestScore = score;
    }

    return isGood;
}
bool IsGoodItem(Property& prop)
{
    Statics("HarBinger_Bow", prop);
    //return IsAddingMaximumMagic(prop);
    //return IsGoodSpellWeaverAmulet(prop);
    //return IsGoodSpellWeaverRing(prop);
    //return IsGoodFanaticRing(prop);
    //return IsGoodFanaticBelt(prop);
    return IsGoodHarBingerBow(prop);
}

void CScreenTranslateDlg::OnBnClickedButtonStart()
{
    if (m_state == State::Running)
    {
        m_state = State::Stopping;
        m_startButton.SetWindowText(L"Stopping...");
        return;
    }
    else if (m_state == State::Stopping)
    {
        // do nothing.
        return;
    }
    else if (m_state == State::Stopped)
    {
        m_state = State::Running;
        m_startButton.SetWindowText(L"Running...");
    }

    // TODO: Add your control notification handler code here
    POINT ptStep1 = { 350, 40 };
    POINT ptAnvil = { 125, 195 };
    POINT ptCraftButton = { 120, 420 };
    POINT ptCraftItem = { 140, 200 };
    POINT ptHoverOnItem = { 110, 200 };
    POINT ptFirstSlot = { 470, 325 };
    POINT ptSharedBox = { 400, 58 };
    POINT ptMenu = { 150, 600 };
    POINT ptNewGame = { 320, 340 };
    POINT ptCharacter = { 420, 410 };
    POINT ptNormal = { 420, 380 };

    int bestMagic = 0;// 68;

    do
    {
        // restart
        ::BringWindowToTop(m_hwnd);
        NSleep(100);
        MoveMouse(m_hwnd, ptMenu);
        LeftClick();
        NSleep(100);
        MoveMouse(m_hwnd, ptNewGame);
        LeftClick();
        NSleep(2000);
        
        MoveMouse(m_hwnd, ptCharacter);
        LeftClick();
        LeftClick();// click twice as the character is not pre-selected.
        NSleep(100);
        MoveMouse(m_hwnd, ptNormal);
        LeftClick();

        NSleep(2000);
        // restarted.

        m_restartRequired = false;

        MoveMouse(m_hwnd, ptStep1);
        LeftClick();
        NSleep(3000);
        MoveMouse(m_hwnd, ptAnvil);
        LeftClick();
        NSleep(2000);
        MoveMouse(m_hwnd, ptCraftButton);
        LeftClick();
        NSleep(100);
        CaptureAnImage(m_hwnd, m_lastmatrix);   // baseline

        do
        {
            MoveMouse(m_hwnd, ptCraftItem);
            LeftClick();
            NSleep(10);
            MoveMouse(m_hwnd, ptHoverOnItem);
            NSleep(100);
            CaptureAnImage(m_hwnd, m_currentmatrix);   // current

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

                    KillTimer(0);
                    SetTimer(0, 100000, nullptr);

                    if (IsGoodItem(prop))
                    {
                        KeyEvent(VK_CONTROL, false);
                        MoveMouse(m_hwnd, ptHoverOnItem);
                        LeftClick();
                        KeyEvent(VK_CONTROL, false, false);
                        NSleep(100);
                        MoveMouse(m_hwnd, ptSharedBox);
                        LeftClick();
                        NSleep(2000);
                        MoveMouse(m_hwnd, ptFirstSlot);
                        KeyEvent(VK_CONTROL, false);
                        LeftClick();
                        KeyEvent(VK_CONTROL, false, false);
                        break; // restart
                    }
                }
                else
                {
                    break; // restart
                }
            }
        } while (!m_restartRequired && m_state == State::Running);

    } while (m_state == State::Running);

    assert(m_state == State::Stopping);

    if (m_state == State::Stopping)
    {
        m_state = State::Stopped;
        m_startButton.SetWindowText(L"Stopped.");
    }
}


void CScreenTranslateDlg::OnBnClickedButtonStart2()
{
    
    // TODO: Add your control notification handler code here
    CaptureAnImage(m_hwnd, m_lastmatrix);   // baseline
    POINT ptHoverOnItem = { 110, 200 };
    MoveMouse(m_hwnd, ptHoverOnItem);
    NSleep(100);
    CaptureAnImage(m_hwnd, m_currentmatrix);   // current

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


void CScreenTranslateDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
    OnBnClickedButtonStart();
}
