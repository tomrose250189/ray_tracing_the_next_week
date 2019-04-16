#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED
#include "vec3.h"

std::function<float()> rn = std::bind(std::uniform_real_distribution<float>(), std::default_random_engine());

class ray {
public:
   ray() {}
   ray(const vec3& a, const vec3& b, float ti = 0.0) : A(a), B(b), _time(ti) {}
   vec3 origin() const { return A; }
   vec3 direction() const { return B; }
   float time() const { return _time; }
   vec3 point_at_parameter(float t) const { return A + t*B; }

   vec3 A, B;
   float _time;
};

#endif // RAY_H_INCLUDED
