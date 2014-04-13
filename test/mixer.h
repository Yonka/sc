#ifndef MIXER_H
#define MIXER_H
#include "systemc.h"
SC_MODULE(mixer)
{
    sc_port<sc_fifo_in_if<int> > i1;
sc_port<sc_fifo_in_if<int> > i2;
sc_export<sc_fifo<int> > vo;
// Module constructor
SC_CTOR(mixer);
private:
// Process declaration
void mixer_thread(void);
};
#endif
