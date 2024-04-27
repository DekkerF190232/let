#pragma once
class Thickness
{
public:
	int top;
	int right;
	int bottom;
	int left;

	Thickness();
	Thickness(int all);
	Thickness(int hor, int ver);
	Thickness(int top, int right, int bottom, int left);

	int ver() const;
	int hor() const;
};

