/*����¼:
* 170ע����һ�д���
*/

#include"block.h"
#include"wall.h"
#include<Windows.h>//����system(),Sleep()
#include<conio.h>//����_kbhit(),_getch()����
#include<ctime>//����time()����
#include<cstdlib>//����rand(),srand()����
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
//���������Ѷȵȼ����Ѷ�ʱ��Ĵ����߳�,����ר�ŷ�װ�ɺ���,ʹ������������,�ṹ����
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
	else if (point < 15000)//�Ż�
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
/*����˹������Ϸ���õ��Ĺ�ʽ
* point = level * 5 * ��������
*/
//Sleep()�����ڲ���
int reactionTime = 10;//��Ӧʱ��
int difficultyTime = 300;//�Ѷ�ʱ��
int animationTime = 80;//����ʱ��
int flowTime = 30;//�����ƶ�����ʱ�ʵ�ͣ�ٵĽ����ʱ��
//��ֹ�ʽ:����*10     �����ɿ����ٰ��ٶȼ����ȥ
int main()
{
	//-------------------------------------------------------------------------------------���Ϸ����
	system("mode con cols=60 lines=38");//����̨��С
	system("title ����˹����");//����̨����
	hideCursor();//���ع��
	Wall framework(5, 56, 3, 30);//��Ϸ�����
	gotoxy(15, 3);
	cout << "Welcome To The NINJA88888 World";//ˮӡ
	Wall displayBox(11, 34, 6, 27);//��Ϸ��ʾ��
	//������Ϸ����
	for (short i = 7; i < 27; i++)
	{
		gotoxy(13, i);
		cout << "��������������������";
	}
	Block::setWall(displayBox);//����Ϸ��ʾ�����Ϣ��ֵ��Block��ľ�̬��Ա
	//������������
	short erasedLine = 0;
	//����÷�
	short point = 0;
	gotoxy(39, 7);
	cout << "�÷�";
	gotoxy(48, 9);
	cout << point;
	//������������
	short line = 0;
	gotoxy(39, 11);
	cout << "��������";
	gotoxy(48, 13);
	cout << line;
	//�����Ѷȵȼ�
	short level = 1;
	gotoxy(39, 15);
	cout << "�Ѷȵȼ�";
	gotoxy(48, 17);
	cout << level;
	gotoxy(39, 19);
	cout << "�¸�����";
	gotoxy(43, 21);
	cout << "��������";
	gotoxy(43, 22);
	cout << "��������";
	//��ӡ��Ϸ������ʾ
	gotoxy(15, 32);
	cout << "(��)��ת";
	gotoxy(9, 34);
	cout << "(��)����     (��)����";
	gotoxy(15, 36);
	cout << "(��)����";
	gotoxy(35, 32);
	cout << "(P)��ͣ  (R)���¿�ʼ";
	gotoxy(40, 35);
	cout << "(�ո�)���";
	//-------------------------------------------------------------------------------------��Ϸ����
	char cmd = ' ';//�����û���������
	Block* block = newBlock(true);//���浱ǰ����
	Block* preblock = newBlock(false);//������һ������
	preblock->print(Coordinate(43, 21));//��Ϸ�տ�ʼʱ����ʾ�¸�����
	//-------------------------------------------��Ϸ����
newGame:
	while (1)
	{
	playing:
		for (short i = 0; i < 5; i++)//��ദ�������û�����
		{
			if (_kbhit())
			{
				cmd = _getch();
				switch (cmd)
				{
				case ' ':
					block->translation(' ');
					goto computing;//ֱ�ӽ��봦��
				case 'p'://������д��Сд
				case 'P':
					gotoxy(39, 24);
					cout << "(��Ϸ��ͣ��)";
					while (1)//���ｨ��һ����ѭ��,�����û����°���,�Ҽ���Ϊpʱ����ѭ��
					{
						if (_kbhit())
						{
							cmd = _getch();
							if (cmd == 'p' || cmd == 'P')//������д��Сд
							{
								break;
							}
						}
					}
					gotoxy(39, 24);
					cout << "            ";
					break;
				case -32://��������
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
						for (short i = 0; i < 3; i++)//һ������������
						{
							if (!block->crash())
							{
								Sleep(flowTime);//���������ٶ�,���û��ٴ��
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
		//����ѭ��������,ͳһִ���������,����ײ������,��ײ������ںϵȲ���
		if (!block->crash())//����ײ
		{
			Sleep(difficultyTime);//���������ٶ�,�����Ѷ�
			block->fall();
		}
		else//������ײ
		{
			if (block->isOver())
			{
				break;//Ψһ��Ϸ�����˳���
			}
			if (cmd != ' ')//����ǿո�ָ��������,��ִ���������
			{
				for (int i = 0; i < 10; i++)//�ж�����ײ����������5��ƽ��
				{
					Sleep(3 * reactionTime);//���û����뷴Ӧʱ��,���������ʱ�䷴Ӧ
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
				if (!block->crash())//�������ոյ�ƽ�ƺ�,���鲻�ᷢ����ײ��,��ת�Ƶ�����ʱ����
				{
					goto playing;
				}
			}
			//�����˳����н���,���밴���ں�,��ˢ��,���ɾ�����˳��ִ�д���
			block->mix();
			//ˢ����������
			erasedLine = block->eraseLine();
			//ˢ�·���
			point += level * 10 * erasedLine;
			gotoxy(48, 9);
			cout << point;
			//ˢ����������
			line += erasedLine;
			gotoxy(48, 13);
			cout << line;
			//���ݷ���ˢ�µȼ�
			setLevel(point, level);
			gotoxy(48, 17);
			cout << level;
			//ˢ���Ѷ�ʱ��
			setDifficultTime(level, difficultyTime);
			delete block;//ɾ������
			block = preblock;
			block->print();
			preblock = newBlock(false);
			preblock->print(Coordinate(43, 21));
		}
	}
	//-------------------------------------------------------------------------------------��Ϸ����
	//�����Ͼ�����
	//�����ȱ��������������¸�ֵ,�ʲ���Ҫ������ˢ��
	delete block;
	delete preblock;
	Block::resetGround();
	for (int lines = 26; lines > 6; lines--)//��Ϸ��������
	{
		gotoxy(13, lines);
		cout << "��������������������";
		Sleep(animationTime);
	}
	for (int lines = 7; lines < 27; lines++)//��Ϸ��������
	{
		gotoxy(13, lines);
		cout << "��������������������";
		Sleep(animationTime);
	}
	//�ı䲿����Ϸ����
	gotoxy(39, 7);
	cout << "�Ͼ����յ÷�";
	gotoxy(19, 16);
	cout << "��Ϸ����";
	while (1)//���ｨ��һ����ѭ��,�����û����°���,�Ҽ���Ϊrʱ����ѭ��
	{
		Sleep(reactionTime);
		if (_kbhit())
		{
			cmd = _getch();
			if (cmd == 'r' || cmd == 'R')//������д��Сд
			{
				break;//����׼����һ�ֽ׶�
			}
	
		}
	}
	//��ʰ����,Ϊ��һ������׼��
	//����ʰ����
	gotoxy(39, 7);
	cout << "            ";
	for (short i = 7; i < 27; i++)
	{
		gotoxy(13, i);
		cout << "��������������������";
	}
	//����ʰ����
	//����
	point = 0;
	gotoxy(39, 7);
	cout << "�÷�";
	gotoxy(48, 9);
	cout << "       ";//��ʰ�Ͼֲ��������
	gotoxy(48, 9);
	cout << point;
	//��������
	line = 0;
	gotoxy(48, 13);
	cout << "       ";//��ʰ�Ͼֲ��������
	gotoxy(48, 13);
	cout << line;
	//�Ѷȵȼ�
	level = 1;
	gotoxy(48, 17);
	cout << "       ";//��ʰ�Ͼֲ��������
	gotoxy(48, 17);
	cout << level;
	gotoxy(39, 19);
	cout << "�¸�����";
	gotoxy(43, 21);
	cout << "��������";
	gotoxy(43, 22);
	cout << "��������";
	//���׼���÷���
	block = newBlock(true);
	preblock = newBlock(false);
	preblock->print(Coordinate(43, 21));
	goto newGame;
	return 0;
}