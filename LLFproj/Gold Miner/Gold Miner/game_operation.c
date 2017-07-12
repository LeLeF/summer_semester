//---------头文件--------
#include<Windows.h>
#include"hook.h"

//---------宏定义---------
//计时器ID
#define TIME_ID1 10
#define TIME_ID2 20
#define TIME_ID3 30

//--------全局变量---------
//计时器的时间间隔
DWORD dwTimerElapse1;
DWORD dwTimerElapse2;
DWORD dwTimerElapse3;

////游戏状态
//int GameState = 0;

////计分器，初始为零
//int score = 0;

//倒计时秒数
int second = 120;

//---------函数定义--------
//返回分数
int GetScore() { return score; }

//返回时间
int GetSecond() { return second; }

//倒计时
void SecondDecline()
{
	second--;
}




void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse1,
	DWORD dwInitTimerElapse2,
	DWORD dwInitTimerElapse3,
	int boundary_x, 
	int boundary_y//边界
)
{ 
	dwTimerElapse1 = dwInitTimerElapse1;
	dwTimerElapse2 = dwInitTimerElapse2;
	dwTimerElapse3 = dwInitTimerElapse3;
	SetBoundary(boundary_x, boundary_y);

	//设置金块边界坐标
	SetGoldBoundary();

	//生成金块的数据结构
	CreatAllGolds();

	//生成钩子的数据结构
	CreatHook();

	//初始化转动量和方向
	hooktoleft = 1;
	angle = 0;

	//初始化游戏状态
	GameState = 0;
	score = 0;

	// 创建计时器
	// 每经过 dwTimerElapse 毫秒，hwnd窗口（主窗口）就会收到一个WM_TIMER消息。
	// 计时器是驱动本游戏进行的主要时间线。
	//SetTimer(hwnd, TIME_ID1, dwTimerElapse1, NULL);
	//SetTimer(hwnd, TIME_ID2, dwTimerElapse2, NULL);
	SetTimer(hwnd, TIME_ID3, dwTimerElapse3, NULL);
}

//游戏控制的主要流程
//当计时器发生时进行的处理逻辑
void OnTimer(DWORD hwnd,UINT_PTR timeid)
{
	switch(timeid)
	{	

	   //倒计时结束，弹出分数，停止计时器，结束进程
		case 0:
			KillTimer(hwnd, TIME_ID1);
			KillTimer(hwnd, TIME_ID2);
			KillTimer(hwnd, TIME_ID3);

			//弹出分数
			MessageBox(0, score, "Your Score", 0);
			ExitProcess(0);
			break;
		case TIME_ID1:
		{
			KillTimer(hwnd, TIME_ID2);
			SetTimer(hwnd, TIME_ID1, dwTimerElapse1, NULL);
			HookRoll();
			break; 
		}
		case TIME_ID2:
		{
			KillTimer(hwnd, TIME_ID1);
			SetTimer(hwnd, TIME_ID2, dwTimerElapse2, NULL);
			HookMove();
			CatchGold();
			CatchBoundary();
		
		}
			break;
		case TIME_ID3:
		SecondDecline();
		break;
	}

	return;
}

//当用于操作时进行的逻辑处理
//本游戏只使用了空格和键盘的下键控制
void OnKeyDown(DWORD vk,DWORD hwnd)
{
	switch (vk)//virtual key value
	{
		//开始游戏
	case VK_SPACE:
		GameState = 1;
		//SetTimer(hwnd, TIME_ID1, dwTimerElapse1, NULL);
		break;

		//开始抓取
	case VK_DOWN:
		GameState = 2;
		//SetTimer(hwnd, TIME_ID2, dwTimerElapse2, NULL);
		break;

	}

}
