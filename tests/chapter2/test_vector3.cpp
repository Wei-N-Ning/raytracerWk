//
// Created by wein on 26/08/18.
//

#include <vec3.hh>

#include <sstream>
#include <iostream>
#include <cassert>
#include <cstring>

void RunTinyTests();

void vec3_equal(const RTWK::Vec3& lhs, const RTWK::Vec3& rhs) {
    assert(lhs[0] == rhs[0]);
    assert(lhs[1] == rhs[1]);
    assert(lhs[2] == rhs[2]);
};

void test_ctor() {
    using namespace RTWK;

    Vec3 v1{1, 2, 3};
    Vec3 v2{(-v1)[2], (+v1).g(), (-v1).x()};
    vec3_equal(v2, {-3, 2, -1});
}

void test_sign_op() {
    using namespace RTWK;

    Vec3 v1{1, 2, 3};
    Vec3 v2{(-v1)[2], (+v1).g(), (-v1).x()};

    v2 += v1;
    vec3_equal(v2, {-2, 4, 2});

    v1 -= v2;
    vec3_equal(v1, {3, -2, 1});
}

void test_arithmetic_op() {
    using namespace RTWK;

    Vec3 v1{1, 2, 3};

    v1 *= v1;
    vec3_equal(v1, {1, 4, 9});

    v1 /= v1;
    vec3_equal(v1, {1, 1, 1});
    vec3_equal(Vec3::unit(), v1);

    Vec3 v3{10, 5, 15};
    vec3_equal((v3 *= 0.2), {2, 1, 3});
    vec3_equal((v3 /= 0.2), {10, 5, 15});
}

void test_length() {
    using namespace RTWK;

    Vec3 v3{10, 5, 15};
    assert(12 == Vec3(2, 2, 2).sqr_length());
    assert(2 == Vec3(2, 0, 0).length());
}

void test_ostream() {
    using namespace RTWK;

    Vec3 v3{10, 5, 15};
    std::stringstream ss;
    ss << v3 << std::endl;

    assert(0 == std::strcmp(
        "10 5 15\n", ss.str().c_str()
        ));
}

void test_istream() {
    using namespace RTWK;

    Vec3 tmp;
    std::stringstream ss;
    ss.str(std::string{"11 22 33\n44 55 66"});
    ss >> tmp;
    vec3_equal(tmp, {11, 22 ,33});
    ss >> tmp;
    vec3_equal(tmp, {44, 55 ,66});
    ss >> tmp;
    vec3_equal(tmp, {44, 55 ,66});
}

void test_dot_product() {
    using namespace RTWK;

    assert(-14 == dot({-1, 2, -3}, {1, -2, 3}));
}

void test_cross_product() {
    using namespace RTWK;

    Vec3 result = cross({1, 0, 0}, {0, 1, 0});
    vec3_equal({0, 0, 1}, result);

    result = cross({0, 1, 0}, {1, 0, 0});
    vec3_equal({0, 0, -1}, result);
}

int main(int argc, char **argv) {
    RunTinyTests();
    return 0;
}