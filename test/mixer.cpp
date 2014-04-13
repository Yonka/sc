#include "mixer.h"
using namespace sc_core;
// Constructor
mixer::mixer(sc_module_name inst)
: sc_module(inst)
{ // Register processes
SC_HAS_PROCESS(mixer);
SC_THREAD(mixer_thread);
}
// Processes
void mixer::mixer_thread(void) {
int k1, val1, k2, val2;
while (true) {
k1
= i1->read();
val1 = i1->read();
k2
= i2->read();
val2 = i2->read();
vo->write(k1*val1 + k2*val2);
}
}
