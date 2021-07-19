#pragma once
#ifndef DEAD_H_
#define DEAD_H_

#include "EnemyState.h"
#include "Event.h"

class EnenmyState;

class Dead : public EnemyState
{
public:
	virtual const char* GetName() { return "Dead State"; }
public:
	static Dead* Instance();
private:
	static Dead* instance_;
protected:
	Dead() {}
	~Dead();

	void OnEntry(Machine*, State*) override;
	void OnExit(Machine*, State*) override;
	void OnEvent(Machine*, event::Event*) override;

	void Update(Machine*) override;

	void Transition(Machine*, State*) override;
};

#endif