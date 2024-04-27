#pragma once
#include "Thickness.h"
class FlexData
{
public:
	Thickness margin;
	int fixed_size;
	float weight;

	static FlexData fixed(int fixedSize, Thickness thickness = Thickness {0});
	static FlexData weighted(float weight, Thickness thickness = Thickness{ 0 });

	FlexData();

private:
	FlexData(Thickness margin, int fixedSize, float weight);
	

};

