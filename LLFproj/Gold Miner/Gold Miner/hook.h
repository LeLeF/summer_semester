#include<Windows.h>
#include<math.h>
#include"LinkList.h"
//---------�궨��---------
//���峣��
#define size1 12//2//1//С���ߴ�
#define size2 25//3//2//�н��ߴ�
#define size3 60//5//4//����ߴ�
#define num 20//�������
#define num1 10//С�������
#define num2 6//�н������
#define num3 4//��������
#define score1 20//С������
#define score2 100//�н�����
#define score3 500//�������
#define hookstartlen 50//��ʼ���ӵ�����
#define rotation 0.0314159/4//ÿ0.1����ת�ĽǶ�1.8�㣨�����ƣ�
#define hspeed 3.5//�����·�ʱ���ٶ�
#define pi 3.1//180��Ļ���

//��ʱ��ID
#define TIME_ID1 10
//#define TIME_ID2 20
#define TIME_ID3 30


//��Ϸ����
typedef struct _GAME_COORD
{
	int x;
	int y;
}GAME_COORD, *PGAME_COORD;

//һ�����
typedef struct _Gold
{
	PGAME_COORD coord;
	int size;
}*PGold;


//--------ȫ�ֱ���--------

//�Ʒ�������ʼΪ��
int score;

//����
PGAME_COORD hook;

//�����ƶ�ǰ��λ��
PGAME_COORD exhook;

//��飨����
PLIST gold_list;

//��Ϸ�߽�
PGAME_COORD boundary;

//���߽�
PGAME_COORD goldbounary;

//������ת״̬ �ҡ���Ϊ1  �����Ϊ0 ��ʼΪ1
int hooktoleft;

//���Ӵ�ʱ��ת�붨����������߽�н� ��ʼΪ��
float angle;

//�����ڵĶ���
GAME_COORD point;

//����ʱ��
int second;

//�����ƶ���״̬
int hookback;
int hookmove;

//---------��������--------

//---------�߽�--------

//���ñ߽�����
void SetBoundary(int x, int y);

// ��ñ߽�����
PGAME_COORD GetBoundary();

//���ý��߽�����
void SetGoldBoundary();

//---------���--------

//����һ���µĽ�� �����������
PGold CreatGold(int size);


//��ý��
PGold GetGold(int n);

//��ý������
PGAME_COORD GetGoldAt(int n);


//�������н�������
void CreatAllGolds();


//---------����--------

//�������Ӽ���ʼ����
void CreatHook();

//��ù�������
PGAME_COORD GetHook();

//����ת��
void HookRoll();

//�����ƶ�
void HookMove();

//���ӻص���ʼ��
void HookBack();

//�Ƿ�ץȡ������
int CoordEqual(PGAME_COORD hook, PGold g);

//ץȡ����
int CatchGold();

//�����߽�
int CatchBoundary();

/// game_opration.c �еĽӿں���

int GetScore();

int GetSecond();

void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse1,
	//DWORD dwInitTimerElapse2,
	DWORD dwInitTimerElapse3,
	int boundary_x, 
	int boundary_y//�߽�
	);
void OnRollTimer(HWND hwnd);

//void OnMoveTimer(HWND hwnd);

void OnSecondTimer(HWND hwnd);

void OnKeyDown(DWORD vk, DWORD hwnd);

