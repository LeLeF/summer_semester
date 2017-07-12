#include"hook.h"


//---------�ڲ���������---------
//�ж��Ƿ��̵����
int CoordEqual(PGAME_COORD hook, PGold g);

//---------------��������---------------
//�ж��Ƿ��̵����
int CoordEqual(PGAME_COORD hook, PGold g)
{
	int flag = 0;
	for(int i=-(1/2)*(g->size-1);i<= (1 / 2)*(g->size - 1);i++)
		for (int j = -(1 / 2)*(g->size - 1); j <= (1 / 2)*(g->size - 1); j++)
		{
			if ((int)(hook->x) == g->coord->x + i&&
				(int)(hook->y) == g->coord->y + j)
				flag++;
		}
	if(flag!=0)
		return 1;
	else return 0;

}



//---------�߽�--------

//���ñ߽�����
void SetBoundary(int x, int y)
{
	boundary= (PGAME_COORD)malloc(sizeof(GAME_COORD));
	boundary->x = x;
	boundary->y = y;
}

// ��ñ߽�����
PGAME_COORD GetBoundary()
{
	return boundary;
}

//���ý��߽�����
void SetGoldBoundary()
{
	goldbounary = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	goldbounary->x = boundary->x;
	goldbounary->y =( (boundary->y)/2);
}

//---------���--------

//����һ���µĽ�� �����������
PGold CreatGold(int size)
{
	PGold g;
	g = (PGold)malloc(sizeof(struct _Gold));
	g->coord = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	g->size = size;
	g->coord->x = rand()%(goldbounary->x);
	g->coord->y = rand() %(60-20)+20/*( (boundary->y - goldbounary->y)+ goldbounary->y)*/;
	return g;
}

//��ý��
PGold GetGold(int n)
{
	if (n < num)
		return ListGetAt(gold_list, n);
	else
		return NULL;
}

//��ý������
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

//�������н�������
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

//---------����--------

//��������
void CreatHook()
{
	//������ʼ��
	point.x = boundary->x*1/2;
	point.y = boundary->y * 1 / 5;
	//��ʼ�����ӳ�ʼ����
	//Ĭ��Ϊ���ҡ���ʼ
	hook= (PGAME_COORD)malloc(sizeof(GAME_COORD));
	hook->x = point.x + hookstartlen;
	hook->y = point.y;
	hooktoleft = 1;

}

//��ù�������
PGAME_COORD GetHook()
{
	return hook;
}

//����ת��
void HookRoll()
{
	if (hooktoleft = 1 && angle < pi)
	{
		angle += rotation;
		hook->x = point.x + hookstartlen*cos(angle);
		hook->y = point.y + hookstartlen*sin(angle);
	}
	else if (hooktoleft = 1 && angle >= pi)
	{
		angle = 0;
		hooktoleft = 0;
		hook->x = point.x - hookstartlen;
		hook->x = point.y;
	}
	else if (hooktoleft = 0 && angle < pi)
	{
		angle += rotation;
		hook->x = point.x - hookstartlen*cos(angle);
		hook->y = point.y + hookstartlen*sin(angle);
	}
	else if (hooktoleft = 0 && angle >= pi)
	{
		angle = 0;
		hooktoleft = 1;
		hook->x = point.x + hookstartlen;
		hook->x = point.y;
	}
}


//�����ƶ�
void HookMove()
{
	if (hooktoleft = 1 && angle < pi)
	{
		hook->x += hspeed*cos(angle);
		hook->y += hspeed*sin(angle);
	}
	else if (hooktoleft = 0 && angle < pi)
	{
		hook->x -= hspeed*cos(angle);
		hook->y += hspeed*sin(angle);
	}
}

//���ӻص���ʼ��
void HookBack()
{
	if (hooktoleft = 1 && angle < pi)
	{
		hook->x -= hspeed*cos(angle);
		hook->y -= hspeed*sin(angle);
	}
	else if (hooktoleft = 0 && angle < pi)
	{
		hook->x += hspeed*cos(angle);
		hook->y -= hspeed*sin(angle);
	}
	else if(hook->x == point.x&&hook->y==point.y)
	GameState = 1;

}

void CatchGold()
{
	PGold p;
	p= (PGold)malloc(sizeof(struct _Gold));
	for (int i = 0; i < num; i++)
	{
		p = ListGetAt(gold_list, i);
		if (CoordEqual(hook,p))
		{
			p->coord->x = 0;
			p->coord->y = 0;
			switch (p->size)
			{
			case 1:
				score += score1;
				break;
			case 2:
				score += score2;
				break;
			case 3:
				score += score3;
				break;
			}
			HookBack();
			break;
		}
	}

}

void CatchBoundary()
{
	if ((int)hook->x == (int)boundary->x || (int)hook->y == (int)boundary->y || (int)hook->x == 0)
	{
		HookBack();
	}
}