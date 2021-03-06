#include<Windows.h>
#include<math.h>
#include"LinkList.h"
//---------宏定义---------
//定义常量
#define size1 12//2//1//小金块尺寸
#define size2 25//3//2//中金块尺寸
#define size3 60//5//4//大金块尺寸
#define num 20//金块总数
#define num1 10//小金块数量
#define num2 6//中金块数量
#define num3 4//大金块数量
#define score1 20//小金块分数
#define score2 100//中金块分数
#define score3 500//大金块分数
#define hookstartlen 50//初始钩子的绳长
#define rotation 0.0314159/4//每0.1秒旋转的角度1.8°（弧度制）
#define hspeed 3.5//钩子下放时的速度
#define pi 3.1//180°的弧度

//计时器ID
#define TIME_ID1 10
//#define TIME_ID2 20
#define TIME_ID3 30


//游戏坐标
typedef struct _GAME_COORD
{
	int x;
	int y;
}GAME_COORD, *PGAME_COORD;

//一个金块
typedef struct _Gold
{
	PGAME_COORD coord;
	int size;
}*PGold;


//--------全局变量--------

//计分器，初始为零
int score;

//钩子
PGAME_COORD hook;

//钩子移动前的位置
PGAME_COORD exhook;

//金块（链表）
PLIST gold_list;

//游戏边界
PGAME_COORD boundary;

//金块边界
PGAME_COORD goldbounary;

//钩子旋转状态 右→左为1  左→右为0 初始为1
int hooktoleft;

//钩子此时旋转与定点左右两侧边界夹角 初始为零
float angle;

//矿工所在的定点
GAME_COORD point;

//倒计时器
int second;

//钩子移动的状态
int hookback;
int hookmove;

//---------函数声明--------

//---------边界--------

//设置边界坐标
void SetBoundary(int x, int y);

// 获得边界坐标
PGAME_COORD GetBoundary();

//设置金块边界坐标
void SetGoldBoundary();

//---------金块--------

//生成一个新的金块 随机生成坐标
PGold CreatGold(int size);


//获得金块
PGold GetGold(int n);

//获得金块坐标
PGAME_COORD GetGoldAt(int n);


//创建所有金块的链表
void CreatAllGolds();


//---------钩子--------

//创建钩子即初始坐标
void CreatHook();

//获得钩子坐标
PGAME_COORD GetHook();

//钩子转动
void HookRoll();

//钩子移动
void HookMove();

//钩子回到初始点
void HookBack();

//是否抓取到金子
int CoordEqual(PGAME_COORD hook, PGold g);

//抓取金子
int CatchGold();

//碰到边界
int CatchBoundary();

/// game_opration.c 中的接口函数

int GetScore();

int GetSecond();

void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse1,
	//DWORD dwInitTimerElapse2,
	DWORD dwInitTimerElapse3,
	int boundary_x, 
	int boundary_y//边界
	);
void OnRollTimer(HWND hwnd);

//void OnMoveTimer(HWND hwnd);

void OnSecondTimer(HWND hwnd);

void OnKeyDown(DWORD vk, DWORD hwnd);

