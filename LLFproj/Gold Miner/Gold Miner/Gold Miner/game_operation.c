//---------ͷ�ļ�--------
#include<Windows.h>
#include"hook.h"


//--------ȫ�ֱ���---------
//��ʱ����ʱ����
DWORD dwTimerElapse1;
//DWORD dwTimerElapse2;
DWORD dwTimerElapse3;



//---------��������--------
//���ط���
int GetScore() { return score; }

//����ʱ��
int GetSecond() { return second; }

void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse1,
	//DWORD dwInitTimerElapse2,
	DWORD dwInitTimerElapse3,
	int boundary_x, 
	int boundary_y//�߽�
)
{ 
	dwTimerElapse1 = dwInitTimerElapse1;
	//dwTimerElapse2 = dwInitTimerElapse2;
	dwTimerElapse3 = dwInitTimerElapse3;
	SetBoundary(boundary_x, boundary_y);

	//���ý��߽�����
	SetGoldBoundary();

	//���ɽ������ݽṹ
	CreatAllGolds();

	//���ɹ��ӵ����ݽṹ
	CreatHook();

	//��ʼ��ת�����ͷ���
	hooktoleft = 1;
	angle = 0;
	score = 0;
	second = 120;
	hookback = 0;
	hookmove = 0;

	// ������ʱ��
	// ÿ���� dwTimerElapse ���룬hwnd���ڣ������ڣ��ͻ��յ�һ��WM_TIMER��Ϣ��
	// ��ʱ������������Ϸ���е���Ҫʱ���ߡ�
	//SetTimer(hwnd, TIME_ID1, dwTimerElapse1, NULL);
	//SetTimer(hwnd, TIME_ID2, dwTimerElapse2, NULL);
	//SetTimer(hwnd, TIME_ID3, dwTimerElapse3, NULL);
}

//��Ϸ���Ƶ���Ҫ����
//����ʱ������ʱ���еĴ����߼�
void OnRollTimer(HWND hwnd)
{
	    if (hookmove == 0&&hookback==0&&angle<pi)
		HookRoll();
		
		else if (hookmove == 1&&hookback == 0&& angle<pi)
		{
			HookMove();
			CatchGold();
			CatchBoundary();
		}
		else if (hookmove == 1&&hookback==1&&angle<pi)
		{	 
			HookBack();
        }
	
}

//void OnMoveTimer(HWND hwnd)
//{
//	KillTimer(hwnd, TIME_ID1);
//	HookMove();
//	CatchGold();
//	CatchBoundary();
//	if (hookback)
//	{
//	
//		HookBack();
//	}
//}

void OnSecondTimer(HWND hwnd)
{
	second--;
	if (GetSecond() == 0)
	{
		KillTimer(hwnd, TIME_ID1);
		//KillTimer(hwnd, TIME_ID2);
		KillTimer(hwnd, TIME_ID3);

		//��������
		MessageBox(0, score, "Your Score", 0);
		ExitProcess(0);
	}
}


//�����ڲ���ʱ���е��߼�����
//����Ϸֻʹ���˿ո�ͼ��̵��¼�����
void OnKeyDown(DWORD vk,DWORD hwnd)
{
	switch (vk)//virtual key value
	{
		//��ʼ��Ϸ
	case VK_SPACE:
		//KillTimer(hwnd, TIME_ID2);
		SetTimer(hwnd, TIME_ID1, dwTimerElapse1, NULL);
		SetTimer(hwnd, TIME_ID3, dwTimerElapse3, NULL);
		break;

		//��ʼץȡ
	case VK_DOWN:
		//KillTimer(hwnd, TIME_ID1);
		hookmove = 1;
		//SetTimer(hwnd, TIME_ID2, dwTimerElapse2, NULL);
		break;

	}

}

