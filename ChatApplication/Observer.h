#pragma once

#include <vector>


class Observer
{
public:
	virtual ~Observer() {}
	virtual void update() = 0;
};


class Subject
{
protected:
	std::vector<Observer*> observers;
public:
	Subject() {}

	virtual ~Subject()
	{
		for (auto obs : this->observers)
		{
			//delete obs;
		}
	}

	virtual void registerObserver(Observer * obs)
	{
		this->observers.push_back(obs);
	}

	virtual void unregisterObserver(Observer * obs)
	{
		this->observers.erase(std::find(this->observers.begin(), this->observers.end(), obs));
	}

	virtual void notify()
	{
		for (auto obs : this->observers)
			obs->update();
	}
};