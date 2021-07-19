#include "EnemyState.h"

#include <iostream>
using namespace std;

void EnemyState::OnEntry(Machine *m, State *s)
{
	cout << "OnEntry EnemyState: " << GetName() << endl;
}

void EnemyState::OnExit(Machine *m, State *s)
{
	cout << "OnExit EnemyState\n" << GetName() << endl;
}

void EnemyState::OnEvent(Machine *m, Event *e)
{
}

void EnemyState::Update(Machine *m)
{
}

void EnemyState::Transition(Machine *m, State *s)
{
}
