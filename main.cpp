/*备忘录:
* 170注释了一行代码
*/

#include"block.h"
#include"wall.h"
#include<Windows.h>//引入system(),Sleep()
#include<conio.h>//引入_kbhit(),_getch()函数
#include<ctime>//引入time()函数
#include<cstdlib>//引入rand(),srand()函数
using namespace std;
void hideCursor()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}
Block* newBlock(bool cmd)
{
	srand(time(NULL));
	switch (rand() % 7)
	{
	case 0:
		return new Square(cmd);
	case 1:
		return new Stick(cmd);
	case 2:
		return new LeftCane(cmd);
	case 3:
		return new RightCane(cmd);
	case 4:
		return new LeftLightning(cmd);
	case 5:
		return new RightLightning(cmd);
	case 6:
		return new Hill(cmd);
	}
}
//由于设置难度等级和难度时间的代码冗长,这里专门封装成函数,使主函数代码简洁,结构清晰
void setLevel(short point,short& level)
{
	if (point < 120)
		level = 1;
	else if (point < 500)
		level = 2;
	else if (point < 1000)
		level = 3;
	else if (point < 2000)
		level = 4;
	else if (point < 4000)
		level = 5;
	else if (point < 8000)
		level = 6;
	else if (point < 15000)//放缓
		level = 7;
	else if (point < 20000)
		level = 8;
	else if (point < 28000)
		level = 9;
	else if (point < 32000)
		level = 10;
}
void setDifficultTime(short level, int& difficultTime)
{
	switch (level)
	{
	case 2:
		difficultTime = 235;
		break;
	case 3:
		difficultTime = 215;
		break;
	case 4:
		difficultTime = 180;
		break;
	case 5:
		difficultTime = 150;
		break;
	case 6:
		difficultTime = 120;
		break;
	case 7:
		difficultTime = 105;
		break;
	case 8:
		difficultTime = 95;
		break;
	case 9:
		difficultTime = 80;
		break;
	case 10:
		difficultTime = 70;
		break;
	}
}
/*俄罗斯方块游戏内用到的公式
* point = level * 5 * 消除行数
*/
//Sleep()函数内参数
int reactionTime = 10;//反应时间
int difficultyTime = 300;//难度时间
int animationTime = 80;//动画时间
int flowTime = 30;//方块移动下落时适当停顿的节奏感时间
//算分公式:行数*10     后续可考虑再把速度加入进去
int main()
{
	//-------------------------------------------------------------------------------------搭建游戏界面
	system("mode con cols=60 lines=38");//控制台大小
	system("title 俄罗斯方块");//控制台标题
	hideCursor();//隐藏光标
	Wall framework(5, 56, 3, 30);//游戏界面框
	gotoxy(15, 3);
	cout << "Welcome To The NINJA88888 World";//水印
	Wall displayBox(11, 34, 6, 27);//游戏显示框
	//铺设游戏背景
	for (short i = 7; i < 27; i++)
	{
		gotoxy(13, i);
		cout << "□□□□□□□□□□";
	}
	Block::setWall(displayBox);//将游戏显示框的信息赋值给Block类的静态成员
	//储存已销行数
	short erasedLine = 0;
	//储存得分
	short point = 0;
	gotoxy(39, 7);
	cout << "得分";
	gotoxy(48, 9);
	cout << point;
	//储存已消行数
	short line = 0;
	gotoxy(39, 11);
	cout << "已消行数";
	gotoxy(48, 13);
	cout << line;
	//储存难度等级
	short level = 1;
	gotoxy(39, 15);
	cout << "难度等级";
	gotoxy(48, 17);
	cout << level;
	gotoxy(39, 19);
	cout << "下个方块";
	gotoxy(43, 21);
	cout << "□□□□";
	gotoxy(43, 22);
	cout << "□□□□";
	//打印游戏按键提示
	gotoxy(15, 32);
	cout << "(↑)旋转";
	gotoxy(9, 34);
	cout << "(←)左移     (→)右移";
	gotoxy(15, 36);
	cout << "(↓)下移";
	gotoxy(35, 32);
	cout << "(P)暂停  (R)重新开始";
	gotoxy(40, 35);
	cout << "(空格)落地";
	//-------------------------------------------------------------------------------------游戏内容
	char cmd = ' ';//储存用户输入命令
	Block* block = newBlock(true);//储存当前方块
	Block* preblock = newBlock(false);//储存下一个方块
	preblock->print(Coordinate(43, 21));//游戏刚开始时先提示下个方块
	//-------------------------------------------游戏主体
newGame:
	while (1)
	{
	playing:
		for (short i = 0; i < 5; i++)//最多处理三次用户键入
		{
			if (_kbhit())
			{
				cmd = _getch();
				switch (cmd)
				{
				case ' ':
					block->translation(' ');
					goto computing;//直接进入处理
				case 'p'://包括大写和小写
				case 'P':
					gotoxy(39, 24);
					cout << "(游戏暂停中)";
					while (1)//这里建立一个死循环,仅在用户按下按键,且键入为p时结束循环
					{
						if (_kbhit())
						{
							cmd = _getch();
							if (cmd == 'p' || cmd == 'P')//包括大写和小写
							{
								break;
							}
						}
					}
					gotoxy(39, 24);
					cout << "            ";
					break;
				case -32://方向键情况
					switch (cmd = _getch())
					{
					case 75:
						block->translation('a');
						Sleep(3 * flowTime);
						break;
					case 72:
						block->rotation();
						Sleep(3 * flowTime);
						break;
					case 77:
						block->translation('d');
						Sleep(3 * flowTime);
						break;
					case 80:
						for (short i = 0; i < 3; i++)//一口气下落三次
						{
							if (!block->crash())
							{
								Sleep(flowTime);//限制下落速度,给用户顿挫感
								block->fall();
							}
						}
						break;
					}
					break;
				}
			}
		}
	computing:
		while (_kbhit())
		{
			_getch();
		}
		//键入循环结束后,统一执行下面代码,不碰撞则下落,碰撞则进行融合等操作
		if (!block->crash())//不碰撞
		{
			Sleep(difficultyTime);//控制下落速度,控制难度
			block->fall();
		}
		else//发生碰撞
		{
			if (block->isOver())
			{
				break;//唯一游戏内容退出口
			}
			if (cmd != ' ')//如果是空格指令下来的,则不执行下面代码
			{
				for (int i = 0; i < 10; i++)//判断是碰撞后允许至多5次平移
				{
					Sleep(3 * reactionTime);//给用户键入反应时间,这里给更多时间反应
					if (_kbhit())
					{
						cmd = _getch();
						if (cmd == -32)
						{
							cmd = _getch();
							switch (cmd)
							{
							case 75:
								block->translation('a');
								break;
							case 77:
								block->translation('d');
								break;
							}
						}
					}
				}
				if (!block->crash())//如果结果刚刚的平移后,方块不会发生碰撞了,则转移到键入时代码
				{
					goto playing;
				}
			}
			//这里的顺序很有讲究,必须按先融合,再刷新,最后删方块的顺序执行代码
			block->mix();
			//刷新已消行数
			erasedLine = block->eraseLine();
			//刷新分数
			point += level * 10 * erasedLine;
			gotoxy(48, 9);
			cout << point;
			//刷新已消行数
			line += erasedLine;
			gotoxy(48, 13);
			cout << line;
			//依据分数刷新等级
			setLevel(point, level);
			gotoxy(48, 17);
			cout << level;
			//刷新难度时间
			setDifficultTime(level, difficultyTime);
			delete block;//删除方块
			block = preblock;
			block->print();
			preblock = newBlock(false);
			preblock->print(Coordinate(43, 21));
		}
	}
	//-------------------------------------------------------------------------------------游戏结束
	//清理上局内容
	//分数等变量会在上面重新赋值,故不需要在这里刷新
	delete block;
	delete preblock;
	Block::resetGround();
	for (int lines = 26; lines > 6; lines--)//游戏结束动画
	{
		gotoxy(13, lines);
		cout << "■■■■■■■■■■";
		Sleep(animationTime);
	}
	for (int lines = 7; lines < 27; lines++)//游戏结束动画
	{
		gotoxy(13, lines);
		cout << "□□□□□□□□□□";
		Sleep(animationTime);
	}
	//改变部分游戏画面
	gotoxy(39, 7);
	cout << "上局最终得分";
	gotoxy(19, 16);
	cout << "游戏结束";
	while (1)//这里建立一个死循环,仅在用户按下按键,且键入为r时结束循环
	{
		Sleep(reactionTime);
		if (_kbhit())
		{
			cmd = _getch();
			if (cmd == 'r' || cmd == 'R')//包括大写和小写
			{
				break;//进入准备新一轮阶段
			}
	
		}
	}
	//收拾画面,为新一轮做好准备
	//先收拾画面
	gotoxy(39, 7);
	cout << "            ";
	for (short i = 7; i < 27; i++)
	{
		gotoxy(13, i);
		cout << "□□□□□□□□□□";
	}
	//再收拾变量
	//分数
	point = 0;
	gotoxy(39, 7);
	cout << "得分";
	gotoxy(48, 9);
	cout << "       ";//收拾上局残余的数字
	gotoxy(48, 9);
	cout << point;
	//已消行数
	line = 0;
	gotoxy(48, 13);
	cout << "       ";//收拾上局残余的数字
	gotoxy(48, 13);
	cout << line;
	//难度等级
	level = 1;
	gotoxy(48, 17);
	cout << "       ";//收拾上局残余的数字
	gotoxy(48, 17);
	cout << level;
	gotoxy(39, 19);
	cout << "下个方块";
	gotoxy(43, 21);
	cout << "□□□□";
	gotoxy(43, 22);
	cout << "□□□□";
	//最后准备好方块
	block = newBlock(true);
	preblock = newBlock(false);
	preblock->print(Coordinate(43, 21));
	goto newGame;
	return 0;
}