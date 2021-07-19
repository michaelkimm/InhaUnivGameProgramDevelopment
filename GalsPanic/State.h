#pragma once
#ifndef STATE_H_
#define STATE_H_

#include "Event.h"

class Machine;
class Event;

class State
{
public:
	virtual const char* GetName() = 0;
public:
	State() {}
	virtual ~State() {}

	virtual void OnEntry(Machine*, State*);
	virtual void OnExit(Machine*, State*);
	virtual void OnEvent(Machine*, Event *);

	virtual void Update(Machine*);
	
	virtual void Transition(Machine*, State*);

};

#endif