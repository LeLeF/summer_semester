#include<windows.h>
#include"LinkList.h"
#include "ui.h"
#include"hook.h"

// ��ͼʱʹ�õı�ʾͼ�ε�ֱ�����ص������
#define CELL_PIXEL			1

// ������ͼ����ɫ
#define COLOR_Gold		RGB(255, 215, 0)
#define COLOR_Hook		RGB(0, 0, 0)
#define COLOR_Background RGB(205, 129, 98)
#define COLOR_BOUNDARY  RGB(205, 129, 98)
#define COLOR_TEXT			RGB(255,216,230)

//��Ϸ��������
#define INIT_TIMER_ELAPSE1 50
//#define INIT_TIMER_ELAPSE2 50
#define INIT_TIMER_ELAPSE3 1000
#define MAX_X		750	// ��Ϸ�����С
#define MAX_Y		600	// ��Ϸ�����С
#define HEIGHT		500
#define WIDTH		500

// ȫ�ֱ����Ķ���
HINSTANCE hinst; /// HINSTANCE��������ʾ��������ʵ���ġ������handle��ĳЩAPI������ʹ�õ����������instance ʵ��
RECT rectBoundary;//rectangle һ�ֽṹ��

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó����롾һ�㲻�ܡ�
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�������һ�㲻�ܡ�
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ������������
{
	WNDCLASS wc;//�ṹ��
				// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	//��wc�ĳ�Ա��ֵ
	// ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
	/*���ص㡿*/wc.lpfnWndProc = MainWndProc;//lp��ͷ�ı�ʾָ�룬lpfn��ʾ����ָ��
										 // no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon��Ա����ָ�����ڵ�ͼ��
	// ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);//��ͼ�꡿
						 // Cursor������꣬�������趨����������ʽ��
						 // ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);//����꡿
				   // GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
				   // ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// ���ڵĲ˵�����Դ����
	wc.lpszMenuName = "MainMenu";
	// ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�

	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// �����������������Ѿ�ע���˵Ĵ�����
		"Gold Miner!!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	// ���ڴ����ɹ����������С�

	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
	UpdateWindow(hwnd);

	//������ں����������˳�
	// ���½�����Ϣѭ������ȡ��Ϣ--������Ϣ--������Ϣ���ɴ��ڵ���Ϣ��������������Ϣ��
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);//�ڲ�����MainWndProc����ָ��  �ص�����
	}
	return msg.wParam;
}

void GamePaint(HWND hwnd)
{
	HPEN hpen;
	HPEN hpenline;
	//HBRUSH hbrush;
	HDC hdc, hdcmem;
	HBITMAP hbmMem;

	HPEN hPenBoundary;
	HPEN hOldPen;

	HBRUSH hBrushGold;
	HBRUSH hBrushHook;
	HBRUSH hBackground;
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;

	PGAME_COORD pGold;
	PGold p;
	PGAME_COORD lpHook;
	int i;

	GetClientRect(hwnd, &rect);//��ÿͻ����ı߽�

	hdc = GetDC(hwnd);

	hdcmem = CreateCompatibleDC(hdc);//����һ���µ�dc����ǰ�Ĳ���������ͬ�Ļ�����
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	// ������Ҫ�õ���PEN��BRUSH
	hBrushGold = CreateSolidBrush(COLOR_Gold); // RGB��ɫ��solidʵ��BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN�� PS_NULL��ʾ���ɼ�����ϸΪ��
	hpenline= CreatePen(0, 1, COLOR_Hook);//���ӺͶ���������
	hBrushHook = CreateSolidBrush(COLOR_Hook);
	hBackground= CreateSolidBrush(COLOR_Background);
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);//0����ʵ�ģ�3��ʾ��ϸ��3������

	//HDC hdcBitmapSrc;
	//HBITMAP hBitmap;
	//HBITMAP hbmpMiner;
	//BITMAP bmp;

	//hbmpMiner = LoadImage(NULL, "����.bmp",
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//hBitmap = CreateCompatibleBitmap(hdc, // ������hdcMem��������ɺڰ�2ɫλͼ
	//	rect.right - rect.left, rect.bottom - rect.top);
	//SelectObject(hdcmem, hBitmap);
	//hdcBitmapSrc = CreateCompatibleDC(hdc);
	//SelectObject(hdcBitmapSrc, hbmpMiner);
	//GetObject(hbmpMiner, sizeof(BITMAP), &bmp);
	//StretchBlt(hdcmem,
	//	point.x, point.y,
	//	WIDTH, HEIGHT,
	//	hdcBitmapSrc,
	//	0, 0, bmp.bmWidth, bmp.bmHeight,
	//	SRCCOPY);


	//������
	FillRect(hdcmem, &rect, (HBRUSH)hBackground);//���һ�����Σ�ȫ���Ǳ���ɫ

	//������
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushHook);//ѡ��ˢ
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);//ѡ���
	lpHook = GetHook();//�õ���������

	Rectangle(hdcmem,
		lpHook->x * CELL_PIXEL /*+ rectBoundary.left*/,
		lpHook->y * CELL_PIXEL /*+ rectBoundary.top*/,
		(lpHook->x + 1)*CELL_PIXEL /*+ rectBoundary.left*/,
		(lpHook->y + 1)*CELL_PIXEL) /*+ rectBoundary.top)*/;
	
	//�����ӺͶ���������
	SelectObject(hdcmem, hpenline);
	MoveToEx(hdcmem, point.x*CELL_PIXEL, point.y*CELL_PIXEL, NULL);
	LineTo(hdcmem, lpHook->x*CELL_PIXEL, lpHook->y*CELL_PIXEL);

	SelectObject(hdcmem, hpen);
	//�����
	SelectObject(hdcmem, hBrushGold);
	for (i = 0; i < num; i++)
	{
		p = GetGold(i);
		pGold = GetGoldAt(i);
		if(pGold->x!=0||pGold->y!=0)
		Ellipse/*Rectangle*/(hdcmem,
			(p->size+pGold->x )* CELL_PIXEL + rectBoundary.left,
			(p->size +pGold->y )* CELL_PIXEL + rectBoundary.top,
			(-p->size +(pGold->x + 1))*CELL_PIXEL + rectBoundary.left,
			(-p->size +(pGold->y + 1))*CELL_PIXEL + rectBoundary.top);
	}
	
	//HDC hdcBitmapSrc;
	//HBITMAP hBitmap;
	//HBITMAP hbmpMiner;
	//BITMAP bmp;

	//hbmpMiner = LoadImage(NULL, "����.bmp",
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//
	//hBitmap = CreateCompatibleBitmap(hdc, // ������hdcMem��������ɺڰ�2ɫλͼ
	//	rect.right - rect.left, rect.bottom - rect.top);
	//SelectObject(hdcmem, hBitmap);
	//hdcBitmapSrc = CreateCompatibleDC(hdc);
	//SelectObject(hdcBitmapSrc, hbmpMiner);
	//GetObject(hbmpMiner, sizeof(BITMAP), &bmp);
	//StretchBlt(hdcmem,
	//	point.x, point.y,
	//	WIDTH, HEIGHT,
	//	hdcBitmapSrc,
	//	0, 0, bmp.bmWidth, bmp.bmHeight,
	//	SRCCOPY);


	//���߿�
	SelectObject(hdcmem, hPenBoundary);

	// ��PEN�ƶ�����Ҫ���Ƶķ�������Ͻ�
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// ����һ��������ʾLineTo����
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	//дһ����
	// ������һ���������
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "Sorce %d  Second %d   ", GetScore(),GetSecond());
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);
	//���ڴ濽��������dc��
	
	// ������Դ
    // DeleteObject �ͷ�GDI����
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hBrushHook);
	DeleteObject(hBrushGold);
	DeleteObject(hpen);
	DeleteObject(hBackground);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	DeleteObject(hpenline);
	//�ͷ�ռ�õ�DC��ϵͳ��Դ��
	ReleaseDC(hwnd, hdc);
}

//������Ϸ���������ϵ��С������Ϸ���ڴ�С

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// ���Ͻ�
	POINT ptRightBottom;	// ���½�
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// ������Ϸ�߽�
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// �����������ҽǵ�λ��
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// ������ˣ����ô��ڴ�С��
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // ����߽���������߱߿���ȡ�
		rectBoundary.bottom + 120, //��������Ϣ������ʾ�ռ䡣
		TRUE);
}

LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // ��Ϣ
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	// ע�⣬��switch-case, ÿ��������������ã�ֻ�����뵽һ��case�С�
	switch (msg)
	{
		// �����ڱ�����ʱ���յ��ĵ�һ����Ϣ����WM_CREATE����ֻ����һ��
		// һ���յ������Ϣ��������У�������������һЩ��ʼ���Ĺ���
	case WM_CREATE:
		CreateGame(hwnd,
			INIT_TIMER_ELAPSE1,
			//INIT_TIMER_ELAPSE2,
            INIT_TIMER_ELAPSE3,
			MAX_X,
			MAX_Y//�߽�
		);
		ReSizeGameWnd(hwnd);
		break;

		// ��ϵͳ��Ϊ�����ϵ�GDI����Ӧ�ñ��ػ�ʱ�����򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ��ȻӦ�ó���Ҳ����ͨ������ UpateWindow�������򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ����ʹ��GDI�ڴ����ϻ���ͼ�εĳ��� �����롱 д�����
		// ���������WM_PAINT��Ϣ�Ĵ�������л���GDIͼ�Σ���ô�ڴ���ˢ��ʱ�ͻᱻ�±�Ĩ���͸���
	case WM_PAINT:
		GamePaint(hwnd);
		break;

	case WM_KEYDOWN://���̴���

		OnKeyDown(wParam,hwnd);
		GamePaint(hwnd);
		break;

	case WM_LBUTTONDOWN:
		//OnTimer(hwnd);
		GamePaint(hwnd);
		break;

	case WM_TIMER://��ʱ��
	{
		switch (wParam)
		{
		case TIME_ID1 :
			OnRollTimer(hwnd);
			GamePaint(hwnd);
			break;

		/*case TIME_ID2 :
			OnMoveTimer(hwnd);
			GamePaint(hwnd);
			break;*/
		
		case TIME_ID3 :
			OnSecondTimer(hwnd);
			GamePaint(hwnd);
			break;

		}
	
	}
		break;

	case WM_DESTROY://���ڹرգ���������
		ExitProcess(0);
		break;

	default:
		break;
	}

	

	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);//defalt����
}
