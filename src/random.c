#include <math.h>
#include <stdint.h>

#include "random.h"


enum {
    SFC64_INITIAL_ROUND = 12,
    SFC64_ROT           = 24,
    SFC64_RSHIFT        = 11,
    SFC64_LSHIFT        = 3,
};


void random_seed(struct random *random, uint64_t seed)
{
    random->a = seed;
    random->b = seed;
    random->c = seed;
    random->counter = 1;

    for (int i = 0; i < SFC64_INITIAL_ROUND; i++) {
        random_next(random);
    }
}


uint64_t random_next(struct random *random)
{
    uint64_t output = random->a + random->b + random->counter;

    random->a = random->b ^ (random->b >> SFC64_RSHIFT);
    random->b = random->c + (random->c << SFC64_LSHIFT);
    random->c = ((random->c << SFC64_ROT) | random->c >> (64 - SFC64_ROT)) + output;
    random->counter++;

    return output;
}


double random_uniform(struct random *random)
{
    return 0x1p-64 * (double) random_next(random);
}


double random_normal(struct random *random)
{
    double u, v, s;

    do {
        u = random_uniform(random);
        v = random_uniform(random);
        s = u * u + v * v;
    } while (s > 1 || s == 0);

    s = 1 / s;
    s = s * log(s);

    return u * sqrt(s + s);
}
