#include<windows.h>
#include"LinkList.h"
#include "ui.h"
#include"hook.h"

// 画图时使用的表示图形的直径像素点个数。
#define CELL_PIXEL			1

// 用来绘图的颜色
#define COLOR_Gold		RGB(255, 215, 0)
#define COLOR_Hook		RGB(0, 0, 0)
#define COLOR_Background RGB(205, 129, 98)
#define COLOR_BOUNDARY  RGB(205, 129, 98)
#define COLOR_TEXT			RGB(255,216,230)

//游戏参数设置
#define INIT_TIMER_ELAPSE1 50
//#define INIT_TIMER_ELAPSE2 50
#define INIT_TIMER_ELAPSE3 1000
#define MAX_X		750	// 游戏界面大小
#define MAX_Y		600	// 游戏界面大小
#define HEIGHT		500
#define WIDTH		500

// 全局变量的定义
HINSTANCE hinst; /// HINSTANCE是用来表示程序运行实例的【句柄】handle，某些API函数会使用到这个变量。instance 实例
RECT rectBoundary;//rectangle 一种结构体

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入【一般不管】
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄【一般不管】
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。隐藏与否
{
	WNDCLASS wc;//结构体
				// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	//给wc的成员赋值
	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	/*【重点】*/wc.lpfnWndProc = MainWndProc;//lp开头的表示指针，lpfn表示函数指针
										 // no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);//【图标】
						 // Cursor是鼠标光标，这里是设定了鼠标光标的样式。
						 // 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);//【光标】
				   // GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
				   // 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = "MainMenu";
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。

	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// 窗口类名，必须是已经注册了的窗口类
		"Gold Miner!!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);

	//返回入口函数，窗口退出
	// 以下进入消息循环。获取消息--翻译消息--分配消息（由窗口的消息处理函数来处理消息）
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);//内部调用MainWndProc函数指针  回调机制
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

	GetClientRect(hwnd, &rect);//获得客户区的边界

	hdc = GetDC(hwnd);

	hdcmem = CreateCompatibleDC(hdc);//创建一个新的dc以以前的参数建立相同的缓冲区
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	// 创建需要用到的PEN和BRUSH
	hBrushGold = CreateSolidBrush(COLOR_Gold); // RGB颜色，solid实心BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN， PS_NULL表示不可见，粗细为零
	hpenline= CreatePen(0, 1, COLOR_Hook);//钩子和定点间的连线
	hBrushHook = CreateSolidBrush(COLOR_Hook);
	hBackground= CreateSolidBrush(COLOR_Background);
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);//0代表实心，3表示粗细，3个像素

	//HDC hdcBitmapSrc;
	//HBITMAP hBitmap;
	//HBITMAP hbmpMiner;
	//BITMAP bmp;

	//hbmpMiner = LoadImage(NULL, "背景.bmp",
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//hBitmap = CreateCompatibleBitmap(hdc, // 不能是hdcMem，否则会变成黑白2色位图
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


	//画背景
	FillRect(hdcmem, &rect, (HBRUSH)hBackground);//填充一个矩形，全覆盖背景色

	//画钩子
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushHook);//选择刷
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);//选择笔
	lpHook = GetHook();//得到钩子坐标

	Rectangle(hdcmem,
		lpHook->x * CELL_PIXEL /*+ rectBoundary.left*/,
		lpHook->y * CELL_PIXEL /*+ rectBoundary.top*/,
		(lpHook->x + 1)*CELL_PIXEL /*+ rectBoundary.left*/,
		(lpHook->y + 1)*CELL_PIXEL) /*+ rectBoundary.top)*/;
	
	//画钩子和定点间的连线
	SelectObject(hdcmem, hpenline);
	MoveToEx(hdcmem, point.x*CELL_PIXEL, point.y*CELL_PIXEL, NULL);
	LineTo(hdcmem, lpHook->x*CELL_PIXEL, lpHook->y*CELL_PIXEL);

	SelectObject(hdcmem, hpen);
	//画金块
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

	//hbmpMiner = LoadImage(NULL, "背景.bmp",
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//
	//hBitmap = CreateCompatibleBitmap(hdc, // 不能是hdcMem，否则会变成黑白2色位图
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


	//画边框
	SelectObject(hdcmem, hPenBoundary);

	// 将PEN移动到需要绘制的方框的左上角
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// 画了一个方框。演示LineTo函数
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	//写一行字
	// 创建了一个字体对象
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "Sorce %d  Second %d   ", GetScore(),GetSecond());
		// 设置输出颜色
		SetTextColor(hdcmem, COLOR_TEXT);
		// 输出字符串。
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	// 在内存DC中画完，一次输出的窗口DC上。
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);
	//从内存拷贝到窗口dc上
	
	// 回收资源
    // DeleteObject 释放GDI对象
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hBrushHook);
	DeleteObject(hBrushGold);
	DeleteObject(hpen);
	DeleteObject(hBackground);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	DeleteObject(hpenline);
	//释放占用的DC等系统资源。
	ReleaseDC(hwnd, hdc);
}

//根据游戏界面的坐标系大小设置游戏窗口大小

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// 左上角
	POINT ptRightBottom;	// 右下角
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// 设置游戏边界
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// 计算上下左右角的位置
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// 计算好了，设置窗口大小。
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // 保存边界和左右两边边框相等。
		rectBoundary.bottom + 120, //给积分信息留出显示空间。
		TRUE);
}

LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，且只创建一次
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE:
		CreateGame(hwnd,
			INIT_TIMER_ELAPSE1,
			//INIT_TIMER_ELAPSE2,
            INIT_TIMER_ELAPSE3,
			MAX_X,
			MAX_Y//边界
		);
		ReSizeGameWnd(hwnd);
		break;

		// 当系统认为窗口上的GDI对象应该被重绘时，会向窗口发送一个WM_PAINT消息。
		// 当然应用程序也可以通过调用 UpateWindow来主动向窗口发送一个WM_PAINT消息。
		// 所有使用GDI在窗口上绘制图形的程序都 “必须” 写在这里。
		// 如果不是在WM_PAINT消息的处理过程中绘制GDI图形，那么在窗口刷新时就会被新被抹除和覆盖
	case WM_PAINT:
		GamePaint(hwnd);
		break;

	case WM_KEYDOWN://键盘处理

		OnKeyDown(wParam,hwnd);
		GamePaint(hwnd);
		break;

	case WM_LBUTTONDOWN:
		//OnTimer(hwnd);
		GamePaint(hwnd);
		break;

	case WM_TIMER://计时器
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

	case WM_DESTROY://窗口关闭，结束进程
		ExitProcess(0);
		break;

	default:
		break;
	}

	

	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);//defalt处理
}
