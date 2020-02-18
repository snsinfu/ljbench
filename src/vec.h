#ifndef INCLUDED_VEC_H
#define INCLUDED_VEC_H


struct vec
{
    double x;
    double y;
    double z;
};

struct vec vec_add(struct vec v1, struct vec v2);
struct vec vec_sub(struct vec v1, struct vec v2);
struct vec vec_mul(struct vec v, double a);
double     vec_dot(struct vec v1, struct vec v2);


inline
struct vec vec_add(struct vec v1, struct vec v2)
{
    return (struct vec) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z,
    };
}


inline
struct vec vec_sub(struct vec v1, struct vec v2)
{
    return (struct vec) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z,
    };
}


inline
struct vec vec_mul(struct vec v, double a)
{
    return (struct vec) {
        .x = v.x * a,
        .y = v.y * a,
        .z = v.z * a,
    };
}


inline
double vec_dot(struct vec v1, struct vec v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

#endif
