#pragma once

#include "Observer.h"
#include <vector>
#include <memory>
#include <stdexcept>

template <typename T>
class Subject
{

private:
	std::vector<std::weak_ptr<Observer<T>>> _observers;

public:
	void notify(T t);
	void subscribe(std::weak_ptr<Observer<T>> observer);
	void unsubscribe(Observer<T>* observer);
};

template <typename T>
void Subject<T>::notify(T value)
{
	for (size_t i = 0; i < _observers.size(); i++)
	{
		auto l = _observers[i].lock();
		if (l == nullptr)
		{
			_observers.erase(_observers.begin() + i);
			i--;
			continue;
		}
		l->update(value);
	}
}

template <typename T>
void Subject<T>::subscribe(std::weak_ptr<Observer<T>> observer)
{
	if (observer.expired()) throw std::runtime_error("added nullptr observer");
	_observers.push_back(observer);
}

template <typename T>
void Subject<T>::unsubscribe(Observer<T>* observer)
{
	for (size_t i = 0; i < _observers.size(); i++)
	{
		auto l = _observers[i].lock();
		if (l == nullptr || l == observer)
		{
			_observers.erase(_observers.begin() + i);
			i--;
		}
	}
}