#include "top.h"
#include "datagen.h"
#include "mixer.h"
#include "checker.h"
using namespace sc_core;
top::top(sc_module_name inst)
: sc_module(inst)
, gen (new datagen("gen"))
, mix (new mixer ("mix"))
, chk (new checker("chk"))
{ // Instantiate modules
// Connectivity
gen->d1(c1);
gen->d2(c2);
mix->i1(c1);
mix->i2(c2);
mix->vo(chk->vi);
};
