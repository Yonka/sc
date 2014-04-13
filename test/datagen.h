#ifndef DATAGEN_H
#define DATAGEN_H
#include "systemc.h"
SC_MODULE(datagen) {
// Ports
sc_port <sc_fifo_out_if<int> > d1, d2;
// Process declaration
void stim_thread(void);
SC_CTOR(datagen);
};
#endif
