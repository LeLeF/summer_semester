//---------ͷ�ļ�--------
#include<Windows.h>
#include"hook.h"

//---------�궨��---------
//��ʱ��ID
#define TIME_ID1 10
#define TIME_ID2 20
#define TIME_ID3 30

//--------ȫ�ֱ���---------
//��ʱ����ʱ����
DWORD dwTimerElapse1;
DWORD dwTimerElapse2;
DWORD dwTimerElapse3;

////��Ϸ״̬
//int GameState = 0;

////�Ʒ�������ʼΪ��
//int score = 0;

//����ʱ����
int second = 120;

//---------��������--------
//���ط���
int GetScore() { return score; }

//����ʱ��
int GetSecond() { return second; }

//����ʱ
void SecondDecline()
{
	second--;
}




void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse1,
	DWORD dwInitTimerElapse2,
	DWORD dwInitTimerElapse3,
	int boundary_x, 
	int boundary_y//�߽�
)
{ 
	dwTimerElapse1 = dwInitTimerElapse1;
	dwTimerElapse2 = dwInitTimerElapse2;
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

	//��ʼ����Ϸ״̬
	GameState = 0;
	score = 0;

	// ������ʱ��
	// ÿ���� dwTimerElapse ���룬hwnd���ڣ������ڣ��ͻ��յ�һ��WM_TIMER��Ϣ��
	// ��ʱ������������Ϸ���е���Ҫʱ���ߡ�
	//SetTimer(hwnd, TIME_ID1, dwTimerElapse1, NULL);
	//SetTimer(hwnd, TIME_ID2, dwTimerElapse2, NULL);
	SetTimer(hwnd, TIME_ID3, dwTimerElapse3, NULL);
}

//��Ϸ���Ƶ���Ҫ����
//����ʱ������ʱ���еĴ����߼�
void OnTimer(DWORD hwnd,UINT_PTR timeid)
{
	switch(timeid)
	{	

	   //����ʱ����������������ֹͣ��ʱ������������
		case 0:
			KillTimer(hwnd, TIME_ID1);
			KillTimer(hwnd, TIME_ID2);
			KillTimer(hwnd, TIME_ID3);

			//��������
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

//�����ڲ���ʱ���е��߼�����
//����Ϸֻʹ���˿ո�ͼ��̵��¼�����
void OnKeyDown(DWORD vk,DWORD hwnd)
{
	switch (vk)//virtual key value
	{
		//��ʼ��Ϸ
	case VK_SPACE:
		GameState = 1;
		//SetTimer(hwnd, TIME_ID1, dwTimerElapse1, NULL);
		break;

		//��ʼץȡ
	case VK_DOWN:
		GameState = 2;
		//SetTimer(hwnd, TIME_ID2, dwTimerElapse2, NULL);
		break;

	}

}
