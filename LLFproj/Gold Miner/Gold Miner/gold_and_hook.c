#include"hook.h"
#include"math.h"


//---------内部函数声明---------
//判断是否捞到金块
int CoordEqual(PGAME_COORD hook, PGold g);

//---------------函数定义---------------
//判断是否捞到金块
int CoordEqual(PGAME_COORD hook, PGold g)
{
	float d, r;
	r = g->size*1.0;
	d = sqrt((hook->x - (g->coord->x))*(hook->x - (g->coord->x)) +
		(hook->y - (g->coord->y))*(hook->y - (g->coord->y))
	);
	if (d <= r)
	{
		hookback=1;
		return 1;
	}
	else return 0;


}



//---------边界--------

//设置边界坐标
void SetBoundary(int x, int y)
{
	boundary= (PGAME_COORD)malloc(sizeof(GAME_COORD));
	boundary->x = x;
	boundary->y = y;
}

// 获得边界坐标
PGAME_COORD GetBoundary()
{
	return boundary;
}

//设置金块边界坐标
void SetGoldBoundary()
{
	goldbounary = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	goldbounary->x = boundary->x;
	goldbounary->y =( (boundary->y)/2);
}

//---------金块--------

//生成一个新的金块 随机生成坐标
PGold CreatGold(int size)
{
	PGold g;
	g = (PGold)malloc(sizeof(struct _Gold));
	g->coord = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	g->size = size;
	g->coord->x = rand()%(goldbounary->x);
	g->coord->y = rand() %(600-200)+200/*( (boundary->y - goldbounary->y)+ goldbounary->y)*/;
	return g;
}

//获得金块
PGold GetGold(int n)
{
	if (n < num)
		return ListGetAt(gold_list, n);
	else
		return NULL;
}

//获得金块坐标
PGAME_COORD GetGoldAt(int n)
{
	PGold p;
	p= (PGold)malloc(sizeof(struct _Gold));
	
	if (n < num)
	{
		p = GetGold(n);
		return p->coord;
	}
	else return NULL;
}

//创建所有金块的链表
void CreatAllGolds()
{
	PGold g;
	PGold newg;

	gold_list = ListCreate(NULL);

	/*g = (PGold)malloc(sizeof(struct _Gold));*/
	g = CreatGold(size1);
	ListPushFront(gold_list, g);

	for (int i = 1; i < num1; i++)
	{
		/*newg= (PGold)malloc(sizeof(struct _Gold));*/
		newg= CreatGold(size1);
		ListPushFront(gold_list, newg);

	}

	for(int i=0;i<num2;i++)
	{
		/*newg = (PGold)malloc(sizeof(struct _Gold));*/
		newg = CreatGold(size2);
		ListPushFront(gold_list, newg);

	}
	for (int i = 0; i<num3; i++)
	{
		/*newg = (PGold)malloc(sizeof(struct _Gold));*/
		newg = CreatGold(size3);
		ListPushFront(gold_list, newg);

	}
	return;
}

//---------钩子--------

//创建钩子
void CreatHook()
{
	//创建初始点
	point.x = boundary->x*1/2;
	point.y = boundary->y * 1 / 5;
	//初始化钩子初始坐标
	//默认为由右→左开始
	hook= (PGAME_COORD)malloc(sizeof(GAME_COORD));
	hook->x = point.x + hookstartlen;
	hook->y = point.y;

	//创建钩子原位置
	exhook = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	exhook->x = hook->x;
	exhook->y = hook->y;
}

//获得钩子坐标
PGAME_COORD GetHook()
{
	return hook;
}


//钩子转动
void HookRoll()
{

	if (hooktoleft == 1 && angle < pi)
	{
		angle += rotation;
		hook->x = point.x + hookstartlen*cos(angle);
		hook->y = point.y + hookstartlen*sin(angle);
	}
	else if (hooktoleft == 1 && angle >= pi)
	{
		angle = 0;
		hooktoleft = 0;
		hook->x = point.x - hookstartlen;
		hook->x = point.y;
	}
	else if (hooktoleft == 0 && angle < pi)
	{
		angle += rotation;
		hook->x = point.x - hookstartlen*cos(angle);
		hook->y = point.y + hookstartlen*sin(angle);
	}
	else if (hooktoleft == 0 && angle >=pi)
	{
		angle = 0;
		hooktoleft = 1;
		hook->x = point.x + hookstartlen;
		hook->x = point.y;
	}
}


//钩子移动
void HookMove()
{
	if (hooktoleft == 1 && angle < pi)
	{
		hook->x = (hook->x + hspeed*cos(angle));
		hook->y = (hook->y + hspeed*sin(angle));
	}
	else if (hooktoleft == 1 && angle >= pi)
	{
		angle = 0;
		hooktoleft = 0;
		hook->x = point.x - hookstartlen;
		hook->x = point.y;
	}
	else if (hooktoleft == 0 && angle < pi)
	{
		hook->x = (hook->x - hspeed*cos(angle));
		hook->y = (hook->y + hspeed*sin(angle));
	}
	else if (hooktoleft == 0 && angle >= pi)
	{
		angle = 0;
		hooktoleft = 1;
		hook->x = point.x + hookstartlen;
		hook->x = point.y;
	}
}

//钩子回到初始点
void HookBack()
{
	if ((hooktoleft == 1 && angle < pi))
	{
		hook->x -= hspeed*cos(angle);
		hook->y -= hspeed*sin(angle);
		if (sqrt((hook->x - point.x)*(hook->x - point.x) + (hook->y - point.y)*(hook->y - point.y)) <= hookstartlen)
		{

			hookback = 0;
			hookmove = 0;
		
		}
	}
	else if ((hooktoleft == 0 && angle < pi))
	{
		hook->x += hspeed*cos(angle);
		hook->y -= hspeed*sin(angle);
		if (sqrt((hook->x - point.x)*(hook->x - point.x) + (hook->y - point.y)*(hook->y - point.y)) <= hookstartlen)
		{
			hookback = 0;
			hookmove = 0;
		}
	}
	else if (sqrt((hook->x-point.x)*(hook->x - point.x)+(hook->y - point.y)*(hook->y - point.y))<=hookstartlen)
	{
		hookback = 0;
		hookmove = 0;
		
	}
}

int CatchGold()
{
	PGold p;
	int flag=0;
	p = (PGold)malloc(sizeof(struct _Gold));
	for (int i = 0; i < num; i++)
	{
		p = ListGetAt(gold_list, i);
		if (CoordEqual(hook, p))
		{
			p->coord->x =0;
			p->coord->y =0;
			switch (p->size)
			{
			case size1:
				score += score1;
				break;
			case size2:
				score += score2;
				break;
			case size3:
				score += score3;
				break;
			}
			break;
			flag++;
		
		}

	}
	if (flag != 0)
	{
		hookback = 1;
		return 1;
	}
	else return 0;
}

int CatchBoundary()
{
	if ((int)hook->x >= (int)boundary->x || (int)hook->y >= (int)boundary->y || (int)hook->x == 0)
	{
		hookback = 1;
		return 1;
		
	}
	else return 0;
	
}