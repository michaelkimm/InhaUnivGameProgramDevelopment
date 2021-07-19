#pragma once
#ifndef IDLE_H_
#define IDLE_H_

#include "Alive.h"
#include "Event.h"

class Alive;
class Event;

class Idle : public Alive
{
public:
	virtual const char* GetName() { return "Idle State"; }
private:
	static Idle* instance_;
public:
	static Idle* Instance();
protected:
	Idle() : Alive() {}
	~Idle();
public:

	void OnEntry(Machine*, State*) override;
	void OnExit(Machine*, State*) override;
	void OnEvent(Machine*, Event*) override;

	void Update(Machine*) override;

	void Transition(Machine*, State*) override;
};

#endif