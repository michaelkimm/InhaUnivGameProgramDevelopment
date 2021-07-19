#pragma once
#ifndef ENEMYEVENT_H_
#define ENEMYEVENT_H_

#include "Event.h"
#include "State.h"

class event::Event;
class Machine;
class State;

namespace event
{

	enum enemy_event_code { ec_detect_dt_passed, ec_en_attacked, ec_en_Idle_dt };

/*********************************************************************************
 CD Enemy STATE TRANSITION TABLE
---------------+---------------------+------------+--------------------------------------
 CURRENT STATE | EVENT               | NEXT STATE | TRANSITION ACTION
---------------+---------------------+------------+--------------------------------------
 Idle	       | ec_detect_dt_passed | Attack     | EnemyShake
 Idle	       | ec_en_attacked      | Dead       | EnemyShake, Destroy
 Attack        | ec_en_Idle_dt       | Idle       | -
 Attack        | ec_en_attacked      | Dead       | EnemyShake, Destroy
---------------+---------------------+------------+--------------------------------------
*********************************************************************************/

	class EnemyEvent : public event::Event
	{
	private:
		static EnemyEvent* instance_;
	public:
		static EnemyEvent* Instance(enemy_event_code _code);
	protected:
		explicit EnemyEvent(enemy_event_code _code) : code_(_code) {}
		~EnemyEvent() {}
	public:
		enemy_event_code code_;
		virtual void Notify(Machine*, State*);
		void SetEventCode(enemy_event_code _code);
	};
}
#endif

