#pragma once
#include"coordinate.h"
#include<iostream>//引入标准输入输出流对象
using namespace std;
class Wall 
{
public:
	Wall():LEFT(0),RIGHT(0),TOP(0),BUTTOM(0){}//默认构造函数
	Wall(short l, short r, short t, short b);//构造函数,cmd为真则输出上围墙壁
	short getLeft() const { return LEFT; }//LEFT对外接口
	short getRight() const { return RIGHT; }
	short getTop() const { return TOP; }
	short getButtom() const { return BUTTOM; }
private:
	short LEFT;//墙壁最左边坐标
	short RIGHT;
	short TOP;
	short BUTTOM;
};