#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "random.h"
#include "simulation.h"
#include "vec.h"


struct config const default_config = {
    .particle_count = 500,
    .mass           = 0.1,
    .epsilon        = 1,
    .sigma          = 0.1,
    .temperature    = 1,
    .timestep       = 1e-5,
    .steps          = 1000,
    .log_interval   = 0,
    .seed           = 0,
};


struct state
{
    struct vec    *positions;
    struct vec    *velocities;
    struct vec    *forces;
    double         potential_energy;
    double         time;
    struct random  random;
};


static void init_state(struct state *state, struct config const *config);
static void free_state(struct state *state);
static void compute_forcefield(struct state *state, struct config const *config);
static void simulate_motion(struct state *state, struct config const *config);

static double evaluate_lennard_jones(
    struct vec r, struct vec *force, double epsilon, double sigma
);


void run_simulation(struct config const *config, struct result *result)
{
    assert(config);
    assert(result);

    struct state state;
    init_state(&state, config);
    simulate_motion(&state, config);
    result->mean_energy = state.potential_energy / (double) config->particle_count;
    free_state(&state);
}


static
void init_state(struct state *state, struct config const *config)
{
    assert(state);
    assert(config);

    state->positions = calloc(sizeof(struct vec), config->particle_count);
    state->velocities = calloc(sizeof(struct vec), config->particle_count);
    state->forces = calloc(sizeof(struct vec), config->particle_count);
    state->potential_energy = 0;
    state->time = 0;
    random_seed(&state->random, config->seed);

    // Distribute particles on a grid.
    int span = (int) cbrt((double) config->particle_count);

    for (size_t i = 0; i < config->particle_count; i++) {
        int ix = (int) i;
        int iy = ix / span;
        int iz = iy / span;
        ix %= span;
        iy %= span;

        state->positions[i] = (struct vec) {
            .x = config->sigma * (double) ix,
            .y = config->sigma * (double) iy,
            .z = config->sigma * (double) iz,
        };
    }

    // Draw random velocity from Maxwell-Boltzmann distribution.
    double const maxwell = sqrt(2 * config->temperature / config->mass);
    for (size_t i = 0; i < config->particle_count; i++) {
        state->velocities[i] = (struct vec) {
            .x = maxwell * random_normal(&state->random),
            .y = maxwell * random_normal(&state->random),
            .z = maxwell * random_normal(&state->random),
        };
    }

    // Initialize forcefield.
    compute_forcefield(state, config);
}


static
void free_state(struct state *state)
{
    assert(state);

    free(state->positions);
    free(state->velocities);
    free(state->forces);
}


static void simulate_motion(struct state *state, struct config const *config)
{
    assert(state);
    assert(config);

    for (int64_t step = 0; step < config->steps; step++) {
        if (config->log_interval > 0 && step % config->log_interval == 0) {
            double mean_energy = state->potential_energy / (double) config->particle_count;
            fprintf(stderr, "%g\t%g\n", state->time, mean_energy);
        }

        // Velocity Verlet scheme.

        double dt = config->timestep;
        double dt_div_m = config->timestep / config->mass;

        size_t n = config->particle_count;
        struct vec *positions = state->positions;
        struct vec *velocities = state->velocities;
        struct vec const *forces = state->forces;

        for (size_t i = 0; i < n; i++) {
            velocities[i] = vec_add(velocities[i], vec_mul(forces[i], dt_div_m / 2));
            positions[i] = vec_add(positions[i], vec_mul(velocities[i], dt));
        }

        compute_forcefield(state, config);

        for (size_t i = 0; i < n; i++) {
            velocities[i] = vec_add(velocities[i], vec_mul(forces[i], dt_div_m / 2));
        }

        state->time += dt;
    }
}


static
void compute_forcefield(struct state *state, struct config const *config)
{
    assert(state);
    assert(config);

    size_t n = config->particle_count;
    struct vec const *positions = state->positions;
    struct vec *forces = state->forces;

    // Initialize to zero.
    double energy = 0;
    for (size_t i = 0; i < n; i++) {
        forces[i] = (struct vec) { 0, 0, 0 };
    }

    // Lennard-Jones forcefield.
    double epsilon = config->epsilon;
    double sigma = config->sigma;

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            struct vec force;
            struct vec r = vec_sub(positions[i], positions[j]);
            energy += evaluate_lennard_jones(r, &force, epsilon, sigma);
            forces[i] = vec_add(forces[i], force);
            forces[j] = vec_sub(forces[j], force);
        }
    }

    state->potential_energy = energy;
}


static
double evaluate_lennard_jones(
    struct vec r, struct vec *force, double epsilon, double sigma
)
{
    double r2_inv = 1 / vec_dot(r, r);
    double u2 = sigma * sigma * r2_inv;
    double u6 = u2 * u2 * u2;

    double force_divr = 12 * epsilon * (u6 * u6 - u6) * r2_inv;
    double energy = epsilon * (u6 * u6 - u6 - u6);

    *force = vec_mul(r, force_divr);
    return energy;
}
