#include "Event.h"
#include "State.h"

#include <iostream>
using namespace std;

void event::Event::Notify(Machine *m, State *s)
{
	cout << "state: " << s->GetName() << " is OnEvent\n";
	s->OnEvent(m, this);
}
