#include "EnemyEvent.h"

#include <iostream>
using namespace std;

event::EnemyEvent* event::EnemyEvent::instance_ = 0;

event::EnemyEvent * event::EnemyEvent::Instance(enemy_event_code _code)
{
	if (instance_ != nullptr)
	{
		instance_ = new EnemyEvent(_code);
	}
	return instance_;
}

void event::EnemyEvent::Notify(Machine *m, State *s)
{
	switch (code_)
	{
	case event::ec_detect_dt_passed:
	{
		cout << "I am event 'ec_detect_dt_passed', ";
	}
		break;
	
	case event::ec_en_attacked:
	{
		cout << "I am event 'ec_en_attacked', ";
	}
		break;

	case event::ec_en_Idle_dt:
	{
		cout << "I am event 'ec_en_Idle_dt', ";
	}
		break;

	default:
		break;
	}
	cout << "Notify to machine m of state s that im here!\n";

	event::Event::Notify(m, s);
}

void event::EnemyEvent::SetEventCode(enemy_event_code _code)
{
	code_ = _code;
}
