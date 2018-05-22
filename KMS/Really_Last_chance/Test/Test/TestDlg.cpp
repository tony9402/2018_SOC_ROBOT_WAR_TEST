
// TestDlg.cpp: 구현 파일
//

#pragma region DONT_CHANGE
#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include"Color.h"
#include<memory>
#include<stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BITMAPINFO BmInfo;
LPBYTE pImgBuffer;	

CTestDlg::CTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_stDisplay);
	DDX_Control(pDX, IDC_STATIC2, m_stDisplay2);
	//DDX_Control(pDX, IDC_STATIC2, m_a);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CTestDlg 메시지 처리기


BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	

	this->SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOSIZE);

	m_Cap = capCreateCaptureWindow(TEXT("Image Test"), WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, m_stDisplay, NULL);
	if (capDriverConnect(m_Cap, NULL) == FALSE)return FALSE;
	if (capSetCallbackOnFrame(m_Cap, CallbackOnFrame) == FALSE)return FALSE;
	
	capPreviewRate(m_Cap, 33);
	capOverlay(m_Cap, false);
	capPreview(m_Cap, true);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	capDriverDisconnect(m_Cap);

	if (pImgBuffer != NULL) {
		delete[] pImgBuffer;
	}
}
#pragma endregion

int labeling(BYTE (*input)[width]);
void Close();

BYTE Yellow[height][width] = { 0 };
BYTE Red[height][width] = { 0 };
BYTE Blue[height][width] = { 0 };

int Area_red = 0, Area_yellow = 0, Area_blue = 0;

LRESULT CALLBACK CallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	short i, j;
	__color::Input(lpVHdr);
	memset(Blue, 0, sizeof(BYTE) * height * width);
	memset(Red, 0, sizeof(BYTE) * height * width);
	memset(Yellow, 0, sizeof(BYTE) * height * width);
	
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (!IsPocarIColor(i, j) && !IsCantataColor(i,j) && !IsPocaColor(i,j))
			{
				mRGB[i][j][RED] = mRGB[i][j][GREEN] = mRGB[i][j][BLUE] = 0;
			}
			else
			{
				if (IsPocarIColor(i, j))
				{
					Blue[i][j] = 255;
					mRGB[i][j][RED] = mRGB[i][j][GREEN] = mRGB[i][j][BLUE] = 0;
				}
				else {
					Blue[i][j] = 0;
				}
				if (IsPocaColor(i, j))
				{
					Yellow[i][j] = 255;
					mRGB[i][j][RED] = mRGB[i][j][GREEN] = mRGB[i][j][BLUE] = 0;
				}
				else
				{
					Yellow[i][j] = 0;
				}
				if (IsCantataColor(i, j))
				{
					Red[i][j] = 255;
				}
				else
				{
					Red[i][j] = 0;
				}
			}
		}
	}

	Close();

	Area_blue = labeling(Blue);
	Area_red = labeling(Red);
	Area_yellow = labeling(Yellow);

	printf("Red : %5d Yellow : %5d Blue : %5d\n", Area_red, Area_yellow, Area_blue);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (Red[i][j] == 255 || Yellow[i][j] == 255 || Blue[i][j] == 255)
			{
				mRGB[i][j][RED] = mRGB[i][j][GREEN] = mRGB[i][j][BLUE] = 255;
			}
			else
			{
				mRGB[i][j][RED] = mRGB[i][j][GREEN] = mRGB[i][j][BLUE] = 0;
			}
		}
	}

	///*
	if (Area_red >= 2000 && Area_yellow >= 1000 && Area_blue >= 200)
	{
		printf("포카칩 발견\n");
	}
	else if (Area_blue >= 2800 && !(Area_yellow >= 1000) && !(Area_red >= 1000))
	{
		printf("포카리 발견\n");
	}
	else if (Area_red >= 900 && !(Area_blue >= 1400) && !(Area_yellow >= 1400))
	{
		printf("칸타타 발견\n");
	}
	else
	{
		printf("Seraching\n");
	}
	//*/
	


	__color::Output(lpVHdr);

	return (LRESULT)true;
}

#define min(x,y) (x) > (y) ? (y) : (x)
#define max(x,y) (x) > (y) ? (x) : (y)

BYTE sRed[height][width] = { 0 };
BYTE sYellow[height][width] = { 0 };
BYTE sBlue[height][width] = { 0 };

void Close()
{
	short i, j;

	for (i = 0; i < height; i++)
	{
		memcpy(sRed[i], Red[i], sizeof(BYTE) * width);
		memcpy(sYellow[i], Yellow[i], sizeof(BYTE) * width);
		memcpy(sBlue[i], Blue[i], sizeof(BYTE) * width);
	}

	for (int CNT = 0; CNT < 3; CNT++) {
		for (i = 1; i < height - 1; i++)
		{
			for (j = 1; j < width - 1; j++)
			{
				if (sRed[i][j] == 0) {
					for (int m = -1; m <= 1; m++)
					{
						for (int n = -1; n <= 1; n++)
						{
							if (sRed[i + m][j + n] != 0)
							{
								Red[i][j] = 255;
								break;
							}
						}
					}
				}
				if (sYellow[i][j] == 0) {
					for (int m = -1; m <= 1; m++)
					{
						for (int n = -1; n <= 1; n++)
						{
							if (sYellow[i + m][j + n] != 0)
							{
								Yellow[i][j] = 255;
								break;
							}
						}
					}
				}
				if (sBlue[i][j] == 0) {
					for (int m = -1; m <= 1; m++)
					{
						for (int n = -1; n <= 1; n++)
						{
							if (sBlue[i + m][j + n] != 0)
							{
								Blue[i][j] = 255;
								break;
							}
						}
					}
				}
			}
		}
		for (i = 0; i < height; i++) {
			memcpy(sRed[i], Red[i], sizeof(BYTE) * width);
			memcpy(sYellow[i], Yellow[i], sizeof(BYTE) * width);
			memcpy(sBlue[i], Blue[i], sizeof(BYTE) * width);
		}
	}

	for (i = 1; i < height - 1; i++)
	{
		for (j = 1; j < width - 1; j++)
		{
			if (sRed[i][j] == 255) {
				for (int m = -1; m <= 1; m++)
				{
					for (int n = -1; n <= 1; n++)
					{
						if (sRed[i + m][j + n] == 0)
						{
							Red[i][j] = 0;
							break;
						}
					}
				}
			}
			if (sYellow[i][j] == 255) {
				for (int m = -1; m <= 1; m++)
				{
					for (int n = -1; n <= 1; n++)
					{
						if (sYellow[i + m][j + n] == 0)
						{
							Yellow[i][j] = 0;
							break;
						}
					}
				}
			}
			if (sBlue[i][j] == 255) {
				for (int m = -1; m <= 1; m++)
				{
					for (int n = -1; n <= 1; n++)
					{
						if (sBlue[i + m][j + n] == 0)
						{
							Blue[i][j] = 0;
							break;
						}
					}
				}
			}
		}
	}

	return;
}

int labeling(BYTE(*input)[width])
{
	short i, j;
	int **index = (int**)malloc(sizeof(int*) * height);
	int *cnt_Val = (int*)malloc(sizeof(int) * 700);
	int *area = (int*)malloc(sizeof(int) * 700);

	for (i = 0; i < height; i++)
	{
		index[i] = (int*)malloc(sizeof(int) * width);
		memset(index[i], 0, sizeof(int) * width);
	}
	memset(cnt_Val, 0, sizeof(int) * 700);
	memset(area, 0, sizeof(int) * 700);

	int cnt = 0, last_cnt = 0;

	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
			if (input[i][j] == 255)
			{
				if (index[i - 1][j] == 0 && index[i][j - 1] == 0)
				{
					cnt++;
					index[i][j] = cnt;
					cnt_Val[cnt] = cnt;
				}
				else if (index[i - 1][j] != 0 && index[i][j - 1] == 0)
				{
					index[i][j] = index[i - 1][j];
				}
				else if (index[i - 1][j] == 0 && index[i][j - 1] != 0)
				{
					index[i][j] = index[i][j - 1];
				}
				else if (index[i - 1][j] == index[i][j - 1] && index[i][j - 1] != 0)
				{
					index[i][j] = index[i][j - 1];
				}
				else if (index[i - 1][j] != index[i][j - 1] && index[i - 1][j] != 0 && index[i][j - 1] != 0)
				{
					index[i][j] = min(index[i][j - 1], index[i - 1][j]);
					cnt_Val[max(index[i][j - 1], index[i - 1][j])] = min(cnt_Val[max(index[i][j - 1], index[i - 1][j])], min(index[i][j - 1], index[i - 1][j]));
				}
				else
				{
					index[i][j] = 0;
				}
				last_cnt = cnt;
			}
		}
	}
	if (last_cnt == 0)return 0;
	int Forcnt;

	for (Forcnt = last_cnt; Forcnt >= 1; Forcnt--)
	{
		if (cnt_Val[Forcnt] != Forcnt)
		{
			for (i = 1; i < height; i++)
			{
				for (j = 1; j < width; j++)
				{
					if (index[i][j] == Forcnt)
					{
						index[i][j] = cnt_Val[Forcnt];
					}
				}
			}
		}
	}

	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
			if (index[i][j] != 0)
			{
				area[index[i][j]]++;
			}
		}
	}

	int area_Max = 0, area_cnt = 0;

	for (i = 1; i < last_cnt; i++)
	{
		if (area[i] > area_Max)
		{
			area_Max = area[i];
			area_cnt = i;
		}
	}

	if (area_cnt == 0)
	{
		memset(input, 0, sizeof(BYTE)*height * width);
	}
	else
	{
		for (i = 1; i < height; i++)
		{
			for (j = 1; j < width; j++)
			{
				if (index[i][j] != area_cnt)
				{
					input[i][j] = 0;
				}
				else
				{
					input[i][j] = 255;
				}
			}
		}
	}

	for (i = 0; i < height; i++)
	{
		free(index[i]);
	}
	free(index);
	free(cnt_Val);
	free(area);

	return area_Max;
}