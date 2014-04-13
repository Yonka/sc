#include "top.h"
using namespace sc_core;
int exit_status = 0;

int sc_main(int args, char *argv[])
{
    top top_i("top_i");
    sc_start(100, SC_NS);
    if (!sc_end_of_simulation_invoked())
        sc_stop();
    return exit_status;
}
