#include "Dead.h"

Dead* Dead::instance_ = 0;

Dead * Dead::Instance()
{
	if (instance_ == nullptr)
		instance_ = new Dead();
	return instance_;
}

Dead::~Dead()
{
	if (instance_ != nullptr)
		delete instance_;
}

void Dead::OnEntry(Machine *m, State *s)
{
}

void Dead::OnExit(Machine *m, State *s)
{
}

void Dead::OnEvent(Machine *m, event::Event *e)
{
}

void Dead::Update(Machine *m)
{
}

void Dead::Transition(Machine *m, State *s)
{
}
