#include"wall.h"
Wall::Wall(short l, short r, short t, short b) :LEFT(l), RIGHT(r), TOP(t), BUTTOM(b) 
{
		gotoxy(LEFT, TOP);//��(LEFT,TOP)������Ͻ�ǽ��
		cout << "�X";
		for (short i = LEFT + 2; i <= RIGHT - 3; i += 2)//x��LEFT+2-RIGHT-3��Χ�����Χǽ��
		{
			gotoxy(i, TOP);
			cout << "�T";
		}
		gotoxy(RIGHT - 1, TOP);//��(RIGHT-1,TOP)������Ͻ�ǽ��
		cout << "�[";
	for (short i = TOP + 1; i <= BUTTOM - 1; i++) 
	{//y��TOP-BUTTOM-1��Χ�����Χ,��Χǽ��
		gotoxy(LEFT, i);
		cout << "��";
		gotoxy(RIGHT-1, i);
		cout << "��";
	}
	gotoxy(LEFT, BUTTOM);//��(TOP,BUTTOM)������½�ǽ��
	cout << "�^";
	for (short i = LEFT + 2; i <= RIGHT - 3; i += 2)
	{
		gotoxy(i, BUTTOM);//x��LEFT+2-RIGHT-3��Χ�����Χǽ��
		cout << "�T";
	}
	gotoxy(RIGHT - 1, BUTTOM);//��(RIGHT-1,BUTTOM)������½�ǽ��
	cout << "�a";
}