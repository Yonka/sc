#ifndef TOP_H
#define TOP_H
#include <systemc>
struct datagen;
struct mixer;
struct checker;

SC_MODULE(top)
{
    SC_CTOR(top);

    sc_core::sc_fifo<int> c1;
    sc_core::sc_fifo<int> c2;
    datagen* gen;
    mixer* mix;
    checker* chk;


};
#endif // TOP_H
