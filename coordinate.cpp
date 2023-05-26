#include"coordinate.h"
bool Coordinate::operator< (const Coordinate& coord)const
{
    if (y < coord.y) 
    {
        return true;
    }
    else if (y > coord.y)
    {
        return false;
    }
    else if (y == coord.y) 
    {
        if (x >= coord.x)
            return false;
        else
            return true;
    }
}
void gotoxy(short x, short y) 
{
    COORD pos;
    pos.X = x - 1;
    pos.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}