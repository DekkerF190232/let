#pragma once
#include "Parent.h"
#include "TextArea.h"
class TestParent : public Parent
{
	TextArea* ta;
	void on_bounds_updated();
};

