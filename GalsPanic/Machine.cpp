#include "Machine.h"
#include "State.h"
#include "Event.h"

#include <iostream>
using namespace std;

Machine::Machine(State * _state, POINT _pose, float _vx, float _vy, SIZE _collision_size)
	: state_(_state), pose_(_pose), vx_(_vx), vy_(_vy), collision_size_(_collision_size)
{
}

void Machine::Transition(State *cur_state)
{
	State* prev_state_;

	prev_state_ = state_;
	if (prev_state_)
		prev_state_->OnExit(this, cur_state);
	
	state_ = cur_state;
	if (state_)
		state_->OnEntry(this, prev_state_);

}

void Machine::Notify(Event *e)
{
	cout << "머신 Notify 함수 안임\n";

	if (e)
	{
		cout << "머신에서 이벤트 발생시킴\n";
		e->Notify(this, state_);
	}
}

State* Machine::GetState() const
{
	return state_;
}

int Machine::Draw(HDC) const
{
	return 0;
}

int Machine::Update()
{
	return 0;
}

int Machine::Input()
{
	return 0;
}

int Machine::Input(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}
