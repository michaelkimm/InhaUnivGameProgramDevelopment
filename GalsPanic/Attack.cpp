#include "Attack.h"

#include "Alive.h"
#include "Dead.h"
#include "Idle.h"

#include "Enemy.h"
#include "Event.h"
#include "EnemyEvent.h"

Attack* Attack::instance_ = 0;

Attack * Attack::Instance()
{
	if (instance_ == nullptr)
		instance_ = new Attack();
	return instance_;
}

Attack::~Attack()
{
	if (instance_ != nullptr)
		delete instance_;
}

void Attack::OnEntry(Machine *m, State *s)
{
}

void Attack::OnExit(Machine *m, State *s)
{
}

void Attack::OnEvent(Machine *m, event::Event *e)
{
	// 머신과 이벤트 정적 캐스팅
	Enemy*				pt_enemy = static_cast<Enemy*> (m);
	event::EnemyEvent*	pt_Event = static_cast<event::EnemyEvent*> (e);

	// 이벤트에 따라 transition 결정
	switch (pt_Event->code_)
	{
	case event::ec_en_attacked:
	{
		pt_enemy->EnemyShake();
		pt_enemy->Destroy();
		pt_enemy->Transition(Dead::Instance());
	}
	break;

	case event::ec_en_Idle_dt:
	{
		pt_enemy->Transition(Idle::Instance());
	}
	break;

	default:
		break;
	}
}

void Attack::Update(Machine *m)
{
}

void Attack::Transition(Machine *m, State *s)
{
}
