#pragma once
#ifndef EVENT_H_
#define EVENT_H_

#include "State.h"


class Machine;
class State;

namespace event
{
	class Event
	{
	private:
	public:
		Event() {}
		virtual ~Event() {}

		virtual void Notify(Machine*, State*);
	};
}



#endif


