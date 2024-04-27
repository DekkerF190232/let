#include "FlexData.h"
#include <limits>

FlexData::FlexData(Thickness margin, int fixedSize, float weight) :
    margin(margin),
    fixed_size(fixedSize),
    weight(weight)
{
}

FlexData FlexData::fixed(int fixedSize, Thickness thickness)
{
    return FlexData(thickness, fixedSize, 0);
}

FlexData FlexData::weighted(float weight, Thickness thickness)
{
    return FlexData(thickness, std::numeric_limits<int>::min(), weight);
}

FlexData::FlexData() : 
    FlexData({}, 0, 0)
{
}
