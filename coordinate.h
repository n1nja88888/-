#pragma once
#include<Windows.h>//引入COORD类,设置光标位置函数
#include<utility>//通过<和==重载其他运算符
struct Coordinate 
{//二维坐标结构体,存储坐标
	short x;//x坐标
	short y;//y坐标
	Coordinate(short x, short y) :x(x), y(y) {}//构造函数
	bool operator <(const Coordinate& coord)const;//重载<运算符
};
void gotoxy(short x, short y);//设置光标位置
