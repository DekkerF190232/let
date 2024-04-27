#pragma once
#include "Parent.h"
template <typename T>
class Listener
{
public:
	virtual void handle_action(T* t) = 0;
};


