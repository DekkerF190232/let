#include "ScrollContent.h"

ScrollContent::ScrollContent() :
	_width(0),
	_height(0)
{
}

void ScrollContent::set_content_size(int width, int height)
{
	if (_width == width && _height == height) return;
	_width = width;
	_height = height;
	size_changed.notify(this);
}

int ScrollContent::get_content_width() const { return _width; }
int ScrollContent::get_content_height() const { return _height; }
