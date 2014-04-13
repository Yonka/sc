#ifndef CHECKER_H
#define CHECKER_H
#include "systemc.h"
SC_MODULE(checker) {
// Ports
sc_core::sc_export
< sc_core::sc_fifo<int>
> vi;
sc_core::sc_fifo<int> c3;
// Module constructor
SC_CTOR(checker);
private:
int const wid; // row width
int
cnt;
int
pel; // pixel element
// Process declaration
void checker_method(void);
};
#endif
