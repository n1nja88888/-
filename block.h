/*
* 地面完全由坐标定位其位置,因为它是一直静止的
* 对于方块,由于其一直在运动,不能直接知道该方块每个小格的坐标,所以采取一下方案:
*     ■              c
*   ■■■	        1 2 3
* ■■■■■ 对应 b 4 5 6 d      5
*   ■■■          7 8 9
*     ■             a
* 通过序号来定位方块每个小格的位置.其中以1号位为准
* 综上,地面采用set容器储存数据,方块采用map容器储存数据
*/
/*
* 关于每种方块的状态[判定:
* 以每个类声明上面的图案为向上,顺时针旋转,依次对应向左,向下,向右
* 旋转均以该方块中心为旋转中心
* 进行碰撞检测始终以旋转中心为准做判断
*/
#pragma once
#include"coordinate.h"//各种方块类需要用到coordinate储存坐标,和gotoxy()函数改变光标位置
#include"wall.h"//需要获取墙壁的边界坐标
#include<set>
#include<map>
#include<utility>//map容器需要用到pair类型
#include<iostream>//向标准输出设备输出图案
#include<ctime>//引入time()函数
#include<cstdlib>//引入rand(),srand()函数
extern int animationTime;
using namespace std;
class Block 
{
public:
	static void setWall(const Wall& w);
	static void resetGround() { ground.erase(ground.begin(), ground.end()); }
	virtual void print(const Coordinate& coord) const = 0;//在指定位置输出方块图案,全部都显示在方框内,输入的坐标视为方框左上角
	virtual void print() const = 0;//重载print()函数,不需要参数版是按照当前方块坐标输出图案
	virtual void translation(char cmd) = 0;//平移函数
	virtual void rotation() = 0;//旋转函数
	virtual void fall() = 0;//下落函数,将该方块的图案及其坐标都向下移动一格
	virtual bool crash() const= 0;//碰撞函数,判断是否碰撞
	virtual void mix() = 0;//融合函数,将当前方块的坐标融入进ground
	virtual short eraseLine() = 0;//擦除行函数,对当前方块所在行进行是否满行检测,若该行已满则擦除该行图案及其坐标,并将该行以上的图案及其坐标向下移动一格
	virtual bool isOver() const= 0;//判断游戏是否结束
protected:
	static set<Coordinate> ground;//储存地面上方块的坐标
	static Wall wall;//告知该方块墙的边界
	map<char, Coordinate> body;//储存方块坐标
	char state;//储存方块状态,由旋转导致的不同图案看作不同状态
	void erase(const Coordinate& coord) const;//为子类的print()函数写一个公用的擦除函数
	void printLine(const string& pattern = "■") const;
	void printBlankLine() const { printLine("□"); }
};
//■■
//■■
class Square :public Block 
{//公有继承抽象类Block
public:
	Square(bool cmd);//构造函数
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation(){}//Square类发生旋转不做变换
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "■") const;//输出方块函数
	void eraseBlock() const { printBlock("□"); };//擦除方块函数,将原来方块的图案擦除
};
//■
//■
//■
//■
class Stick :public Block
{//公有继承抽象类Block
public:
	Stick(bool cmd);//构造函数
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation();
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "■") const;//输出方块函数
	void eraseBlock() const { printBlock("□"); };//擦除方块函数,将原来方块的图案擦除
};
//■■
//  ■
//  ■
class LeftCane :public Block
{//公有继承抽象类Block
public:
	LeftCane(bool cmd);//构造函数
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation();
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "■") const;//输出方块函数
	void eraseBlock() const { printBlock("□"); };//擦除方块函数,将原来方块的图案擦除
};
//■■
//■
//■
class RightCane :public Block 
{//公有继承抽象类Block
public:
	RightCane(bool cmd);//构造函数
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation();
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "■") const;//输出方块函数
	void eraseBlock() const { printBlock("□"); }//擦除方块函数,将原来方块的图案擦除
};
//■
//■■
//  ■
class LeftLightning :public Block 
{//公有继承抽象类Block
public:
	LeftLightning(bool cmd);//构造函数
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation();
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "■") const;//输出方块函数
	void eraseBlock() const { printBlock("□"); }//擦除方块函数,将原来方块的图案擦除
};
//  ■
//■■
//■
class RightLightning :public Block 
{//公有继承抽象类Block
public:
	RightLightning(bool cmd);//构造函数
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation();
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "■") const;//输出方块函数
	void eraseBlock() const { printBlock("□"); }//擦除方块函数,将原来方块的图案擦除
};
//  ■
//■■■
class Hill :public Block
{//公有继承抽象类Block
public:
	Hill(bool cmd);//构造函数
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation();
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "■") const;//输出方块函数
	void eraseBlock() const { printBlock("□"); }//擦除方块函数,将原来方块的图案擦除
};