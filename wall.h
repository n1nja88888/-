#pragma once
#include"coordinate.h"
#include<iostream>//�����׼�������������
using namespace std;
class Wall 
{
public:
	Wall():LEFT(0),RIGHT(0),TOP(0),BUTTOM(0){}//Ĭ�Ϲ��캯��
	Wall(short l, short r, short t, short b);//���캯��,cmdΪ���������Χǽ��
	short getLeft() const { return LEFT; }//LEFT����ӿ�
	short getRight() const { return RIGHT; }
	short getTop() const { return TOP; }
	short getButtom() const { return BUTTOM; }
private:
	short LEFT;//ǽ�����������
	short RIGHT;
	short TOP;
	short BUTTOM;
};