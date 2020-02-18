#ifndef INCLUDED_RANDOM_H
#define INCLUDED_RANDOM_H

#include <stdint.h>


struct random
{
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t counter;
};

void     random_seed(struct random *random, uint64_t seed);
uint64_t random_next(struct random *random);
double   random_uniform(struct random *random);
double   random_normal(struct random *random);

#endif
