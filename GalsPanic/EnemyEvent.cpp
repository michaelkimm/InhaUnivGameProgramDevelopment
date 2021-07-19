#include "EnemyEvent.h"

#include <iostream>
using namespace std;

EnemyEvent* EnemyEvent::instance_ = 0;

EnemyEvent * EnemyEvent::Instance(enemy_event_code _code)
{
	if (instance_ == nullptr)
	{
		instance_ = new EnemyEvent(_code);
	}
	// 적 코드 업데이트
	instance_->code_ = _code;
	
	return instance_;
}

enemy_event_code code_;

EnemyEvent::~EnemyEvent()
{
	if (instance_ != nullptr)
		delete instance_;
}

void EnemyEvent::Notify(Machine *m, State *s)
{
	switch (code_)
	{
	case ec_detect_dt_passed:
	{
		cout << "I am event 'ec_detect_dt_passed', ";
	}
		break;
	
	case ec_en_attacked:
	{
		cout << "I am event 'ec_en_attacked', ";
	}
		break;

	case ec_en_Idle_dt:
	{
		cout << "I am event 'ec_en_Idle_dt', ";
	}
		break;

	default:
		break;
	}
	cout << "Notify to machine m of state s that im here!\n";

	Event::Notify(m, s);
}

void EnemyEvent::SetEventCode(enemy_event_code _code)
{
	code_ = _code;
}
