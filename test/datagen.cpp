#include <fstream>
#include "datagen.h"

datagen::datagen(sc_module_name nm)
    : sc_module(nm)
{
    SC_HAS_PROCESS(datagen);
    SC_THREAD(stim_thread);
}

void datagen::stim_thread(void) {
std::ifstream f1("img1"),f2("img2");
do {
int data1, data2;
f1 >> data1; f2 >> data2;
d1->write(data1);
d2->write(data2);
} while (!f1.eof() && !f2.eof());
// What happens when this exits?
}
