
// ScreenTranslateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenTranslate.h"
#include "ScreenTranslateDlg.h"
#include "afxdialogex.h"
#include <gdiplus.h>
#include <algorithm>
#include <list>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
}

void CScreenTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PROGRAMLIST, m_programs);
}

BEGIN_MESSAGE_MAP(CScreenTranslateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CScreenTranslateDlg::OnBnClickedButtonRefresh)
    ON_BN_CLICKED(IDC_BUTTON2, &CScreenTranslateDlg::OnBnClickedButton2)
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

std::vector<unsigned char> ToPixels(HBITMAP BitmapHandle, int &width, int &height)
{
    BITMAP Bmp = { 0 };
    BITMAPINFO Info = { 0 };
    std::vector<unsigned char> Pixels = std::vector<unsigned char>();

    HDC DC = CreateCompatibleDC(NULL);
    std::memset(&Info, 0, sizeof(BITMAPINFO)); //not necessary really..
    HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, BitmapHandle);
    GetObject(BitmapHandle, sizeof(Bmp), &Bmp);

    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = width = Bmp.bmWidth;
    Info.bmiHeader.biHeight = height = Bmp.bmHeight;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = ((width * Bmp.bmBitsPixel + 31) / 32) * 4 * height;

    Pixels.resize(Info.bmiHeader.biSizeImage);
    GetDIBits(DC, BitmapHandle, 0, height, &Pixels[0], &Info, DIB_RGB_COLORS);
    SelectObject(DC, OldBitmap);

    height = std::abs(height);
    DeleteDC(DC);
    return Pixels;
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
            union MyUnion
            {
                char buf[4];
                DWORD dw;
            };
            MyUnion my;
            my.buf[3] = buf[3];
            my.buf[2] = buf[0];
            my.buf[1] = buf[1];
            my.buf[0] = buf[2];
            matrix[h][w] =  reinterpret_cast<DWORD*>(lpbitmap)[h * bmpScreen.bmWidth + w];
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



void CScreenTranslateDlg::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    //SetTimer(0, 5000, nullptr);
    int curSel = m_programs.GetCurSel();
    if (curSel != -1)
    {
        HWND hwnd = reinterpret_cast<HWND>(m_programs.GetItemData(curSel));
        std::vector<std::vector<COLORREF>> matrix;
        CaptureAnImage(hwnd, matrix);

        if (m_matrix.size() != 0 && matrix.size() == m_matrix.size() && m_matrix[0].size() == matrix[0].size())
        {
            size_t Width = m_matrix[0].size();;
            size_t Height = m_matrix.size();
            std::vector<std::vector<COLORREF>> diff(Height, std::vector<COLORREF>(Width));
            int delta = 0;
            
            for (int x = 0; x < Width; x++)
            {
                for (int y = 0; y < Height; y++)
                {
                    auto c1 = m_matrix[y][x];
                    auto c2 = matrix[y][x];
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
            if (delta > 100 * 100)
            {

                std::vector<int> rows(Height, 0);
                std::vector<int> cols(Width, 0);
                for (int x = 0; x < Width; x++)
                {
                    for (int y = 0; y < Height; y++)
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


                for (int x = 0; x < Width; x++)
                {
                    for (int y = 0; y < Height; y++)
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

                for (int x = 0; x < Width; x++)
                {
                    for (int y = 0; y < Height; y++)
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
                
                if (firstRow != lastRow && firstCol != lastCol)
                {
                    int left = std::distance(cols.begin(), firstCol);
                    int right = std::distance(cols.begin(), lastCol);
                    int top = std::distance(rows.begin(), firstRow);
                    int bottom = std::distance(rows.begin(), lastRow);
                    auto pDC = GetDC();
                    pDC->SetStretchBltMode(HALFTONE);

                    for (int x = left; x < right; x++)
                    {
                        for (int y = top; y < bottom; y++)
                        {
                            auto& c = matrix[y][x];
                            if (c != 0)
                            {
                                c &= 0x00ffffff;
                                auto r = (c & 0xff0000) >> 16;
                                auto g = (c & 0xff00) >> 8;
                                auto b = c & 0xff;

                                if (r + g + b > 200)
                                    pDC->SetPixel(x + 0, bottom - y + 100, 0);
                                else
                                    c = 0;
                            }
                        }
                    }
                    ReleaseDC(pDC);

                    
                    // split characters
                    vector<vector<int>> visited(bottom - top, vector<int>(right - left, 0));
                    for (int x = left; x < right; x++)
                    {
                        for (int y = top; y < bottom; y++)
                        {
                            auto& c = matrix[y][x];
                            visited[y - top][x - left] = 1;
                            if (c != 0)
                            {
                                std::vector<std::pair<int, int>> character;
                                std::vector<pair<int, int>> stack;
                                stack.push_back.push_back(make_pair(x, y));
                                while (!stack.empty())
                                {
                                    auto point = *stack.rbegin();
                                    stack.pop_back();
                                    for (int i = -1; i <= 1; i++)
                                    {
                                        for (int j = -1; j <= 1; j++)
                                        {
                                            if (i == 0 && j == 0) continue;

                                        }
                                    }
                                }
                                
                            }
                        }
                    }
                }
            }
        }

        m_matrix.swap(matrix);
    }
}


void CScreenTranslateDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnTimer(nIDEvent);
    int curSel = m_programs.GetCurSel();
    if (curSel != -1)
    {
        HWND hwnd = reinterpret_cast<HWND>(m_programs.GetItemData(curSel));
       // CaptureAnImage(hwnd);
        
        auto hDC = ::GetDC(hwnd);
        const size_t Width = 100;
        const size_t Height = 100;
        std::vector<std::vector<COLORREF>> matrix(Width, std::vector<COLORREF>(Height));

        for (int x = 0; x < Width; x++)
        {
            for (int y = 0; y < Height; y++)
            {
                auto c = ::GetPixel(hDC, x, y);
                matrix[x][y] = c;
            }
        }
        
        ::ReleaseDC(hwnd, hDC);
        std::vector<std::vector<COLORREF>> diff(Width, std::vector<COLORREF>(Height));
        if (m_matrix.size() == matrix.size())
        {
            int delta = 0;
            for (int x = 0; x < Width; x++)
            {
                for (int y = 0; y < Width; y++)
                {
                    if (m_matrix[x][y] != matrix[x][y])
                    {
                        delta++;
                        diff[x][y] = matrix[x][y];
                    }
                    else
                    {
                        diff[x][y] = 0x000000;
                    }
                }
            }
            if (delta * 200 > Width * Height)
            {
                auto pDC = GetDC();
                pDC->SetStretchBltMode(HALFTONE);
                //SetStretchBltMode(hdcWindow, HALFTONE);
                const int left = 100;
                const int top = 100;
                for (int x = 0; x < Width; x++)
                {
                    for (int y = 0; y < Width; y++)
                    {
                        pDC->SetPixel(x + left, y + top, diff[x][y]);
                    }
                }
                ReleaseDC(pDC);
            }            
        }

        m_matrix.swap(matrix);
       
    }
}
