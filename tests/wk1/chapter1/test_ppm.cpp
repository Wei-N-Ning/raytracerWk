//
// Created by wein on 26/08/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <ppm.hh>
#include <sstream>

TEST_CASE("expect pixel values")
{
    std::ostringstream oss;
    RTWK1::createTestImage( oss, 2, 2);
    CHECK_EQ(oss.str(), R"(P3
2 2
255
0 0 51
127 0 51
0 127 51
127 127 51
)");
}
