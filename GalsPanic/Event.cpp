#include "Event.h"
#include "State.h"

#include <iostream>
using namespace std;

void Event::Notify(Machine *m, State *s)
{
	if (s)
	{
		cout << "state: " << s->GetName() << " is OnEvent\n";
		s->OnEvent(m, this);
	}
}
