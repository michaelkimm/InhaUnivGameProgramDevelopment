#pragma once
#ifndef MACHINE_H_
#define MACHINE_H_

#include <Windows.h>

#include "Event.h"
#include "State.h"

class State;
class Event;

class Machine
{
protected:
	State* state_ = 0;

	POINT pose_;
	POINT prev_pose_;
	float vx_;
	float vy_;
	SIZE collision_size_;
public:
	explicit Machine(State* _state, POINT _pose, float _vx, float _vy, SIZE _collision_size) {}
	virtual ~Machine() {}

	virtual void Transition(State*);
	virtual void Notify(event::Event *e);
	State* GetState() const;

	virtual int Draw(HDC) const;
	virtual int Update();
	virtual int Input();
	virtual int Input(HWND, UINT, WPARAM, LPARAM);
};

#endif