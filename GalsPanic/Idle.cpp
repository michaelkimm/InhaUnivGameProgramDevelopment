#include "Idle.h"

#include "Enemy.h"
#include "EnemyEvent.h"

#include "Attack.h"
#include "Dead.h"
#include "Alive.h"

Idle::~Idle()
{
	if (instance_ != nullptr)
		delete instance_;
}

Idle* Idle::instance_ = 0;

Idle * Idle::Instance()
{
	if (instance_ == nullptr)
		instance_ = new Idle();
	return instance_;
}

void Idle::OnEntry(Machine *m, State *s)
{
}

void Idle::OnExit(Machine *m, State *s)
{
}

void Idle::OnEvent(Machine *m, event::Event *e)
{
	Enemy*				pt_enemy	= static_cast<Enemy*> (m);
	event::EnemyEvent*  pt_event	= static_cast<event::EnemyEvent*> (e);

	switch (pt_event->code_)
	{
		case event::ec_detect_dt_passed:
		{
			pt_enemy->EnemyShake();
			pt_enemy->Transition(Attack::Instance());
		}
		break;

		case event::ec_en_attacked:
		{
			pt_enemy->EnemyShake();
			pt_enemy->Destroy();
			pt_enemy->Transition(Dead::Instance());
		}
		break;
		
		default:
			break;
	}
}

void Idle::Update(Machine *m)
{
}

void Idle::Transition(Machine *m, State *s)
{
}
