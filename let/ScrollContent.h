#pragma once
#include "Subject.h"
class ScrollContent
{
private:
	int _width;
	int _height;
public:
	Subject<const ScrollContent*> size_changed;
	ScrollContent();
	void set_content_size(int width, int height);
	int get_content_width() const;
	int get_content_height() const;
};