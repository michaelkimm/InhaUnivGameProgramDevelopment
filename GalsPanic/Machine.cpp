#include "Machine.h"
#include "State.h"
#include "Event.h"

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

void Machine::Notify(event::Event *e)
{
	e->Notify(this, state_);
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
