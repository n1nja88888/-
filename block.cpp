//注:0在ASCII表中对应的十进制是48
#include"block.h"
template<class functor>
void for_each_backword(set<Coordinate>::iterator last, set<Coordinate>::iterator first, functor fn)//为eraseLine函数写一个算法,逆序遍历容器内元素
{
	while (last != first) {
		fn(*last);
		--last;
	}
}
struct yPlus 
{
	//自从VS2010开始，set的iterator类型自动就是const的引用类型，因此当set保存的是类类型时，对iterator解引用无法调用类的非const成员。
	void operator()(const Coordinate& coord) { const_cast<Coordinate&>(coord).y++; }
};
struct printNew 
{
	void operator()(const Coordinate& coord) const
	{
		gotoxy(coord.x, coord.y - 1);
		cout << "□";
		gotoxy(coord.x, coord.y);
		cout << "■";
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
//Block基类
set<Coordinate> Block::ground;
Wall Block::wall;
void Block::setWall(const Wall& w)
{
	wall = w;
}
void Block::erase(const Coordinate& coord) const
{
	gotoxy(coord.x, coord.y);
	cout << "□□□□";
	gotoxy(coord.x, coord.y + 1);
	cout << "□□□□";
}
void Block::printLine(const string& pattern) const
{
	for (short i = 0; i < (wall.getRight() - wall.getLeft() - 3) / 2; i++)
	{
		cout << pattern;
	}
}
//■■
//■■
Square::Square(bool cmd)
{
	state = 'u';//初始化state
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 1;
	short y = wall.getTop() - 1;
	body.insert(make_pair('1', Coordinate(x, y)));//插入左上角的方块格
	//从左往右,从上到下,依次插入
	body.insert(make_pair('2', Coordinate(x + 2, y)));
	body.insert(make_pair('4', Coordinate(x, y + 1)));
	body.insert(make_pair('5', Coordinate(x + 2, y + 1)));
	if (cmd)
	{
		printBlock();//为真则输出图案
	}
}
void Square::print(const Coordinate& coord) const 
{
erase(coord);//先擦除之前的方块
gotoxy(coord.x + 2, coord.y);
cout << "■■";
gotoxy(coord.x + 2, coord.y + 1);
cout << "■■";
}
void Square::translation(char cmd)
{
	switch (cmd) {
	case 'a':
		//下面if语句分别对两种情况进行判断,一个是对应移动方向一侧的方块是否要碰墙,一个是是对应移动方向一侧的方块是否有ground阻挡
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
		//while分别考虑两种情况,方块是否遇到ground阻挡,方块是否落到底
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
	//当发生碰撞或者y+i达到地面就将方块融入地面
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
	ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));//将'1'号位,即iter1,存入ground
	ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//'2'号位
	ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//'4'号位
	ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//'5'号位
}
short Square::eraseLine()
{
	short line = 0;//储存消除行数
	//储存各行y坐标
	short arr[] = { 0,0 };//从最下面开始存
	//从下往上检测
	short xL = wall.getLeft() + 2;//储存一行最左边x坐标
	short xR = wall.getRight() - 3;//储存一行最右边x坐标
	short y = body.find('4')->second.y;
	//统计满行的行的y坐标
	for (short i = 0; i < 2; i++)
	{
		short num = 1;//储存该行有几个方块
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//指向该行第一个方块
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//指向该行最后一个方块	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//如果该行没有第一个或最后一个方块,则直接进入下次循环
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//这里加到临时迭代器上就不会改变iter1的值
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//判断是否满行
		{
			line++;
			arr[i] = y - i;
		}
	}
	//开始动画
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//执行三遍动画
		{
			Sleep(animationTime);
			for (short j = 0; j < 2; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < 2; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//删除满行元素等操作
		for (short i = 0; i < 2; i++)//遍历数组
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp!=ground.end())//防止删除满行元素上面没有元素,从而ground.begin()指向了满行元素下面
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
		gotoxy(body.find('1')->second.x, body.find('1')->second.y);//将光标移到1号位
		cout << pattern << pattern;//在新位置输出图案
	}
	if (body.find('4')->second.y > wall.getTop())
	{
		gotoxy(body.find('4')->second.x, body.find('4')->second.y);//4号位
		cout << pattern << pattern;
	}
	
}
//■
//■
//■
//■
Stick::Stick(bool cmd)
{
	state = 'l';//初始化state
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//居中出现
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
	erase(coord);//先擦除之前的方块
	gotoxy(coord.x, coord.y + 1);
	cout << "■■■■";
}
void Stick::translation(char cmd) 
{
	switch (cmd) 
	{
	case 'a':
		switch (state)//再对方块所处状态进行分类讨论
		{
		case 'u':
			//下面if语句分别对两种情况进行判断,一个是对应移动方向一侧的方块是否要碰墙,一个是是对应移动方向一侧的方块是否有ground阻挡
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
	//对于当前方块状态进行分情况讨论
	switch (state)
	{
	case 'u':
		//以5号位为准,旋转前先进行碰撞检测.先检测旋转后的位置是否已存在方块,再检测旋转途中是否存在方块
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 4, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end())))
		{
			//再对旋转是否撞墙进行判断
			if (body.find('5')->second.x - 4 > wall.getLeft() && body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();//先消除原有状态的方块,再改变方块的状态
				state = 'l';//改变方块状态
				//依照5号位元素的x,y值,插入新元素
				body.insert(make_pair('b', Coordinate(body.find('5')->second.x - 4, body.find('5')->second.y)));
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.erase('2');//把2号位元素删除
				body.erase('8');//8号位
				body.erase('a');//a号位
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
		//分别判断是否与ground内的方块碰撞或者撞到墙底
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
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//将2号位,存入ground
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8号位
		ground.insert(Coordinate(body.find('a')->second.x, body.find('a')->second.y));//a号位
		break;
	case 'l':
		ground.insert(Coordinate(body.find('b')->second.x, body.find('b')->second.y));//将b号位,存入ground
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//4号位
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6号位
		break;
	case 'd':
		ground.insert(Coordinate(body.find('c')->second.x, body.find('c')->second.y));//将c号位,存入ground
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2号位
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8号位
		break;
	case 'r':
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//将4号位,存入ground
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6号位
		ground.insert(Coordinate(body.find('d')->second.x, body.find('d')->second.y));//d号位
		break;
	}
}
short Stick::eraseLine()
{
	short line = 0;//储存消除行数
	//从下往上检测
	short xL = wall.getLeft() + 2;//储存一行最左边x坐标
	short xR = wall.getRight() - 3;//储存一行最右边x坐标
	//控制之后循环值和各个情况的y坐标
	//储存各行y坐标
	short arr[] = { 0,0,0,0 };//从最下面开始存
	short round = 0;//也相当于数组实际的长度
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
	//统计满行的行的y坐标
	for (short i = 0; i < round; i++)//循环给数组赋值
	{
		short num = 1;//储存该行有几个方块
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//指向该行第一个方块
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//指向该行最后一个方块	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//如果该行没有第一个或最后一个方块,则直接进入下次循环
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//这里加到临时迭代器上就不会改变iter1的值
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//判断是否满行
		{
			line++;
			arr[i] = y - i;
		}
	}
	//开始动画
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//执行三遍动画
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//删除满行元素等操作
		for (short i = 0; i < round; i++)//遍历数组
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//防止删除满行元素上面没有元素,从而ground.begin()指向了满行元素下面
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
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);//将光标移到2号位
			cout << pattern;//在新位置输出图案
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5号位
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//8号位
			cout << pattern;
		}
		if (body.find('a')->second.y > wall.getTop())
		{
			gotoxy(body.find('a')->second.x, body.find('a')->second.y);//a号位
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
//■■
//  ■
//  ■
LeftCane::LeftCane(bool cmd)
{
	state = 'l';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//居中出现
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
	erase(coord);//先擦除之前的方块
	gotoxy(coord.x + 4, coord.y);
	cout << "■";
	gotoxy(coord.x, coord.y + 1);
	cout << "■■■";
}
void LeftCane::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//下面if语句分别对两种情况进行判断,一个是对应移动方向一侧的方块是否要碰墙,一个是是对应移动方向一侧的方块是否有ground阻挡
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
	//对于当前方块状态进行分情况讨论
	switch (state)
	{
	case 'u':
		//以5号位为准,旋转前先进行碰撞检测,先检测旋转后的位置是否已存在方块,再检测旋转途中是否存在方块
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end()&& ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end()))
		{
			//再对旋转是否撞墙进行判断
			if (body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();//先消除原有状态的方块,再改变方块的状态
				state = 'l';//改变方块状态
				//依照5号位元素的x,y值,插入新元素
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('3', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)));
				body.erase('1');//把1号位元素删除
				body.erase('2');//2号位
				body.erase('8');//8号位
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
			//画图可知,方块处在l状态向d状态转换肯定不会撞墙
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
		//分别判断是否与ground内的方块碰撞或者撞到墙底
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
		ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));//将1号位,存入ground
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2号位
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8号位
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
	short line = 0;//储存消除行数
	//从下往上检测
	short xL = wall.getLeft() + 2;//储存一行最左边x坐标
	short xR = wall.getRight() - 3;//储存一行最右边x坐标
	//控制之后循环值和各个情况的y坐标
	//储存各行y坐标
	short arr[] = { 0,0,0 };//从最下面开始存
	short round = 0;//也相当于数组实际的长度
	short y = 0;//储存最低行y坐标
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
	//统计满行的行的y坐标
	for (short i = 0; i < round; i++)//循环给数组赋值
	{
		short num = 1;//储存该行有几个方块
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//指向该行第一个方块
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//指向该行最后一个方块	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//如果该行没有第一个或最后一个方块,则直接进入下次循环
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//这里加到临时迭代器上就不会改变iter1的值
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//判断是否满行
		{
			line++;
			arr[i] = y - i;
		}
	}
	//开始动画
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//执行三遍动画
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//删除满行元素等操作
		for (short i = 0; i < round; i++)//遍历数组
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//防止删除满行元素上面没有元素,从而ground.begin()指向了满行元素下面
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
			gotoxy(body.find('1')->second.x, body.find('1')->second.y);//将光标移到2号位
			cout << pattern << pattern;//在新位置输出图案
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5号位
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//8号位
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
//■■
//■
//■
RightCane::RightCane(bool cmd)
{
	state = 'r';// 为了和LeftCane对称,这里初始化状态为r
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//居中出现
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
	erase(coord);//先擦除之前的方块
	gotoxy(coord.x, coord.y);
	cout << "■";
	gotoxy(coord.x, coord.y + 1);
	cout << "■■■";
}
void RightCane::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//下面if语句分别对两种情况进行判断,一个是对应移动方向一侧的方块是否要碰墙,一个是是对应移动方向一侧的方块是否有ground阻挡
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
	//对于当前方块状态进行分情况讨论
	switch (state)
	{
	case 'u':
		//以5号位为准,旋转前先进行碰撞检测,先检测旋转后的位置是否已存在方块,再检测旋转途中是否存在方块
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end()))
		{
			//再对旋转是否撞墙进行判断
			if (body.find('5')->second.x - 2 > wall.getLeft())
			{
				eraseBlock();//先消除原有状态的方块,再改变方块的状态
				state = 'l';//改变方块状态
				//依照5号位元素的x,y值,插入新元素
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('6', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y)));
				body.insert(make_pair('9', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)));
				body.erase('3');//把3号位元素删除
				body.erase('2');//2号位
				body.erase('8');//8号位
				printBlock();
			}
		}
		break;
	case 'l':
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end()) && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end()))
		{
			//画图可知,方块处在l状态向d状态转换肯定不会撞墙
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
		//分别判断是否与ground内的方块碰撞或者撞到墙底
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
		ground.insert(Coordinate(body.find('3')->second.x, body.find('3')->second.y));//将3号位,存入ground
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2号位
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8号位
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
	short line = 0;//储存消除行数
	//从下往上检测
	short xL = wall.getLeft() + 2;//储存一行最左边x坐标
	short xR = wall.getRight() - 3;//储存一行最右边x坐标
	//控制之后循环值和各个情况的y坐标
	//储存各行y坐标
	short arr[] = { 0,0,0 };//从最下面开始存
	short round = 0;//也相当于数组实际的长度
	short y = 0;//储存最低行y坐标
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
	//统计满行的行的y坐标
	for (short i = 0; i < round; i++)//循环给数组赋值
	{
		short num = 1;//储存该行有几个方块
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//指向该行第一个方块
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//指向该行最后一个方块	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//如果该行没有第一个或最后一个方块,则直接进入下次循环
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//这里加到临时迭代器上就不会改变iter1的值
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//判断是否满行
		{
			line++;
			arr[i] = y - i;
		}
	}
	//开始动画
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//执行三遍动画
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//删除满行元素等操作
		for (short i = 0; i < round; i++)//遍历数组
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//防止删除满行元素上面没有元素,从而ground.begin()指向了满行元素下面
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
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);//将光标移到2号位
			cout << pattern << pattern;//在新位置输出图案
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5号位
			cout << pattern;
		}
		if (body.find('8')->second.y > wall.getTop())
		{
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//8号位
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
//■
//■■
//  ■
LeftLightning::LeftLightning(bool cmd)
{
	state = 'l';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//居中出现
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
	erase(coord);//先擦除之前的方块
	gotoxy(coord.x + 2, coord.y);
	cout << "■■";
	gotoxy(coord.x, coord.y + 1);
	cout << "■■";
}
void LeftLightning::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//下面if语句分别对两种情况进行判断,一个是对应移动方向一侧的方块是否要碰墙,一个是是对应移动方向一侧的方块是否有ground阻挡
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
	//对于当前方块状态进行分情况讨论
	switch (state)
	{
	case 'u':
		//以5号位为准,旋转前先进行碰撞检测,先检测旋转后的位置是否已存在方块,再检测旋转途中是否存在方块
		if ((ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end())
		{
			//再对旋转是否撞墙进行判断
			if (body.find('5')->second.x + 2 < wall.getRight() - 1)
			{
				eraseBlock();//先消除原有状态的方块,再改变方块的状态
				state = 'l';//改变方块状态
				//依照5号位元素的x,y值,插入新元素
				body.insert(make_pair('2', Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)));
				body.insert(make_pair('3', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)));
				body.erase('1');//把1号位元素删除
				body.erase('8');//8号位
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
			//画图可知,方块处在r状态向u状态转换肯定不会撞墙
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
		//分别判断是否与ground内的方块碰撞或者撞到墙底
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
		ground.insert(Coordinate(body.find('1')->second.x, body.find('1')->second.y));//将1号位,存入ground
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//4号位
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8号位
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
	short line = 0;//储存消除行数
	//从下往上检测
	short xL = wall.getLeft() + 2;//储存一行最左边x坐标
	short xR = wall.getRight() - 3;//储存一行最右边x坐标
	//控制之后循环值和各个情况的y坐标
	//储存各行y坐标
	short arr[] = { 0,0,0 };//从最下面开始存
	short round = 0;//也相当于数组实际的长度
	short y = 0;//储存最低行y坐标
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
	//统计满行的行的y坐标
	for (short i = 0; i < round; i++)//循环给数组赋值
	{
		short num = 1;//储存该行有几个方块
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//指向该行第一个方块
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//指向该行最后一个方块	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//如果该行没有第一个或最后一个方块,则直接进入下次循环
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//这里加到临时迭代器上就不会改变iter1的值
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//判断是否满行
		{
			line++;
			arr[i] = y - i;
		}
	}
	//开始动画
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//执行三遍动画
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//删除满行元素等操作
		for (short i = 0; i < round; i++)//遍历数组
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//防止删除满行元素上面没有元素,从而ground.begin()指向了满行元素下面
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
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//将光标移到8号位
			cout << pattern;//在新位置输出图案
		}
		if (body.find('4')->second.y > wall.getTop())
		{
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);//4号位
			cout << pattern << pattern;
		}
		if (body.find('1')->second.y > wall.getTop())
		{
			gotoxy(body.find('1')->second.x, body.find('1')->second.y);//1号位
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
//  ■
//■■
//■
//靠右显示,为了与RightCane保持一致
RightLightning::RightLightning(bool cmd)
{
	state = 'l';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//居中出现
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
	erase(coord);//先擦除之前的方块
	gotoxy(coord.x, coord.y);
	cout << "■■";
	gotoxy(coord.x + 2, coord.y + 1);
	cout << "■■";
}
void RightLightning::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//下面if语句分别对两种情况进行判断,一个是对应移动方向一侧的方块是否要碰墙,一个是是对应移动方向一侧的方块是否有ground阻挡
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
	//对于当前方块状态进行分情况讨论
	switch (state)
	{
	case 'u':
		//以5号位为准,旋转前先进行碰撞检测,先检测旋转后的位置是否已存在方块,再检测旋转途中是否存在方块
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end())
		{
			//再对旋转是否撞墙进行判断
			if (body.find('5')->second.x - 2 > wall.getLeft())
			{
				eraseBlock();//先消除原有状态的方块,再改变方块的状态
				state = 'l';//改变方块状态
				//依照5号位元素的x,y值,插入新元素
				body.insert(make_pair('4', Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y)));
				body.insert(make_pair('9', Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)));
				body.erase('3');//把3号位元素删除
				body.erase('6');//8号位
				printBlock();
			}
		}
		break;
	case 'l':
		if ((ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y + 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y - 1)) == ground.end()) && ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end())
		{
			//画图可知,方块处在l状态向d状态转换肯定不会撞墙
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
			//画图可知,方块处在r状态向u状态转换肯定不会撞墙
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
		//分别判断是否与ground内的方块碰撞或者撞到墙底
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
		ground.insert(Coordinate(body.find('3')->second.x, body.find('3')->second.y));//将3号位,存入ground
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6号位
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('8')->second.x, body.find('8')->second.y));//8号位
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
	short line = 0;//储存消除行数
	//从下往上检测
	short xL = wall.getLeft() + 2;//储存一行最左边x坐标
	short xR = wall.getRight() - 3;//储存一行最右边x坐标
	//控制之后循环值和各个情况的y坐标
	//储存各行y坐标
	short arr[] = { 0,0,0 };//从最下面开始存
	short round = 0;//也相当于数组实际的长度
	short y = 0;//储存最低行y坐标
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
	//统计满行的行的y坐标
	for (short i = 0; i < round; i++)//循环给数组赋值
	{
		short num = 1;//储存该行有几个方块
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//指向该行第一个方块
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//指向该行最后一个方块	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//如果该行没有第一个或最后一个方块,则直接进入下次循环
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//这里加到临时迭代器上就不会改变iter1的值
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//判断是否满行
		{
			line++;
			arr[i] = y - i;
		}
	}
	//开始动画
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//执行三遍动画
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//删除满行元素等操作
		for (short i = 0; i < round; i++)//遍历数组
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//防止删除满行元素上面没有元素,从而ground.begin()指向了满行元素下面
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
			gotoxy(body.find('8')->second.x, body.find('8')->second.y);//将光标移到8号位
			cout << pattern;//在新位置输出图案
		}
		if (body.find('5')->second.y > wall.getTop())
		{
			gotoxy(body.find('5')->second.x, body.find('5')->second.y);//5号位
			cout << pattern << pattern;
		}
		if (body.find('3')->second.y > wall.getTop())
		{
			gotoxy(body.find('3')->second.x, body.find('3')->second.y);//3号位
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
//  ■
//■■■
Hill::Hill(bool cmd)
{
	state = 'u';
	short x = (wall.getLeft() + wall.getRight() - 1) / 2 - 3;//居中出现
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
	erase(coord);//先擦除之前的方块
	gotoxy(coord.x + 2, coord.y);
	cout << "■";
	gotoxy(coord.x, coord.y + 1);
	cout << "■■■";
}
void Hill::translation(char cmd)
{
	switch (cmd)
	{
	case 'a':
		switch (state)
		{
		case 'u':
			//下面if语句分别对两种情况进行判断,一个是对应移动方向一侧的方块是否要碰墙,一个是是对应移动方向一侧的方块是否有ground阻挡
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
			//下面while语句分别对两种情况进行判断,一个是该方块最下面部分是否有ground阻挡,一个是方块是否落到底
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
	//对于当前方块状态进行分情况讨论
	switch (state)
	{
	case 'u':
		//以5号位为准,旋转前先进行碰撞检测,先检测旋转后的位置是否已存在方块,再检测旋转途中是否存在方块
		if (ground.find(Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x - 2, body.find('5')->second.y - 1)) == ground.end() && (ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y - 1)) == ground.end() && ground.find(Coordinate(body.find('5')->second.x + 2, body.find('5')->second.y + 1)) == ground.end())))
		{
			//再对旋转是否撞墙进行判断
			if (body.find('5')->second.y + 1 < wall.getButtom())
			{
				eraseBlock();//先消除原有状态的方块,再改变方块的状态
				state = 'l';//改变方块状态
				//依照5号位元素的x,y值,插入新元素
				body.insert(make_pair('8', Coordinate(body.find('5')->second.x, body.find('5')->second.y + 1)));
				body.erase('4');//把4号位元素删除
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
			//画图可知,方块处在r状态向u状态转换肯定不会撞墙
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
		//分别判断是否与ground内的方块碰撞或者撞到墙底
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
		ground.insert(Coordinate(body.find('4')->second.x, body.find('4')->second.y));//将4号位,存入ground
		ground.insert(Coordinate(body.find('5')->second.x, body.find('5')->second.y));//5号位
		ground.insert(Coordinate(body.find('6')->second.x, body.find('6')->second.y));//6号位
		ground.insert(Coordinate(body.find('2')->second.x, body.find('2')->second.y));//2号位
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
	short line = 0;//储存消除行数
	//从下往上检测
	short xL = wall.getLeft() + 2;//储存一行最左边x坐标
	short xR = wall.getRight() - 3;//储存一行最右边x坐标
	//控制之后循环值和各个情况的y坐标
	//储存各行y坐标
	short arr[] = { 0,0,0 };//从最下面开始存
	short round = 0;//也相当于数组实际的长度
	short y = 0;//储存最低行y坐标
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
	//统计满行的行的y坐标
	for (short i = 0; i < round; i++)//循环给数组赋值
	{
		short num = 1;//储存该行有几个方块
		set<Coordinate>::iterator iter1 = ground.find(Coordinate(xL, y - i));//指向该行第一个方块
		set<Coordinate>::iterator iter2 = ground.find(Coordinate(xR, y - i));//指向该行最后一个方块	
		if (iter1 == ground.end() || iter2 == ground.end())
		{
			continue;//如果该行没有第一个或最后一个方块,则直接进入下次循环
		}
		set<Coordinate>::iterator temp = iter1;
		while (temp != iter2)
		{
			temp++;//这里加到临时迭代器上就不会改变iter1的值
			num++;
		}
		if (num == (wall.getRight() - wall.getLeft() - 3) / 2)//判断是否满行
		{
			line++;
			arr[i] = y - i;
		}
	}
	//开始动画
	if (line > 0)
	{
		for (short i = 0; i < 3; i++)//执行三遍动画
		{
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printBlankLine();
				}
			}
			Sleep(animationTime);
			for (short j = 0; j < round; j++)//遍历数组
			{
				if (arr[j])
				{
					gotoxy(xL, arr[j]);
					printLine();
				}
			}
		}
		//删除满行元素等操作
		for (short i = 0; i < round; i++)//遍历数组
		{
			if (arr[i])
			{
				set<Coordinate>::iterator temp = --ground.find(Coordinate(xL, arr[i]));
				ground.erase(ground.find(Coordinate(xL, arr[i])), ++ground.find(Coordinate(xR, arr[i])));
				gotoxy(xL, arr[i]);
				printBlankLine();
				if (temp != ground.end())//防止删除满行元素上面没有元素,从而ground.begin()指向了满行元素下面
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
			gotoxy(body.find('4')->second.x, body.find('4')->second.y);//将光标移到4号位
			cout << pattern << pattern << pattern;//在新位置输出图案
		}
		if (body.find('2')->second.y > wall.getTop())
		{
			gotoxy(body.find('2')->second.x, body.find('2')->second.y);//2号位
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