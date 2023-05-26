/*
* ������ȫ�����궨λ��λ��,��Ϊ����һֱ��ֹ��
* ���ڷ���,������һֱ���˶�,����ֱ��֪���÷���ÿ��С�������,���Բ�ȡһ�·���:
*     ��              c
*   ������	        1 2 3
* ���������� ��Ӧ b 4 5 6 d      5
*   ������          7 8 9
*     ��             a
* ͨ���������λ����ÿ��С���λ��.������1��λΪ׼
* ����,�������set������������,�������map������������
*/
/*
* ����ÿ�ַ����״̬[�ж�:
* ��ÿ�������������ͼ��Ϊ����,˳ʱ����ת,���ζ�Ӧ����,����,����
* ��ת���Ը÷�������Ϊ��ת����
* ������ײ���ʼ������ת����Ϊ׼���ж�
*/
#pragma once
#include"coordinate.h"//���ַ�������Ҫ�õ�coordinate��������,��gotoxy()�����ı���λ��
#include"wall.h"//��Ҫ��ȡǽ�ڵı߽�����
#include<set>
#include<map>
#include<utility>//map������Ҫ�õ�pair����
#include<iostream>//���׼����豸���ͼ��
#include<ctime>//����time()����
#include<cstdlib>//����rand(),srand()����
extern int animationTime;
using namespace std;
class Block 
{
public:
	static void setWall(const Wall& w);
	static void resetGround() { ground.erase(ground.begin(), ground.end()); }
	virtual void print(const Coordinate& coord) const = 0;//��ָ��λ���������ͼ��,ȫ������ʾ�ڷ�����,�����������Ϊ�������Ͻ�
	virtual void print() const = 0;//����print()����,����Ҫ�������ǰ��յ�ǰ�����������ͼ��
	virtual void translation(char cmd) = 0;//ƽ�ƺ���
	virtual void rotation() = 0;//��ת����
	virtual void fall() = 0;//���亯��,���÷����ͼ���������궼�����ƶ�һ��
	virtual bool crash() const= 0;//��ײ����,�ж��Ƿ���ײ
	virtual void mix() = 0;//�ںϺ���,����ǰ��������������ground
	virtual short eraseLine() = 0;//�����к���,�Ե�ǰ���������н����Ƿ����м��,�������������������ͼ����������,�����������ϵ�ͼ���������������ƶ�һ��
	virtual bool isOver() const= 0;//�ж���Ϸ�Ƿ����
protected:
	static set<Coordinate> ground;//��������Ϸ��������
	static Wall wall;//��֪�÷���ǽ�ı߽�
	map<char, Coordinate> body;//���淽������
	char state;//���淽��״̬,����ת���µĲ�ͬͼ��������ͬ״̬
	void erase(const Coordinate& coord) const;//Ϊ�����print()����дһ�����õĲ�������
	void printLine(const string& pattern = "��") const;
	void printBlankLine() const { printLine("��"); }
};
//����
//����
class Square :public Block 
{//���м̳г�����Block
public:
	Square(bool cmd);//���캯��
	void print(const Coordinate& coord) const;
	void print() const { printBlock(); }
	virtual void translation(char cmd);
	virtual void rotation(){}//Square�෢����ת�����任
	virtual void fall();
	virtual bool crash() const;
	virtual void mix();
	virtual short eraseLine();
	virtual bool isOver() const;
private:
	void printBlock(const string& pattern = "��") const;//������麯��
	void eraseBlock() const { printBlock("��"); };//�������麯��,��ԭ�������ͼ������
};
//��
//��
//��
//��
class Stick :public Block
{//���м̳г�����Block
public:
	Stick(bool cmd);//���캯��
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
	void printBlock(const string& pattern = "��") const;//������麯��
	void eraseBlock() const { printBlock("��"); };//�������麯��,��ԭ�������ͼ������
};
//����
//  ��
//  ��
class LeftCane :public Block
{//���м̳г�����Block
public:
	LeftCane(bool cmd);//���캯��
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
	void printBlock(const string& pattern = "��") const;//������麯��
	void eraseBlock() const { printBlock("��"); };//�������麯��,��ԭ�������ͼ������
};
//����
//��
//��
class RightCane :public Block 
{//���м̳г�����Block
public:
	RightCane(bool cmd);//���캯��
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
	void printBlock(const string& pattern = "��") const;//������麯��
	void eraseBlock() const { printBlock("��"); }//�������麯��,��ԭ�������ͼ������
};
//��
//����
//  ��
class LeftLightning :public Block 
{//���м̳г�����Block
public:
	LeftLightning(bool cmd);//���캯��
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
	void printBlock(const string& pattern = "��") const;//������麯��
	void eraseBlock() const { printBlock("��"); }//�������麯��,��ԭ�������ͼ������
};
//  ��
//����
//��
class RightLightning :public Block 
{//���м̳г�����Block
public:
	RightLightning(bool cmd);//���캯��
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
	void printBlock(const string& pattern = "��") const;//������麯��
	void eraseBlock() const { printBlock("��"); }//�������麯��,��ԭ�������ͼ������
};
//  ��
//������
class Hill :public Block
{//���м̳г�����Block
public:
	Hill(bool cmd);//���캯��
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
	void printBlock(const string& pattern = "��") const;//������麯��
	void eraseBlock() const { printBlock("��"); }//�������麯��,��ԭ�������ͼ������
};