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
	// �ӽŰ� �̺�Ʈ ���� ĳ����
	Enemy*		pt_enemy = static_cast<Enemy*> (m);
	EnemyEvent*	pt_Event = static_cast<EnemyEvent*> (e);

	// �̺�Ʈ�� ���� transition ����
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
