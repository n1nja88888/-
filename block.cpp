//ע:0��ASCII���ж�Ӧ��ʮ������48
#include"block.h"
template<class functor>
void for_each_backword(set<Coordinate>::iterator last, set<Coordinate>::iterator first, functor fn)//ΪeraseLine����дһ���㷨,�������������Ԫ��
{
	while (last != first) {
		fn(*last);
		--last;
	}
}
struct yPlus 
{
	//�Դ�VS2010��ʼ��set��iterator�����Զ�����const���������ͣ���˵�set�������������ʱ����iterator�������޷�������ķ�const��Ա��
	void operator()(const Coordinate& coord) { const_cast<Coordinate&>(coord).y++; }
};
struct printNew 
{
	void operator()(const Coordinate& coord) const
	{
		gotoxy(coord.x, coord.y - 1);
		cout << "��";
		gotoxy(coord.x, coord.y);
		cout << "��";
	}
};
void arrPlus(short arr[4], short size)
{
	for (short i = 0; i < size; i++)
	{
		if (arr[i])
		{
			arr[i]++;
		}
	}
}
//Block����
set<Coordinate> Block::ground;
Wall Block::wall;
void Block::setWall(const Wall& w)
{
	wall = w;
}
void Block::erase(const Coordinate& coord) const
{
	gotoxy(coord.x, coord.y);
	cout << "��������";
	gotoxy(coord.x, coord.y + 1);
	cout << "��������";
}
void Block::printLine(const string& pattern) const
{
	for (short i = 0; i < (wall.getRight() - wall.getLeft() - 3) / 2; i++)
	{
		cout << pattern;
	}
}
//����
//����
Square::Square(bool cmd)
{
	state = 'u';//��ʼ��state
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 1;
	short y = wall.getTop() - 1;
	body.insert(make_pair('1', Coordinate(x, y)));//�������Ͻǵķ����
	//��������,���ϵ���,���β���
	body.insert(make_pair('2', Coordinate(x + 2, y)));
	body.insert(make_pair('4', Coordinate(x, y + 1)));
	body.insert(make_pair('5', Coordinate(x + 2, y + 1)));
	if (cmd)
	{
		printBlock();//Ϊ�������ͼ��
	}
}
void Square::print(const Coordinate& coord) const 
{
erase(coord);//�Ȳ���֮ǰ�ķ���
gotoxy(coord.x + 2, coord.y);
cout << "����";
gotoxy(coord.x + 2, coord.y + 1);
cout << "����";
}
void Square::translation(char cmd)
{
	switch (cmd) {
	case 'a':
		//����if���ֱ��������������ж�,һ���Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ�Ҫ��ǽ,һ�����Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ���ground�赲
		if (body.find('1')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('1')->second.x - 2, body.find('1')->second.y)) != ground.end() || ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end()))
		{
			break;
		}
		eraseBlock();
		body.find('1')->second.x -= 2;
		body.find('2')->second.x -= 2;
		body.find('4')->second.x -= 2;
		body.find('5')->second.x -= 2;
		printBlock();
		break;
	case 'd':
		if (body.find('2')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()))
		{
			break;
		}
		eraseBlock();
		body.find('1')->second.x += 2;
		body.find('2')->second.x += 2;
		body.find('4')->second.x += 2;
		body.find('5')->second.x += 2;
		printBlock();
		break;
	case ' ':
		eraseBlock();
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
		//while�ֱ����������,�����Ƿ�����ground�赲,�����Ƿ��䵽��
		while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('4')->second.y + 1 != wall.getButtom())
		{
			body.find('1')->second.y += 1;
			body.find('2')->second.y += 1;
			body.find('4')->second.y += 1;
			body.find('5')->second.y += 1;
			iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
		}
		printBlock();
		break;
	}
}
void Square::fall()
{
	eraseBlock();
	body.find('1')->second.y += 1;
	body.find('2')->second.y += 1;
	body.find('4')->second.y += 1;
	body.find('5')->second.y += 1;
	printBlock();
}
bool Square::crash() const
{
	//��������ײ����y+i�ﵽ����ͽ������������
	if ((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || body.find('4')->second.y + 1 == wall.getButtom())
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Square::mix()
{
	ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));//��'1'��λ,��iter1,����ground
	ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//'2'��λ
	ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//'4'��λ
	ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//'5'��λ
}
short Square::eraseLine()
{
	short line = 0;//������������
	//�������y����
	short arr[] = { 0,0 };//�������濪ʼ��
	//�������ϼ��
	short xL = wall.getLeft() + 2;//����һ�������x����
	short xR = wall.getRight() - 3;//����һ�����ұ�x����
	short y = body.find('4')->second.y;
	//ͳ�����е��е�y����
	for (short i = 0; i < 2; i++)
	{
		short num = 1;//��������м�������
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//ָ����е�һ������
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//ָ��������һ������	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//�������û�е�һ�������һ������,��ֱ�ӽ����´�ѭ��
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//����ӵ���ʱ�������ϾͲ���ı�iter1��ֵ
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//�ж��Ƿ�����
		{
			line++;
			arr[i] = y - i;
		}
	}
	//��ʼ����
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//ִ�����鶯��
		{
			Sleep(animationTime);
			for (short j = 0; j < 2; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < 2; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//ɾ������Ԫ�صȲ���
		for (short i = 0; i < 2; i++)//��������
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp!=ground.end())//��ֹɾ������Ԫ������û��Ԫ��,�Ӷ�ground.begin()ָ��������Ԫ������
				{
					for_each_backword(temp, --ground.begin(), yPlus());
					arrPlus(arr, 2);
					for_each_backword(temp, --ground.begin(), printNew());
				}
			}
		}
	}
	return line;
}
bool Square::isOver() const
{
	return body.find('4')->second.y == wall.getTop();
}
void Square::printBlock(const string& pattern) const
{
	if (body.find('1')->second.y > wall.getTop())
	{
		gotoxy(body.find('1')->second.x, body.find('1')->second.y);//������Ƶ�1��λ
		cout << pattern << pattern;//����λ�����ͼ��
	}
	if (body.find('4')->second.y > wall.getTop())
	{
		gotoxy(body.find('4')->second.x, body.find('4')->second.y);//4��λ
		cout << pattern << pattern;
	}
	
}
//��
//��
//��
//��
Stick::Stick(bool cmd)
{
	state = 'l';//��ʼ��state
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//���г���
	short y = wall.getTop();
	body.insert(make_pair('b', Coordinate(x, y)));
	body.insert(make_pair('4', Coordinate(x + 2, y)));
	body.insert(make_pair('5', Coordinate(x + 4, y)));
	body.insert(make_pair('6', Coordinate(x + 6, y)));
	if (cmd)
	{
		printBlock();
	}
}
void Stick::print(const Coordinate& coord) const
{
	erase(coord);//�Ȳ���֮ǰ�ķ���
	gotoxy(coord.x, coord.y + 1);
	cout << "��������";
}
void Stick::translation(char cmd) 
{
	switch (cmd) 
	{
	case 'a':
		switch (state)//�ٶԷ�������״̬���з�������
		{
		case 'u':
			//����if���ֱ��������������ж�,һ���Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ�Ҫ��ǽ,һ�����Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ���ground�赲
			if (body.find('2')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end() || ground.find(Coordinate(body.find('a')->second.x - 2, body.find('a')->second.y)) != ground.end())))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			body.find('a')->second.x -= 2;
			printBlock();
			break;
		case 'l':
			if (body.find('b')->second.x == wall.getLeft() + 2 || ground.find(Coordinate(body.find('b')->second.x - 2, body.find('b')->second.y)) != ground.end())
			{
				break;
			}
			eraseBlock();
			body.find('b')->second.x -= 2;
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			printBlock();
			break;
		case 'd':
			if (body.find('c')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('c')->second.x - 2, body.find('c')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end())))
			{
				break;
			}
			eraseBlock();
			body.find('c')->second.x -= 2;
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			printBlock();
			break;
		case 'r':
			if (body.find('4')->second.x == wall.getLeft() + 2 || ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end())
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('d')->second.x -= 2;
			printBlock();
			break;
		}
		break;
	case'd':
		switch (state)
		{
		case 'u':
			if (body.find('2')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end() || ground.find(Coordinate(body.find('a')->second.x + 2, body.find('a')->second.y)) != ground.end())))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			body.find('a')->second.x += 2;
			printBlock();
			break;
		case 'l':
			if (body.find('6')->second.x == wall.getRight() - 3 || ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end())
			{
				break;
			}
			eraseBlock();
			body.find('b')->second.x += 2;
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			printBlock();
			break;
		case 'd':
			if (body.find('c')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('c')->second.x + 2, body.find('c')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end())))
			{
				break;
			}
			eraseBlock();
			body.find('c')->second.x += 2;
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			printBlock();
			break;
		case 'r':
			if (body.find('d')->second.x == wall.getRight() - 3 || ground.find(Coordinate(body.find('d')->second.x + 2, body.find('d')->second.y)) != ground.end())
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('d')->second.x += 2;
			printBlock();
			break;
		}
		break;
	case ' ':
		switch (state) 
		{
		case 'u':
		{
			eraseBlock();
			set<Coordinate>::iterator iter = ground.find(Coordinate(body.find('a')->second.x, body.find('a')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while (iter == ground.end() && body.find('a')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				body.find('a')->second.y += 1;
				iter = ground.find(Coordinate(body.find('a')->second.x, body.find('a')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'l':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('b')->second.x, body.find('b')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter4 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && (iter3 == ground.end() && iter4 == ground.end())) && body.find('b')->second.y + 1 != wall.getButtom())
			{
				body.find('b')->second.y += 1;
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('b')->second.x, body.find('b')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter4 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'd':
		{
			eraseBlock();
			set<Coordinate>::iterator iter = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			while (iter == ground.end() && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('c')->second.y += 1;
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				iter = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'r':
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			set<Coordinate>::iterator iter4 = ground.find(Coordinate(body.find('d')->second.x, body.find('d')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && (iter3 == ground.end() && iter4 == ground.end())) && body.find('4')->second.y + 1 != wall.getButtom())
			{
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('d')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
				iter4 = ground.find(Coordinate(body.find('d')->second.x, body.find('d')->second.y + 1));
			}
			printBlock();
			break;
		}
	}
}
void Stick::rotation()
{
	//���ڵ�ǰ����״̬���з��������
	switch (state)
	{
	case 'u':
		//��5��λΪ׼,��תǰ�Ƚ�����ײ���.�ȼ����ת���λ���Ƿ��Ѵ��ڷ���,�ټ����ת;���Ƿ���ڷ���
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 4, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end())))
		{
			//�ٶ���ת�Ƿ�ײǽ�����ж�
			if (body.find('5')->second.x - 4 > wall.getLeft() && body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();//������ԭ��״̬�ķ���,�ٸı䷽���״̬
				state = 'l';//�ı䷽��״̬
				//����5��λԪ�ص�x,yֵ,������Ԫ��
				body.insert(make_pair('b', Coordinate(body.find('5')->second.x - 4, body.find('5')->second.y)));
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.erase('2');//��2��λԪ��ɾ��
				body.erase('8');//8��λ
				body.erase('a');//a��λ
				printBlock();
			}
		}
		break;
	case 'l':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 2)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end())))
		{
			if (body.find('5')->second.y + 1 < wall.getButtom())
			{
				eraseBlock();
				state = 'd';
				body.insert(make_pair('c', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 2)));
				body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
				body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
				body.erase('b');
				body.erase('4');
				body.erase('6');
				printBlock();
			}
		}
		break;
	case 'd':
		if ((ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 4, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end())))
		{
			if (body.find('5')->second.x - 2 > wall.getLeft() && body.find('5')->second.x + 4 < wall.getRight() - 1)
			{
				eraseBlock();
				state = 'r';
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('d', Coordinate(body.find('5')->second.x + 4, body.find('5')->second.y)));
				body.erase('c');
				body.erase('2');
				body.erase('8');
				printBlock();
			}
		}
		break;
	case 'r':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 2)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end())))
		{
			if (body.find('5')->second.y + 2 < wall.getButtom())
			{
				eraseBlock();
				state = 'u';
				body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
				body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
				body.insert(make_pair('a', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 2)));
				body.erase('4');
				body.erase('6');
				body.erase('d');
				printBlock();
			}
		}
		break;
	}
}
void Stick::fall() 
{
	switch (state)
	{
	case 'u':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		body.find('a')->second.y += 1;
		printBlock();
		break;
	case 'l':
		eraseBlock();
		body.find('b')->second.y += 1;
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		printBlock();
		break;
	case 'd':
		eraseBlock();
		body.find('c')->second.y += 1;
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		printBlock();
		break;
	case 'r':
		eraseBlock();
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('d')->second.y += 1;
		printBlock();
		break;
	}
}
bool Stick::crash() const
{
	switch (state)
	{
	case 'u':
		//�ֱ��ж��Ƿ���ground�ڵķ�����ײ����ײ��ǽ��
		if (ground.find(Coordinate(body.find('a')->second.x, body.find('a')->second.y + 1)) != ground.end() || body.find('a')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'l':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('b')->second.x, body.find('b')->second.y + 1)) != ground.end())) || body.find('b')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'd':
		if (ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'r':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('d')->second.x, body.find('d')->second.y + 1)) != ground.end())) || body.find('d')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
void Stick::mix()
{
	switch (state)
	{
	case 'u':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//��2��λ,����ground
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8��λ
		ground.insert(Coordinate(body.find('a')->second.x, body.find('a')->second.y));//a��λ
		break;
	case 'l':
		ground.insert(Coordinate(body.find('b')->second.x, body.find('b')->second.y));//��b��λ,����ground
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//4��λ
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6��λ
		break;
	case 'd':
		ground.insert(Coordinate(body.find('c')->second.x, body.find('c')->second.y));//��c��λ,����ground
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2��λ
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8��λ
		break;
	case 'r':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//��4��λ,����ground
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6��λ
		ground.insert(Coordinate(body.find('d')->second.x, body.find('d')->second.y));//d��λ
		break;
	}
}
short Stick::eraseLine()
{
	short line = 0;//������������
	//�������ϼ��
	short xL = wall.getLeft() + 2;//����һ�������x����
	short xR = wall.getRight() - 3;//����һ�����ұ�x����
	//����֮��ѭ��ֵ�͸��������y����
	//�������y����
	short arr[] = { 0,0,0,0 };//�������濪ʼ��
	short round = 0;//Ҳ�൱������ʵ�ʵĳ���
	short y = 0;
	switch (state)
	{
	case 'l':
	case 'r':
		round = 1;
		y = body.find('5')->second.y;
		break;
	case 'u':
		round = 4;
		y = body.find('a')->second.y;
		break;
	case 'd':
		round = 4;
		y = body.find('8')->second.y;
		break;
	}
	//ͳ�����е��е�y����
	for (short i = 0; i < round; i++)//ѭ�������鸳ֵ
	{
		short num = 1;//��������м�������
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//ָ����е�һ������
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//ָ��������һ������	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//�������û�е�һ�������һ������,��ֱ�ӽ����´�ѭ��
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//����ӵ���ʱ�������ϾͲ���ı�iter1��ֵ
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//�ж��Ƿ�����
		{
			line++;
			arr[i] = y - i;
		}
	}
	//��ʼ����
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//ִ�����鶯��
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//ɾ������Ԫ�صȲ���
		for (short i = 0; i < round; i++)//��������
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//��ֹɾ������Ԫ������û��Ԫ��,�Ӷ�ground.begin()ָ��������Ԫ������
				{
					for_each_backword(temp, --ground.begin(), yPlus());
					arrPlus(arr, round);
					for_each_backword(temp, --ground.begin(), printNew());
				}
			}
		}
	}
	return line;
}
bool Stick::isOver() const
{
	switch (state)
	{
	case 'u':
		return body.find('a')->second.y == wall.getTop();
	case 'l':
		return body.find('b')->second.y == wall.getTop();
	case 'd':
		return body.find('8')->second.y == wall.getTop();
	case 'r':
		return body.find('d')->second.y == wall.getTop();
	}
}
void Stick::printBlock(const string& pattern) const
{
	switch (state) 
	{
	case 'u':
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);//������Ƶ�2��λ
			cout << pattern;//����λ�����ͼ��
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5��λ
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//8��λ
			cout << pattern;
		}
		if (body.find('a')->second.y > wall.getTop())
		{
			gotoxy(body.find('a')->second.x, body.find('a')->second.y);//a��λ
			cout << pattern;
		}
		break;
	case 'l':
		if (body.find('b')->second.y > wall.getTop())
		{
			gotoxy(body.find('b')->second.x, body.find('b')->second.y);
			cout << pattern << pattern << pattern << pattern;
		}
		break;
	case 'd':
		if (body.find('c')->second.y > wall.getTop())
		{
			gotoxy(body.find('c')->second.x, body.find('c')->second.y);
			cout << pattern;
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern;
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);
			cout << pattern;
		}
		break;
	case 'r':
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern << pattern << pattern;
		}
		break;
	}
}
//����
//  ��
//  ��
LeftCane::LeftCane(bool cmd)
{
	state = 'l';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//���г���
	short y = wall.getTop();
	body.insert(make_pair('4', Coordinate(x, y)));
	body.insert(make_pair('5', Coordinate(x + 2, y)));
	body.insert(make_pair('6', Coordinate(x + 4, y)));
	body.insert(make_pair('3', Coordinate(x + 4, y - 1)));
	if (cmd)
	{
		printBlock();
	}
}
void LeftCane::print(const Coordinate& coord) const
{
	erase(coord);//�Ȳ���֮ǰ�ķ���
	gotoxy(coord.x + 4, coord.y);
	cout << "��";
	gotoxy(coord.x, coord.y + 1);
	cout << "������";
}
void LeftCane::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//����if���ֱ��������������ж�,һ���Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ�Ҫ��ǽ,һ�����Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ���ground�赲
			if (body.find('1')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('1')->second.x - 2, body.find('1')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('1')->second.x -= 2;
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			printBlock();
			break;
		case 'l':
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('3')->second.x - 2, body.find('3')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('3')->second.x -= 2;
			printBlock();
			break;
		case 'd':
			if (body.find('2')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			body.find('9')->second.x -= 2;
			printBlock();
			break;
		case 'r':
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('7')->second.x - 2, body.find('7')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('7')->second.x -= 2;
			printBlock();
			break;
		}
		break;
	case 'd':
		switch (state)
		{
		case 'u':
			if (body.find('2')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('1')->second.x + 2, body.find('1')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('1')->second.x += 2;
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			printBlock();
			break;
		case 'l':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('3')->second.x + 2, body.find('3')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('3')->second.x += 2;
			printBlock();
			break;
		case 'd':
			if (body.find('9')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('9')->second.x + 2, body.find('9')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			body.find('9')->second.x += 2;
			printBlock();
			break;
		case 'r':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('7')->second.x + 2, body.find('7')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('7')->second.x += 2;
			printBlock();
			break;
		}
		break;
	case ' ':
		switch (state)
		{
		case 'u':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('1')->second.x, body.find('1')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('1')->second.y += 1;
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('1')->second.x, body.find('1')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'l':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('4')->second.y + 1 != wall.getButtom())
			{
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('3')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'd':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				body.find('9')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'r':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('5')->second.x, body.find('6')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('7')->second.y + 1 != wall.getButtom())
			{
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('7')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
			}
			printBlock();
			break;
		}
		}
		break;
	}
}
void LeftCane::rotation()
{
	//���ڵ�ǰ����״̬���з��������
	switch (state)
	{
	case 'u':
		//��5��λΪ׼,��תǰ�Ƚ�����ײ���,�ȼ����ת���λ���Ƿ��Ѵ��ڷ���,�ټ����ת;���Ƿ���ڷ���
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end()&& ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end()))
		{
			//�ٶ���ת�Ƿ�ײǽ�����ж�
			if (body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();//������ԭ��״̬�ķ���,�ٸı䷽���״̬
				state = 'l';//�ı䷽��״̬
				//����5��λԪ�ص�x,yֵ,������Ԫ��
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('3', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)));
				body.erase('1');//��1��λԪ��ɾ��
				body.erase('2');//2��λ
				body.erase('8');//8��λ
				printBlock();
			}
		}
		break;
	case 'l':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end()))
		{
			if (body.find('5')->second.y + 1 < wall.getButtom())
			{
				eraseBlock();
				state = 'd';
				body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
				body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
				body.insert(make_pair('9', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)));
				body.erase('4');
				body.erase('6');
				body.erase('3');
				printBlock();
			}
		}
		break;
	case 'd':
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end()))
		{
			if (body.find('5')->second.x - 2 > wall.getLeft())
			{
				eraseBlock();
				state = 'r';
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('7', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)));
				body.erase('2');
				body.erase('8');
				body.erase('9');
				printBlock();
			}
		}
		break;
	case 'r':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end()))
		{
			//��ͼ��֪,���鴦��l״̬��d״̬ת���϶�����ײǽ
			eraseBlock();
			state = 'u';
			body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
			body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
			body.insert(make_pair('1', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)));
			body.erase('4');
			body.erase('6');
			body.erase('7');
			printBlock();
		}
		break;
	}
}
void LeftCane::fall()
{
	switch (state)
	{
	case 'u':
		eraseBlock();
		body.find('1')->second.y += 1;
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		printBlock();
		break;
	case 'l':
		eraseBlock();
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('3')->second.y += 1;
		printBlock();
		break;
	case 'd':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		body.find('9')->second.y += 1;
		printBlock();
		break;
	case 'r':
		eraseBlock();
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('7')->second.y += 1;
		printBlock();
		break;
	}
}
bool LeftCane::crash() const
{
	switch (state)
	{
	case 'u':
		//�ֱ��ж��Ƿ���ground�ڵķ�����ײ����ײ��ǽ��
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('1')->second.x, body.find('1')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'l':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end())) || body.find('4')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'd':
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'r':
		if (((ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end())) || body.find('7')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
void LeftCane::mix()
{
	switch (state)
	{
	case 'u':
		ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));//��1��λ,����ground
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2��λ
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8��λ
		break;
	case 'l':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('3')->second.x, body.find('3')->second.y));
		break;
	case 'd':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));
		ground.insert(Coordinate(body.find('9')->second.x, body.find('9')->second.y));
		break;
	case 'r':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('7')->second.x, body.find('7')->second.y));
		break;
	}
}
short LeftCane::eraseLine()
{
	short line = 0;//������������
	//�������ϼ��
	short xL = wall.getLeft() + 2;//����һ�������x����
	short xR = wall.getRight() - 3;//����һ�����ұ�x����
	//����֮��ѭ��ֵ�͸��������y����
	//�������y����
	short arr[] = { 0,0,0 };//�������濪ʼ��
	short round = 0;//Ҳ�൱������ʵ�ʵĳ���
	short y = 0;//���������y����
	switch (state)
	{
	case 'l':
		round = 2;
		y = body.find('4')->second.y;
		break;
	case 'r':
		round = 2;
		y = body.find('7')->second.y;
		break;
	case 'u':
	case 'd':
		round = 3;
		y = body.find('8')->second.y;
		break;
	}
	//ͳ�����е��е�y����
	for (short i = 0; i < round; i++)//ѭ�������鸳ֵ
	{
		short num = 1;//��������м�������
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//ָ����е�һ������
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//ָ��������һ������	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//�������û�е�һ�������һ������,��ֱ�ӽ����´�ѭ��
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//����ӵ���ʱ�������ϾͲ���ı�iter1��ֵ
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//�ж��Ƿ�����
		{
			line++;
			arr[i] = y - i;
		}
	}
	//��ʼ����
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//ִ�����鶯��
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//ɾ������Ԫ�صȲ���
		for (short i = 0; i < round; i++)//��������
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//��ֹɾ������Ԫ������û��Ԫ��,�Ӷ�ground.begin()ָ��������Ԫ������
				{
					for_each_backword(temp, --ground.begin(), yPlus());
					arrPlus(arr, round);
					for_each_backword(temp, --ground.begin(), printNew());
				}
			}
		}
	}
	return line;
}
bool LeftCane::isOver() const
{
	switch (state)
	{
	case 'u':
	case 'd':
		return body.find('8')->second.y == wall.getTop();
	case 'l':
		return body.find('4')->second.y == wall.getTop();
	case 'r':
		return body.find('7')->second.y == wall.getTop();
	}
}
void LeftCane::printBlock(const string& pattern) const
{
	switch (state)
	{
	case 'u':
		if (body.find('1')->second.y > wall.getTop())
		{
			gotoxy(body.find('1')->second.x, body.find('1')->second.y);//������Ƶ�2��λ
			cout << pattern << pattern;//����λ�����ͼ��
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5��λ
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//8��λ
			cout << pattern;
		}
		break;
	case 'l':
		if (body.find('3')->second.y > wall.getTop())
		{
			gotoxy(body.find('3')->second.x, body.find('3')->second.y);
			cout << pattern;
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern << pattern;
		}
		break;
	case 'd':
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern;
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);
			cout << pattern << pattern;
		}
		break;
	case 'r':
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern << pattern;
		}
		if (body.find('7')->second.y > wall.getTop())
		{
			gotoxy(body.find('7')->second.x, body.find('7')->second.y);
			cout << pattern;
		}
		break;
	}
}
//����
//��
//��
RightCane::RightCane(bool cmd)
{
	state = 'r';// Ϊ�˺�LeftCane�Գ�,�����ʼ��״̬Ϊr
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//���г���
	short y = wall.getTop();
	body.insert(make_pair('4', Coordinate(x, y)));
	body.insert(make_pair('5', Coordinate(x + 2, y)));
	body.insert(make_pair('6', Coordinate(x + 4, y)));
	body.insert(make_pair('1', Coordinate(x, y - 1)));
	if (cmd)
	{
		printBlock();
	}
}
void RightCane::print(const Coordinate& coord) const
{
	erase(coord);//�Ȳ���֮ǰ�ķ���
	gotoxy(coord.x, coord.y);
	cout << "��";
	gotoxy(coord.x, coord.y + 1);
	cout << "������";
}
void RightCane::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//����if���ֱ��������������ж�,һ���Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ�Ҫ��ǽ,һ�����Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ���ground�赲
			if (body.find('2')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('3')->second.x -= 2;
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			printBlock();
			break;
		case 'l':
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('9')->second.x - 2, body.find('9')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('9')->second.x -= 2;
			printBlock();
			break;
		case 'd':
			if (body.find('7')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('7')->second.x - 2, body.find('7')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			body.find('7')->second.x -= 2;
			printBlock();
			break;
		case 'r':
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('1')->second.x - 2, body.find('1')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('1')->second.x -= 2;
			printBlock();
			break;
		}
		break;
	case 'd':
		switch (state)
		{
		case 'u':
			if (body.find('3')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('3')->second.x + 2, body.find('3')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('3')->second.x += 2;
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			printBlock();
			break;
		case 'l':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('9')->second.x + 2, body.find('9')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('9')->second.x += 2;
			printBlock();
			break;
		case 'd':
			if (body.find('8')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			body.find('7')->second.x += 2;
			printBlock();
			break;
		case 'r':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('1')->second.x + 2, body.find('1')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('1')->second.x += 2;
			printBlock();
			break;
		}
		break;
	case ' ':
		switch (state)
		{
		case 'u':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('3')->second.x, body.find('3')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('3')->second.y += 1;
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('3')->second.x, body.find('3')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'l':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('9')->second.y + 1 != wall.getButtom())
			{
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('9')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'd':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				body.find('7')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'r':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('4')->second.y + 1 != wall.getButtom())
			{
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('1')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			}
			printBlock();
			break;
		}
		}
		break;
	}
}
void RightCane::rotation()
{
	//���ڵ�ǰ����״̬���з��������
	switch (state)
	{
	case 'u':
		//��5��λΪ׼,��תǰ�Ƚ�����ײ���,�ȼ����ת���λ���Ƿ��Ѵ��ڷ���,�ټ����ת;���Ƿ���ڷ���
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end()))
		{
			//�ٶ���ת�Ƿ�ײǽ�����ж�
			if (body.find('5')->second.x - 2 > wall.getLeft())
			{
				eraseBlock();//������ԭ��״̬�ķ���,�ٸı䷽���״̬
				state = 'l';//�ı䷽��״̬
				//����5��λԪ�ص�x,yֵ,������Ԫ��
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('9', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)));
				body.erase('3');//��3��λԪ��ɾ��
				body.erase('2');//2��λ
				body.erase('8');//8��λ
				printBlock();
			}
		}
		break;
	case 'l':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end()))
		{
			//��ͼ��֪,���鴦��l״̬��d״̬ת���϶�����ײǽ
			eraseBlock();
			state = 'd';
			body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
			body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
			body.insert(make_pair('7', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)));
			body.erase('4');
			body.erase('6');
			body.erase('9');
			printBlock();
		}
		break;
	case 'd':
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end()))
		{
			if (body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();
				state = 'r';
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('1', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)));
				body.erase('2');
				body.erase('8');
				body.erase('7');
				printBlock();
			}
		}
		break;
	case 'r':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end()))
		{
			if (body.find('5')->second.y + 1 < wall.getButtom())
			{
				eraseBlock();
				state = 'u';
				body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
				body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
				body.insert(make_pair('3', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)));
				body.erase('4');
				body.erase('6');
				body.erase('1');
				printBlock();
			}
		}
		break;
	}
}
void RightCane::fall()
{
	switch (state)
	{
	case 'u':
		eraseBlock();
		body.find('3')->second.y += 1;
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		printBlock();
		break;
	case 'l':
		eraseBlock();
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('9')->second.y += 1;
		printBlock();
		break;
	case 'd':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		body.find('7')->second.y += 1;
		printBlock();
		break;
	case 'r':
		eraseBlock();
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('1')->second.y += 1;
		printBlock();
		break;
	}
}
bool RightCane::crash() const
{
	switch (state)
	{
	case 'u':
		//�ֱ��ж��Ƿ���ground�ڵķ�����ײ����ײ��ǽ��
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('3')->second.x, body.find('3')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'l':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1)) != ground.end())) || body.find('9')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'd':
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'r':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end())) || body.find('4')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
void RightCane::mix()
{
	switch (state)
	{
	case 'u':
		ground.insert(Coordinate(body.find('3')->second.x, body.find('3')->second.y));//��3��λ,����ground
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2��λ
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8��λ
		break;
	case 'l':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('9')->second.x, body.find('9')->second.y));
		break;
	case 'd':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));
		ground.insert(Coordinate(body.find('7')->second.x, body.find('7')->second.y));
		break;
	case 'r':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));
		break;
	}
}
short RightCane::eraseLine()
{
	short line = 0;//������������
	//�������ϼ��
	short xL = wall.getLeft() + 2;//����һ�������x����
	short xR = wall.getRight() - 3;//����һ�����ұ�x����
	//����֮��ѭ��ֵ�͸��������y����
	//�������y����
	short arr[] = { 0,0,0 };//�������濪ʼ��
	short round = 0;//Ҳ�൱������ʵ�ʵĳ���
	short y = 0;//���������y����
	switch (state)
	{
	case 'l':
		round = 2;
		y = body.find('9')->second.y;
		break;
	case 'r':
		round = 2;
		y = body.find('4')->second.y;
		break;
	case 'u':
	case 'd':
		round = 3;
		y = body.find('8')->second.y;
		break;
	}
	//ͳ�����е��е�y����
	for (short i = 0; i < round; i++)//ѭ�������鸳ֵ
	{
		short num = 1;//��������м�������
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//ָ����е�һ������
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//ָ��������һ������	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//�������û�е�һ�������һ������,��ֱ�ӽ����´�ѭ��
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//����ӵ���ʱ�������ϾͲ���ı�iter1��ֵ
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//�ж��Ƿ�����
		{
			line++;
			arr[i] = y - i;
		}
	}
	//��ʼ����
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//ִ�����鶯��
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//ɾ������Ԫ�صȲ���
		for (short i = 0; i < round; i++)//��������
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//��ֹɾ������Ԫ������û��Ԫ��,�Ӷ�ground.begin()ָ��������Ԫ������
				{
					for_each_backword(temp, --ground.begin(), yPlus());
					arrPlus(arr, round);
					for_each_backword(temp, --ground.begin(), printNew());
				}
			}
		}
	}
	return line;
}
bool RightCane::isOver() const
{
	switch (state)
	{
	case 'u':
	case 'd':
		return body.find('8')->second.y == wall.getTop();
	case 'l':
		return body.find('9')->second.y == wall.getTop();
	case 'r':
		return body.find('4')->second.y == wall.getTop();
	}
}
void RightCane::printBlock(const string& pattern) const
{
	switch (state)
	{
	case 'u':
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);//������Ƶ�2��λ
			cout << pattern << pattern;//����λ�����ͼ��
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5��λ
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//8��λ
			cout << pattern;
		}
		break;
	case 'l':
		if (body.find('9')->second.y > wall.getTop())
		{
			gotoxy(body.find('9')->second.x, body.find('9')->second.y);
			cout << pattern;
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern << pattern;
		}
		break;
	case 'd':
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern;
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);
			cout << pattern;
		}
		if (body.find('7')->second.y > wall.getTop())
		{
			gotoxy(body.find('7')->second.x, body.find('7')->second.y);
			cout << pattern << pattern;
		}
		break;
	case 'r':
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern << pattern;
		}
		if (body.find('1')->second.y > wall.getTop())
		{
			gotoxy(body.find('1')->second.x, body.find('1')->second.y);
			cout << pattern;
		}
		break;
	}
}
//��
//����
//  ��
LeftLightning::LeftLightning(bool cmd)
{
	state = 'l';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//���г���
	short y = wall.getTop();
	body.insert(make_pair('2', Coordinate(x + 2, y - 1)));
	body.insert(make_pair('3', Coordinate(x + 4, y - 1)));
	body.insert(make_pair('4', Coordinate(x, y)));
	body.insert(make_pair('5', Coordinate(x + 2, y)));
	if (cmd)
	{
		printBlock();
	}
}
void LeftLightning::print(const Coordinate& coord) const
{
	erase(coord);//�Ȳ���֮ǰ�ķ���
	gotoxy(coord.x + 2, coord.y);
	cout << "����";
	gotoxy(coord.x, coord.y + 1);
	cout << "����";
}
void LeftLightning::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//����if���ֱ��������������ж�,һ���Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ�Ҫ��ǽ,һ�����Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ���ground�赲
			if (body.find('1')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('1')->second.x - 2, body.find('1')->second.y)) != ground.end() || ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('1')->second.x -= 2;
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			printBlock();
			break;
		case 'l':
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('3')->second.x -= 2;
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			printBlock();
			break;
		case 'd':
			if (body.find('2')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('9')->second.x - 2, body.find('9')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('9')->second.x -= 2;
			printBlock();
			break;
		case 'r':
			if (body.find('7')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('7')->second.x - 2, body.find('7')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('7')->second.x -= 2;
			body.find('8')->second.x -= 2;
			printBlock();
			break;
		}
		break;
	case 'd':
		switch (state)
		{
		case 'u':
			if (body.find('5')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end() || ground.find(Coordinate(body.find('1')->second.x + 2, body.find('1')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('1')->second.x += 2;
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			printBlock();
			break;
		case 'l':
			if (body.find('3')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('3')->second.x + 2, body.find('3')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('3')->second.x += 2;
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			printBlock();
			break;
		case 'd':
			if (body.find('6')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('9')->second.x + 2, body.find('9')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('9')->second.x += 2;
			printBlock();
			break;
		case 'r':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('7')->second.x += 2;
			body.find('8')->second.x += 2;
			printBlock();
			break;
		}
		break;
	case ' ':
		switch (state)
		{
		case 'u':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('1')->second.y += 1;
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'l':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('3')->second.x, body.find('3')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('4')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('3')->second.y += 1;
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('3')->second.x, body.find('3')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'd':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('9')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('9')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'r':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('7')->second.y + 1 != wall.getButtom())
			{
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('7')->second.y += 1;
				body.find('8')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			}
			printBlock();
			break;
		}
		}
		break;
	}
}
void LeftLightning::rotation()
{
	//���ڵ�ǰ����״̬���з��������
	switch (state)
	{
	case 'u':
		//��5��λΪ׼,��תǰ�Ƚ�����ײ���,�ȼ����ת���λ���Ƿ��Ѵ��ڷ���,�ټ����ת;���Ƿ���ڷ���
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end())
		{
			//�ٶ���ת�Ƿ�ײǽ�����ж�
			if (body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();//������ԭ��״̬�ķ���,�ٸı䷽���״̬
				state = 'l';//�ı䷽��״̬
				//����5��λԪ�ص�x,yֵ,������Ԫ��
				body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
				body.insert(make_pair('3', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)));
				body.erase('1');//��1��λԪ��ɾ��
				body.erase('8');//8��λ
				printBlock();
			}
		}
		break;
	case 'l':
		if ((ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end())
		{
			if (body.find('5')->second.y + 1 < wall.getButtom())
			{
				eraseBlock();
				state = 'd';
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('9', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)));
				body.erase('3');
				body.erase('4');
				printBlock();
			}
		}
		break;
	case 'd':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end())
		{
			if (body.find('5')->second.x - 2 > wall.getLeft())
			{
				eraseBlock();
				state = 'r';
				body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
				body.insert(make_pair('7', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)));
				body.erase('2');
				body.erase('9');
				printBlock();
			}
		}
		break;
	case 'r':
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end())
		{
			//��ͼ��֪,���鴦��r״̬��u״̬ת���϶�����ײǽ
			eraseBlock();
			state = 'u';
			body.insert(make_pair('1', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)));
			body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
			body.erase('6');
			body.erase('7');
			printBlock();
		}
		break;
	}
}
void LeftLightning::fall()
{
	switch (state)
	{
	case 'u':
		eraseBlock();
		body.find('1')->second.y += 1;
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		printBlock();
		break;
	case 'l':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('3')->second.y += 1;
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		printBlock();
		break;
	case 'd':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('9')->second.y += 1;
		printBlock();
		break;
	case 'r':
		eraseBlock();
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('7')->second.y += 1;
		body.find('8')->second.y += 1;
		printBlock();
		break;
	}
}
bool LeftLightning::crash() const
{
	switch (state)
	{
	case 'u':
		//�ֱ��ж��Ƿ���ground�ڵķ�����ײ����ײ��ǽ��
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'l':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('3')->second.x, body.find('3')->second.y + 1)) != ground.end())) || body.find('4')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'd':
		if ((ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || body.find('9')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'r':
		if (((ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end())) || body.find('7')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
void LeftLightning::mix()
{
	switch (state)
	{
	case 'u':
		ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));//��1��λ,����ground
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//4��λ
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8��λ
		break;
	case 'l':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		ground.insert(Coordinate(body.find('3')->second.x, body.find('3')->second.y));
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		break;
	case 'd':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('9')->second.x, body.find('9')->second.y));
		break;
	case 'r':
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('7')->second.x, body.find('7')->second.y));
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));
		break;
	}
}
short LeftLightning::eraseLine()
{
	short line = 0;//������������
	//�������ϼ��
	short xL = wall.getLeft() + 2;//����һ�������x����
	short xR = wall.getRight() - 3;//����һ�����ұ�x����
	//����֮��ѭ��ֵ�͸��������y����
	//�������y����
	short arr[] = { 0,0,0 };//�������濪ʼ��
	short round = 0;//Ҳ�൱������ʵ�ʵĳ���
	short y = 0;//���������y����
	switch (state)
	{
	case 'u':
		round = 3;
		y = body.find('8')->second.y;
		break;
	case 'l':
		round = 2;
		y = body.find('4')->second.y;
		break;
	case 'd':
		round = 3;
		y = body.find('9')->second.y;
		break;
	case 'r':
		round = 2;
		y = body.find('7')->second.y;
		break;
	}
	//ͳ�����е��е�y����
	for (short i = 0; i < round; i++)//ѭ�������鸳ֵ
	{
		short num = 1;//��������м�������
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//ָ����е�һ������
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//ָ��������һ������	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//�������û�е�һ�������һ������,��ֱ�ӽ����´�ѭ��
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//����ӵ���ʱ�������ϾͲ���ı�iter1��ֵ
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//�ж��Ƿ�����
		{
			line++;
			arr[i] = y - i;
		}
	}
	//��ʼ����
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//ִ�����鶯��
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//ɾ������Ԫ�صȲ���
		for (short i = 0; i < round; i++)//��������
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//��ֹɾ������Ԫ������û��Ԫ��,�Ӷ�ground.begin()ָ��������Ԫ������
				{
					for_each_backword(temp, --ground.begin(), yPlus());
					arrPlus(arr, round);
					for_each_backword(temp, --ground.begin(), printNew());
				}
			}
		}
	}
	return line;
}
bool LeftLightning::isOver() const
{
	switch (state)
	{
	case 'u':
		return body.find('8')->second.y == wall.getTop();
	case 'l':
		return body.find('4')->second.y == wall.getTop();
	case 'd':
		return body.find('9')->second.y == wall.getTop();
	case 'r':
		return body.find('7')->second.y == wall.getTop();
	}
}
void LeftLightning::printBlock(const string& pattern) const
{
	switch (state)
	{
	case 'u':
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//������Ƶ�8��λ
			cout << pattern;//����λ�����ͼ��
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);//4��λ
			cout << pattern << pattern;
		}
		if (body.find('1')->second.y > wall.getTop())
		{
			gotoxy(body.find('1')->second.x, body.find('1')->second.y);//1��λ
			cout << pattern;
		}
		break;
	case 'l':
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern << pattern;
		}
		break;
	case 'd':
		if (body.find('9')->second.y > wall.getTop())
		{
			gotoxy(body.find('9')->second.x, body.find('9')->second.y);
			cout << pattern;
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern;
		}
		break;
	case 'r':
		if (body.find('7')->second.y > wall.getTop())
		{
			gotoxy(body.find('7')->second.x, body.find('7')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);
			cout << pattern << pattern;
		}
		break;
	}
}
//  ��
//����
//��
//������ʾ,Ϊ����RightCane����һ��
RightLightning::RightLightning(bool cmd)
{
	state = 'l';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//���г���
	short y = wall.getTop();
	body.insert(make_pair('4', Coordinate(x, y - 1)));
	body.insert(make_pair('5', Coordinate(x + 2, y - 1)));
	body.insert(make_pair('8', Coordinate(x + 2, y)));
	body.insert(make_pair('9', Coordinate(x + 4, y)));
	if (cmd)
	{
		printBlock();
	}
}
void RightLightning::print(const Coordinate& coord) const
{
	erase(coord);//�Ȳ���֮ǰ�ķ���
	gotoxy(coord.x, coord.y);
	cout << "����";
	gotoxy(coord.x + 2, coord.y + 1);
	cout << "����";
}
void RightLightning::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//����if���ֱ��������������ж�,һ���Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ�Ҫ��ǽ,һ�����Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ���ground�赲
			if (body.find('5')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end() || ground.find(Coordinate(body.find('3')->second.x - 2, body.find('3')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('3')->second.x -= 2;
			body.find('6')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			printBlock();
			break;
		case 'l':
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('8')->second.x -= 2;
			body.find('9')->second.x -= 2;
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			printBlock();
			break;
		case 'd':
			if (body.find('4')->second.x == wall.getLeft() + 2 || ((ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('7')->second.x - 2, body.find('7')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('4')->second.x -= 2;
			body.find('7')->second.x -= 2;
			printBlock();
			break;
		case 'r':
			if (body.find('1')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('1')->second.x - 2, body.find('1')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('1')->second.x -= 2;
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			printBlock();
			break;
		}
		break;
	case 'd':
		switch (state)
		{
		case 'u':
			if (body.find('3')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('3')->second.x + 2, body.find('3')->second.y)) != ground.end() || ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('3')->second.x += 2;
			body.find('6')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			printBlock();
			break;
		case 'l':
			if (body.find('9')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('9')->second.x + 2, body.find('9')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('8')->second.x += 2;
			body.find('9')->second.x += 2;
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			printBlock();
			break;
		case 'd':
			if (body.find('2')->second.x == wall.getRight() - 3 || ((ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end()) || ground.find(Coordinate(body.find('7')->second.x + 2, body.find('7')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('4')->second.x += 2;
			body.find('7')->second.x += 2;
			printBlock();
			break;
		case 'r':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('1')->second.x += 2;
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			printBlock();
			break;
		}
		break;
	case ' ':
		switch (state)
		{
		case 'u':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('3')->second.y += 1;
				body.find('6')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'l':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('8')->second.y += 1;
				body.find('9')->second.y += 1;
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'd':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('7')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('4')->second.y += 1;
				body.find('7')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'r':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('1')->second.x, body.find('1')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('5')->second.y + 1 != wall.getButtom())
			{
				body.find('1')->second.y += 1;
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('1')->second.x, body.find('1')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			}
			printBlock();
			break;
		}
		}
		break;
	}
}
void RightLightning::rotation()
{
	//���ڵ�ǰ����״̬���з��������
	switch (state)
	{
	case 'u':
		//��5��λΪ׼,��תǰ�Ƚ�����ײ���,�ȼ����ת���λ���Ƿ��Ѵ��ڷ���,�ټ����ת;���Ƿ���ڷ���
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end())
		{
			//�ٶ���ת�Ƿ�ײǽ�����ж�
			if (body.find('5')->second.x - 2 > wall.getLeft())
			{
				eraseBlock();//������ԭ��״̬�ķ���,�ٸı䷽���״̬
				state = 'l';//�ı䷽��״̬
				//����5��λԪ�ص�x,yֵ,������Ԫ��
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('9', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)));
				body.erase('3');//��3��λԪ��ɾ��
				body.erase('6');//8��λ
				printBlock();
			}
		}
		break;
	case 'l':
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end())
		{
			//��ͼ��֪,���鴦��l״̬��d״̬ת���϶�����ײǽ
			eraseBlock();
			state = 'd';
			body.insert(make_pair('7', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)));
			body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
			body.erase('8');
			body.erase('9');
			printBlock();
		}
		break;
	case 'd':
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end())
		{
			if (body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();
				state = 'r';
				body.insert(make_pair('1', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.erase('4');
				body.erase('7');
				printBlock();
			}
		}
		break;
	case 'r':
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end())
		{
			//��ͼ��֪,���鴦��r״̬��u״̬ת���϶�����ײǽ
			eraseBlock();
			state = 'u';
			body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
			body.insert(make_pair('3', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)));
			body.erase('1');
			body.erase('2');
			printBlock();
		}
		break;
	}
}
void RightLightning::fall()
{
	switch (state)
	{
	case 'u':
		eraseBlock();
		body.find('3')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		printBlock();
		break;
	case 'l':
		eraseBlock();
		body.find('8')->second.y += 1;
		body.find('9')->second.y += 1;
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		printBlock();
		break;
	case 'd':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('4')->second.y += 1;
		body.find('7')->second.y += 1;
		printBlock();
		break;
	case 'r':
		eraseBlock();
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		body.find('1')->second.y += 1;
		body.find('2')->second.y += 1;
		printBlock();
		break;
	}
}
bool RightLightning::crash() const 
{
	switch (state)
	{
	case 'u':
		//�ֱ��ж��Ƿ���ground�ڵķ�����ײ����ײ��ǽ��
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'l':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('9')->second.x, body.find('9')->second.y + 1)) != ground.end())) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'd':
		if ((ground.find(Coordinate(body.find('7')->second.x, body.find('7')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || body.find('7')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'r':
		if (((ground.find(Coordinate(body.find('1')->second.x, body.find('1')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) != ground.end()) || (ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end())) || body.find('5')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
void RightLightning::mix()
{
	switch (state)
	{
	case 'u':
		ground.insert(Coordinate(body.find('3')->second.x, body.find('3')->second.y));//��3��λ,����ground
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6��λ
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8��λ
		break;
	case 'l':
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));
		ground.insert(Coordinate(body.find('9')->second.x, body.find('9')->second.y));
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		break;
	case 'd':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('7')->second.x, body.find('7')->second.y));
		break;
	case 'r':
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		break;
	}
}
short RightLightning::eraseLine()
{
	short line = 0;//������������
	//�������ϼ��
	short xL = wall.getLeft() + 2;//����һ�������x����
	short xR = wall.getRight() - 3;//����һ�����ұ�x����
	//����֮��ѭ��ֵ�͸��������y����
	//�������y����
	short arr[] = { 0,0,0 };//�������濪ʼ��
	short round = 0;//Ҳ�൱������ʵ�ʵĳ���
	short y = 0;//���������y����
	switch (state)
	{
	case 'u':
		round = 3;
		y = body.find('8')->second.y;
		break;
	case 'l':
		round = 2;
		y = body.find('8')->second.y;
		break;
	case 'd':
		round = 3;
		y = body.find('7')->second.y;
		break;
	case 'r':
		round = 2;
		y = body.find('5')->second.y;
		break;
	}
	//ͳ�����е��е�y����
	for (short i = 0; i < round; i++)//ѭ�������鸳ֵ
	{
		short num = 1;//��������м�������
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//ָ����е�һ������
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//ָ��������һ������	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//�������û�е�һ�������һ������,��ֱ�ӽ����´�ѭ��
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//����ӵ���ʱ�������ϾͲ���ı�iter1��ֵ
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//�ж��Ƿ�����
		{
			line++;
			arr[i] = y - i;
		}
	}
	//��ʼ����
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//ִ�����鶯��
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//ɾ������Ԫ�صȲ���
		for (short i = 0; i < round; i++)//��������
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//��ֹɾ������Ԫ������û��Ԫ��,�Ӷ�ground.begin()ָ��������Ԫ������
				{
					for_each_backword(temp, --ground.begin(), yPlus());
					arrPlus(arr, round);
					for_each_backword(temp, --ground.begin(), printNew());
				}
			}
		}
	}
	return line;
}
bool RightLightning::isOver() const
{
	switch (state)
	{
	case 'u':
		return body.find('8')->second.y == wall.getTop();
	case 'l':
		return body.find('8')->second.y == wall.getTop();
	case 'd':
		return body.find('7')->second.y == wall.getTop();
	case 'r':
		return body.find('5')->second.y == wall.getTop();
	}
}
void RightLightning::printBlock(const string& pattern) const
{
	switch (state)
	{
	case 'u':
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//������Ƶ�8��λ
			cout << pattern;//����λ�����ͼ��
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5��λ
			cout << pattern << pattern;
		}
		if (body.find('3')->second.y > wall.getTop())
		{
			gotoxy(body.find('3')->second.x, body.find('3')->second.y);//3��λ
			cout << pattern;
		}
		break;
	case 'l':
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern;
		}
		break;
	case 'd':
		if (body.find('7')->second.y > wall.getTop())
		{
			gotoxy(body.find('7')->second.x, body.find('7')->second.y);
			cout << pattern;
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern;
		}
		break;
	case 'r':
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('1')->second.y > wall.getTop())
		{
			gotoxy(body.find('1')->second.x, body.find('1')->second.y);
			cout << pattern << pattern;
		}
		break;
	}
}
//  ��
//������
Hill::Hill(bool cmd)
{
	state = 'u';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//���г���
	short y = wall.getTop();
	body.insert(make_pair('4', Coordinate(x, y)));
	body.insert(make_pair('5', Coordinate(x + 2, y)));
	body.insert(make_pair('6', Coordinate(x + 4, y)));
	body.insert(make_pair('2', Coordinate(x + 2, y - 1)));
	if (cmd)
	{
		printBlock();
	}
}
void Hill::print(const Coordinate& coord) const
{
	erase(coord);//�Ȳ���֮ǰ�ķ���
	gotoxy(coord.x + 2, coord.y);
	cout << "��";
	gotoxy(coord.x, coord.y + 1);
	cout << "������";
}
void Hill::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//����if���ֱ��������������ж�,һ���Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ�Ҫ��ǽ,һ�����Ƕ�Ӧ�ƶ�����һ��ķ����Ƿ���ground�赲
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			printBlock();
			break;
		case 'l':
			if ((body.find('2')->second.x == wall.getLeft() + 2 || ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			body.find('6')->second.x -= 2;
			printBlock();
			break;
		case 'd':
			if (body.find('4')->second.x == wall.getLeft() + 2 || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('8')->second.x -= 2;
			body.find('4')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('6')->second.x -= 2;
			printBlock();
			break;
		case 'r':
			if ((body.find('4')->second.x == wall.getLeft() + 2 || ground.find(Coordinate(body.find('2')->second.x - 2, body.find('2')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('4')->second.x - 2, body.find('4')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x - 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x -= 2;
			body.find('5')->second.x -= 2;
			body.find('8')->second.x -= 2;
			body.find('4')->second.x -= 2;
			printBlock();
			break;
		}
		break;
	case 'd':
		switch (state)
		{
		case 'u':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			printBlock();
			break;
		case 'l':
			if ((body.find('6')->second.x == wall.getRight() - 3 || ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			body.find('6')->second.x += 2;
			printBlock();
			break;
		case 'd':
			if (body.find('6')->second.x == wall.getRight() - 3 || (ground.find(Coordinate(body.find('6')->second.x + 2, body.find('6')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('8')->second.x += 2;
			body.find('4')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('6')->second.x += 2;
			printBlock();
			break;
		case 'r':
			if ((body.find('2')->second.x == wall.getRight() - 3 || ground.find(Coordinate(body.find('2')->second.x + 2, body.find('2')->second.y)) != ground.end()) || (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x + 2, body.find('8')->second.y)) != ground.end()))
			{
				break;
			}
			eraseBlock();
			body.find('2')->second.x += 2;
			body.find('5')->second.x += 2;
			body.find('8')->second.x += 2;
			body.find('4')->second.x += 2;
			printBlock();
			break;
		}
		break;
	case ' ':
		switch (state)
		{
		case 'u':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			//����while���ֱ��������������ж�,һ���Ǹ÷��������沿���Ƿ���ground�赲,һ���Ƿ����Ƿ��䵽��
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('4')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'l':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				body.find('6')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'd':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			set<Coordinate>::iterator iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			while (((iter1 == ground.end() && iter2 == ground.end()) && iter3 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('8')->second.y += 1;
				body.find('4')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('6')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
				iter3 = ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1));
			}
			printBlock();
			break;
		}
		case 'r':
		{
			eraseBlock();
			set<Coordinate>::iterator iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
			set<Coordinate>::iterator iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			while ((iter1 == ground.end() && iter2 == ground.end()) && body.find('8')->second.y + 1 != wall.getButtom())
			{
				body.find('2')->second.y += 1;
				body.find('5')->second.y += 1;
				body.find('8')->second.y += 1;
				body.find('4')->second.y += 1;
				iter1 = ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1));
				iter2 = ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1));
			}
			printBlock();
			break;
		}
		}
		break;
	}
}
void Hill::rotation()
{
	//���ڵ�ǰ����״̬���з��������
	switch (state)
	{
	case 'u':
		//��5��λΪ׼,��תǰ�Ƚ�����ײ���,�ȼ����ת���λ���Ƿ��Ѵ��ڷ���,�ټ����ת;���Ƿ���ڷ���
		if (ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end())))
		{
			//�ٶ���ת�Ƿ�ײǽ�����ж�
			if (body.find('5')->second.y + 1 < wall.getButtom())
			{
				eraseBlock();//������ԭ��״̬�ķ���,�ٸı䷽���״̬
				state = 'l';//�ı䷽��״̬
				//����5��λԪ�ص�x,yֵ,������Ԫ��
				body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
				body.erase('4');//��4��λԪ��ɾ��
				printBlock();
			}
		}
		break;
	case 'l':
		if (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end())))
		{
			if (body.find('5')->second.x - 2 > wall.getLeft())
			{
				eraseBlock();
				state = 'd';
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.erase('2');
				printBlock();
			}
		}
		break;
	case 'd':
		if (ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end())))
		{
			//��ͼ��֪,���鴦��r״̬��u״̬ת���϶�����ײǽ
			eraseBlock();
			state = 'r';
			body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
			body.erase('6');
			printBlock();
		}
		break;
	case 'r':
		if (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end())))
		{
			if (body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();
				state = 'u';
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.erase('8');
				printBlock();
			}
		}
		break;
	}
}
void Hill::fall()
{
	switch (state)
	{
	case 'u':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		printBlock();
		break;
	case 'l':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		body.find('6')->second.y += 1;
		printBlock();
		break;
	case 'd':
		eraseBlock();
		body.find('8')->second.y += 1;
		body.find('4')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('6')->second.y += 1;
		printBlock();
		break;
	case 'r':
		eraseBlock();
		body.find('2')->second.y += 1;
		body.find('5')->second.y += 1;
		body.find('8')->second.y += 1;
		body.find('4')->second.y += 1;
		printBlock();
		break;
	}
}
bool Hill::crash() const
{
	switch (state)
	{
	case 'u':
		//�ֱ��ж��Ƿ���ground�ڵķ�����ײ����ײ��ǽ��
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) !=ground.end()) || ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end()) || body.find('4')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'l':
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'd':
		if (((ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end()) || ground.find(Coordinate(body.find('6')->second.x, body.find('6')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	case 'r':
		if ((ground.find(Coordinate(body.find('8')->second.x, body.find('8')->second.y + 1)) != ground.end() || ground.find(Coordinate(body.find('4')->second.x, body.find('4')->second.y + 1)) != ground.end()) || body.find('8')->second.y + 1 == wall.getButtom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
void Hill::mix()
{
	switch (state)
	{
	case 'u':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//��4��λ,����ground
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5��λ
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6��λ
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2��λ
		break;
	case 'l':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		break;
	case 'd':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));
		break;
	case 'r':
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));
		break;
	}
}
short Hill::eraseLine()
{
	short line = 0;//������������
	//�������ϼ��
	short xL = wall.getLeft() + 2;//����һ�������x����
	short xR = wall.getRight() - 3;//����һ�����ұ�x����
	//����֮��ѭ��ֵ�͸��������y����
	//�������y����
	short arr[] = { 0,0,0 };//�������濪ʼ��
	short round = 0;//Ҳ�൱������ʵ�ʵĳ���
	short y = 0;//���������y����
	switch (state)
	{
	case 'u':
		round = 2;
		y = body.find('4')->second.y;
		break;
	case 'd':
		round = 2;
		y = body.find('8')->second.y;
		break;
	case 'r':
	case 'l':
		round = 3;
		y = body.find('8')->second.y;
		break;
	}
	//ͳ�����е��е�y����
	for (short i = 0; i < round; i++)//ѭ�������鸳ֵ
	{
		short num = 1;//��������м�������
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//ָ����е�һ������
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//ָ��������һ������	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//�������û�е�һ�������һ������,��ֱ�ӽ����´�ѭ��
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//����ӵ���ʱ�������ϾͲ���ı�iter1��ֵ
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//�ж��Ƿ�����
		{
			line++;
			arr[i] = y - i;
		}
	}
	//��ʼ����
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//ִ�����鶯��
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//��������
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//ɾ������Ԫ�صȲ���
		for (short i = 0; i < round; i++)//��������
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//��ֹɾ������Ԫ������û��Ԫ��,�Ӷ�ground.begin()ָ��������Ԫ������
				{
					for_each_backword(temp, --ground.begin(), yPlus());
					arrPlus(arr, round);
					for_each_backword(temp, --ground.begin(), printNew());
				}
			}
		}
	}
	return line;
}
bool Hill::isOver() const
{
	switch (state)
	{
	case 'u':
		return body.find('4')->second.y == wall.getTop();
	case 'd':
	case 'r':
	case 'l':
		return body.find('8')->second.y == wall.getTop();
	}
}
void Hill::printBlock(const string& pattern) const
{
	switch (state)
	{
	case 'u':
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);//������Ƶ�4��λ
			cout << pattern << pattern << pattern;//����λ�����ͼ��
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);//2��λ
			cout << pattern;
		}
		break;
	case 'l':
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);
			cout << pattern;
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern;
		}
		break;
	case 'd':
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);
			cout << pattern;
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern << pattern;
		}
		break;
	case 'r':
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);
			cout << pattern;
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);
			cout << pattern << pattern;
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);
			cout << pattern;
		}
		break;
	}
}