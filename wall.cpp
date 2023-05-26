#include"wall.h"
Wall::Wall(short l, short r, short t, short b) :LEFT(l), RIGHT(r), TOP(t), BUTTOM(b) 
{
		gotoxy(LEFT, TOP);//在(LEFT,TOP)输出左上角墙壁
		cout << "╔";
		for (short i = LEFT + 2; i <= RIGHT - 3; i += 2)//x轴LEFT+2-RIGHT-3范围输出上围墙壁
		{
			gotoxy(i, TOP);
			cout << "═";
		}
		gotoxy(RIGHT - 1, TOP);//在(RIGHT-1,TOP)输出右上角墙壁
		cout << "╗";
	for (short i = TOP + 1; i <= BUTTOM - 1; i++) 
	{//y轴TOP-BUTTOM-1范围输出左围,右围墙壁
		gotoxy(LEFT, i);
		cout << "‖";
		gotoxy(RIGHT-1, i);
		cout << "‖";
	}
	gotoxy(LEFT, BUTTOM);//在(TOP,BUTTOM)输出左下角墙壁
	cout << "╚";
	for (short i = LEFT + 2; i <= RIGHT - 3; i += 2)
	{
		gotoxy(i, BUTTOM);//x轴LEFT+2-RIGHT-3范围输出下围墙壁
		cout << "═";
	}
	gotoxy(RIGHT - 1, BUTTOM);//在(RIGHT-1,BUTTOM)输出右下角墙壁
	cout << "╝";
}