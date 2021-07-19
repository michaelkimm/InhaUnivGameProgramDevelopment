#pragma once
#ifndef ALIVE_H_
#define ALIVE_H_

#include "EnemyState.h"
#include "Event.h"

class EnemyState;

class Alive : public EnemyState
{
public:
	Alive() {}
	virtual ~Alive() {}

	virtual void OnEntry(Machine*, State*);
	virtual void OnExit(Machine*, State*);
	virtual void OnEvent(Machine*, event::Event*);

	virtual void Update(Machine*);

	virtual void Transition(Machine*, State*);
};

#endif