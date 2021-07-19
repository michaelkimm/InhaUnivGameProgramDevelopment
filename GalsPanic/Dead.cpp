#include "Dead.h"

#include "Attack.h"
#include "Alive.h"
#include "Idle.h"

#include "Enemy.h"
#include "Event.h"
#include "EnemyEvent.h"

#include <iostream>
using namespace std;

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
	cout << "Dead state onEntry!\n";
}

void Dead::OnExit(Machine *m, State *s)
{
	cout << "Dead state onExit!\n";
	cout << "------------------------------------------------------------\n";
}

void Dead::OnEvent(Machine *m, Event *e)
{
	// 머신과 이벤트 정적 캐스팅
	Enemy*		pt_enemy = static_cast<Enemy*> (m);
	EnemyEvent*	pt_Event = static_cast<EnemyEvent*> (e);

	// 이벤트에 따라 transition 결정
	switch (pt_Event->code_)
	{
	default:
		break;
	}
}

void Dead::Update(Machine *m)
{
}

void Dead::Transition(Machine *m, State *s)
{
}
