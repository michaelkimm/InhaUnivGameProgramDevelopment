#include "State.h"

#include "Event.h"

#include <iostream>
using namespace std;

void State::OnEntry(Machine *, State *)
{
}

void State::OnExit(Machine *, State *)
{
}

void State::OnEvent(Machine *, event::Event *)
{
}


void State::Update(Machine *)
{
}

void State::Transition(Machine *, State *)
{
}
