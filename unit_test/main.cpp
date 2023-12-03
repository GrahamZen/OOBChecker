#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Interval.h"
#include "Domain.h"

using namespace dataflow;

TEST_CASE("basic arithmetic", "[interval]") {
    using D = Interval;
    auto INF = D::INF();

    SECTION("addition") {
        for (int a = 1; a <= 10; ++a) {
            for (int b = 1; b <= 10; ++b) {
                REQUIRE((D{a}+D{b}) == D{a+b});
                REQUIRE((D{b}+D{a}) == D{b+a});
            }
        }

        REQUIRE((D{1,2}+D{1,2}) == D{2,4});
        REQUIRE((D{1,2}+D{3,4}) == D{4,6});
        REQUIRE((D{1,2}+D{-4,-3} == D{-3,-1}));
        REQUIRE((D{-2,-1}+D{3,4}) == D{1,3});
        REQUIRE((D{-2,-1}+D{-4,-3}) == D{-6,-4});
        REQUIRE((D{-1,2}+D{3,4} == D{2,6}));
        REQUIRE((D{-1,2}+D{-4,-3} == D{-5,-1}));
        REQUIRE((D{1,2}+D{-4,3} == D{-3,5}));
        REQUIRE((D{-2,-1}+D{-4,3} == D{-6,2}));
        REQUIRE((D{-2,1}+D{-4,3} == D{-6,4}));
    }

    SECTION("subtraction and negation") {
        for (int a = 1; a <= 10; ++a) {
            for (int b = 1; b <= 10; ++b) {
                REQUIRE((D{a}-D{b}) == D{a-b});
                REQUIRE((D{b}-D{a}) == D{b-a});
                REQUIRE((-D{a}-D{b}) == D{-a-b});
                REQUIRE((-D{b}-D{a}) == D{-b-a});
            }
        }
        
        REQUIRE((D{2,10}-D{3} == D{-1,7}));
        REQUIRE((D{1,2}-D{3,4}) == D{-3,-1});
        REQUIRE((D{1,2}-D{-4,-3} == D{4,6}));
        REQUIRE((D{3}-D{2,10} == D{-7,1}));
        REQUIRE((D{2,9}-D{4,8} == D{-6,5}));
        
        REQUIRE((-D{1}) == D{-1});
        REQUIRE((-D{1,2} == D{-2,-1}));
        REQUIRE((D{2,10}+-D{3} == D{-1,7}));
        REQUIRE((D{1,2}+-D{3,4}) == D{-3,-1});
        REQUIRE((D{1,2}+-D{-4,-3} == D{4,6}));
        REQUIRE((D{3}+-D{2,10} == D{-7,1}));
        REQUIRE((D{2,9}+-D{4,8} == D{-6,5}));
    }

    SECTION("multiplication") {
        for (int a = 1; a <= 10; ++a) {
            for (int b = 1; b <= 10; ++b) {
                REQUIRE((D{a}*D{b}) == D{a*b});
                REQUIRE((D{a}*D{-b}) == D{-a*b});
                REQUIRE((D{-a}*D{b}) == D{-a*b});
                REQUIRE((D{-a}*D{-b}) == D{a*b});
            }
        }

        REQUIRE((D{1,2}*D{1,2}) == D{1,4});
        REQUIRE((D{1,2}*D{3,4}) == D{3,8});
        REQUIRE((D{1,2}*D{-4,-3} == D{-8,-3}));
        REQUIRE((D{-2,-1}*D{3,4}) == D{-8,-3});
        REQUIRE((D{-2,-1}*D{-4,-3}) == D{3,8});
        REQUIRE((D{-1,2}*D{3,4} == D{-4,8}));
        REQUIRE((D{-1,2}*D{-4,-3} == D{-8,4}));
        REQUIRE((D{1,2}*D{-4,3} == D{-8,6}));
        REQUIRE((D{-2,-1}*D{-4,3} == D{-6,8}));
        REQUIRE((D{-2,1}*D{-4,3} == D{-6,8}));
    }

    SECTION("division") {
        for (int a = 1; a <= 10; ++a) {
            for (int b = 1; b <= 10; ++b) {
                REQUIRE((D{a}/D{b}) == D{a/b});
                REQUIRE((D{a}/D{-b}) == D{-a/b});
                REQUIRE((D{-a}/D{b}) == D{-a/b});
                REQUIRE((D{-a}/D{-b}) == D{a/b});
            }
        }
        REQUIRE((D{1,2}/D{1,2}) == D{0,2});
        REQUIRE((D{1,2}/D{3,4}) == D{0});
        REQUIRE((D{1,2}/D{-4,-3} == D{0}));
        REQUIRE((D{-2,-1}/D{3,4}) == D{0});
        REQUIRE((D{-2,-1}/D{-4,-3}) == D{0});
        REQUIRE((D{-1,2}/D{3,4} == D{0}));
        REQUIRE((D{-1,2}/D{-4,-3} == D{0}));
        REQUIRE((D{1,2}/D{-3,4} == D{0,INF.upper()}));      
    }
}

TEST_CASE("comparison", "[interval]") {
    using D = Interval;
    auto INF = D::INF();

    SECTION("equality") {
        REQUIRE(1 == D{1});
        REQUIRE(1 != D{2});
        REQUIRE(3 != D{1,2});
        REQUIRE(1 == D{1,1});
        REQUIRE(D{1,2} == D{1,2});
        REQUIRE(INF == INF);
        REQUIRE(-INF == -INF);
        REQUIRE(1 != INF);
    }
}

TEST_CASE("domain arithmetic", "[domain]") {
    using D = IntervalDomain;
    SECTION("addition") {
        for (int a = 1; a <= 10; ++a) {
            for (int b = 1; b <= 10; ++b) {
                REQUIRE((D{a}+D{b}) == D{a+b});
                REQUIRE((D{b}+D{a}) == D{b+a});
            }
        }

        REQUIRE((D{1,2}+D{1,2}) == D{2,4});
        REQUIRE((D{1,2}+D{3,4}) == D{4,6});
        REQUIRE((D{1,2}+D{-4,-3} == D{-3,-1}));
        REQUIRE((D{-2,-1}+D{3,4}) == D{1,3});
        REQUIRE((D{-2,-1}+D{-4,-3}) == D{-6,-4});
        REQUIRE((D{-1,2}+D{3,4} == D{2,6}));
        REQUIRE((D{-1,2}+D{-4,-3} == D{-5,-1}));
        REQUIRE((D{1,2}+D{-4,3} == D{-3,5}));
        REQUIRE((D{-2,-1}+D{-4,3} == D{-6,2}));
        REQUIRE((D{-2,1}+D{-4,3} == D{-6,4}));
    }

}
