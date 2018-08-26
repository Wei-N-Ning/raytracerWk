//
// Created by wein on 26/08/18.
//

#include <vec3.hh>

#include <cassert>

int main() {
    using namespace RTWK;

    auto vec3_equal = [](const Vec3& lhs, const Vec3& rhs) {
        assert(lhs[0] == rhs[0]);
        assert(lhs[1] == rhs[1]);
        assert(lhs[2] == rhs[2]);
    };

    Vec3 v1{1, 2, 3};
    Vec3 v2{(-v1)[2], (+v1).g(), (-v1).x()};
    vec3_equal(v2, {-3, 2, -1});

    v2 += v1;
    vec3_equal(v2, {-2, 4, 2});

    v1 -= v2;
    vec3_equal(v1, {3, -2, 1});

    v1 *= v1;
    vec3_equal(v1, {9, 4, 1});

    v1 /= v1;
    vec3_equal(v1, {1, 1, 1});
    vec3_equal(Vec3::unit(), v1);

    Vec3 v3{10, 5, 15};
    vec3_equal((v3 *= 0.2), {2, 1, 3});
    vec3_equal((v3 /= 0.2), {10, 5, 15});

    assert(12 == Vec3(2, 2, 2).sqr_length());
    assert(2 == Vec3(2, 0, 0).length());

    return 0;
}