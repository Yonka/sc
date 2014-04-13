#include "checker.h"
#include <iostream>

checker::checker(sc_module_name nm): sc_module(nm), cnt(0), wid(8)
{
SC_HAS_PROCESS(checker);
SC_METHOD(checker_method);
vi.bind(c3);
}
void checker::checker_method(void)
{
next_trigger(c3.data_written_event());
while (c3.nb_read(pel))
{
    if (pel == 0) pel = int('.');
    std::cout << char(pel) << ' ';
    if ((cnt++ % wid) == (wid-1))
        std::cout << std::endl;
}
}
