#pragma once
#ifndef ATTACK_H_
#define ATTACK_H_

#include "Alive.h"
#include "Event.h"

class Alive;

class Attack : public Alive
{
public:
	virtual const char* GetName() { return "Alive State"; }
public:
	static Attack* Instance();
private:
	static Attack* instance_;
protected:
	Attack() : Alive() {}
	~Attack();
public:

	void OnEntry(Machine*, State*) override;
	void OnExit(Machine*, State*) override;
	void OnEvent(Machine*, event::Event*) override;

	void Update(Machine*) override;

	void Transition(Machine*, State*) override;
};

#endif