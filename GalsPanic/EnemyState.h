#pragma once
#ifndef ENEMYSTATE_H_
#define ENEMYSTATE_H_

#include "State.h"
#include "Event.h"

class State;
class Machine;
class State;

class EnemyState : public State
{
public:
	virtual const char* GetName() = 0;
public:
	EnemyState() {}
	virtual ~EnemyState() {}

	virtual void OnEntry(Machine*, State*);
	virtual void OnExit(Machine*, State*);
	virtual void OnEvent(Machine*, Event*);

	virtual void Update(Machine*) = 0;

	virtual void Transition(Machine*, State*);
};

#endif