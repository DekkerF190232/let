#pragma once
template <typename T>
class Observer
{
public:
	virtual void update(T t) = 0;
};
