#include "Thickness.h"

Thickness::Thickness(int top, int right, int bottom, int left) :
	top(top),
	right(right),
	bottom(bottom),
	left(left)
{
}

Thickness::Thickness() :
	top(0),
	right(0),
	bottom(0),
	left(0)
{
}

Thickness::Thickness(int all) :
	top(all),
	right(all),
	bottom(all),
	left(all)
{
}

Thickness::Thickness(int hor, int ver) :
	top(ver),
	right(hor),
	bottom(ver),
	left(hor)
{
}

int Thickness::ver() const { return top + bottom; }
int Thickness::hor() const { return left + right; }