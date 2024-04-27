#include "FlexElement.h"

FlexElement::FlexElement(UiState* state, bool vertical) :
	Parent(state),
	_vertical(vertical),
	_data_map()
{
}

void FlexElement::_add(std::shared_ptr<Element> element, FlexData data)
{
	if (_data_map.find(element.get()) != _data_map.end()) throw std::invalid_argument("added element twice");
	_data_map.insert({ element.get(), data });
	Parent::add(element);
}

void FlexElement::on_bounds_updated()
{
	int availableSize = vertical() ? get_height() : get_width();
	int position = vertical() ? get_y() : get_x();

	float lastFloatSize = 0;

	std::vector<std::shared_ptr<Element>> children = get_children();

	auto lastWeighted = std::find_if(children.rbegin(), children.rend(),
		[this](std::shared_ptr<Element> el) {
			return _data_map[el.get()].weight == 0;
		});

	float totalWeight = 0;
	int totalFixedArea = 0;
	for (auto& entry : _data_map)
	{
		FlexData data = entry.second;
		totalWeight += data.weight;
		if (data.weight == 0) totalFixedArea += data.fixed_size;
		else totalFixedArea += vertical() ? data.margin.ver() : data.margin.hor();
	}
	int totalWeightedSize = availableSize - totalFixedArea;

	for (auto it = children.begin(); it != children.end(); ++it)
	{
		Element* child = it->get();
		FlexData data = _data_map[child];

		// margin
		int margin = vertical() ? data.margin.ver() : data.margin.hor();
		int oppositeMargin = vertical() ? data.margin.hor() : data.margin.ver();
		int oppositeSize = vertical() ? get_width() : get_height();

		// stretchy size
		int size;
		if (data.weight == 0)
		{
			size = data.fixed_size + margin;
		}
		else
		{
			bool isLastWeighted = lastWeighted != children.rend() && *lastWeighted == *it;
			if (isLastWeighted)
			{
				size = totalWeightedSize - position;
			}
			else
			{
				lastFloatSize += data.weight / totalWeight * totalWeightedSize;
				size = math_floor(lastFloatSize);
				lastFloatSize -= size;
			}
		}

		int cx = vertical() ? get_x() : position;
		int cy = vertical() ? position : get_y();
		int cw = vertical() ? oppositeSize : size;
		int ch = vertical() ? size : oppositeSize;

		child->_layout_update_bounds(cx + data.margin.left, cy + data.margin.top,
			std::max(0, cw - data.margin.hor()), std::max(0, ch - data.margin.ver()));

		position += size;
	}
}

void FlexElement::on_removing(Element* element)
{
	_data_map.erase(element);
}

bool FlexElement::vertical() const { return _vertical; };