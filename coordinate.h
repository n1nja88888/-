#pragma once
#include<Windows.h>//����COORD��,���ù��λ�ú���
#include<utility>//ͨ��<��==�������������
struct Coordinate 
{//��ά����ṹ��,�洢����
	short x;//x����
	short y;//y����
	Coordinate(short x, short y) :x(x), y(y) {}//���캯��
	bool operator <(const Coordinate& coord)const;//����<�����
};
void gotoxy(short x, short y);//���ù��λ��
