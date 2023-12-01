#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Domain.h"
using namespace dataflow;

TEST_CASE("basic arithmetic", "[domain]") {
    using D = IntervalDomain;
    auto INF = D::INF_DOMAIN();

    SECTION("addition") {
        REQUIRE((D{1}+D{1}) == D{2});
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
        REQUIRE((D{1}-D{1}) == D{0});
        REQUIRE((D{2,10}-D{3} == D{-1,7}));
        REQUIRE((D{1,2}-D{3,4}) == D{-3,-1});
        REQUIRE((D{1,2}-D{-4,-3} == D{4,6}));
        REQUIRE((D{3}-D{2,10} == D{-7,1}));
        REQUIRE((D{2,9}-D{4,8} == D{-6,5}));
        
        REQUIRE((-D{1}) == D{-1});
        REQUIRE((-D{1,2} == D{-2,-1}));
        REQUIRE((D{1}-D{1}) == D{0});
        REQUIRE((D{2,10}+-D{3} == D{-1,7}));
        REQUIRE((D{1,2}+-D{3,4}) == D{-3,-1});
        REQUIRE((D{1,2}+-D{-4,-3} == D{4,6}));
        REQUIRE((D{3}+-D{2,10} == D{-7,1}));
        REQUIRE((D{2,9}+-D{4,8} == D{-6,5}));
    }

    SECTION("multiplication") {
        REQUIRE((D{1}*D{1}) == D{1});
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
        REQUIRE((D{1}/D{1}) == D{1});
        REQUIRE((D{1,2}/D{1,2}) == D{0,2});
        REQUIRE((D{1,2}/D{3,4}) == D{0});
        REQUIRE((D{1,2}/D{-4,-3} == D{0}));
        REQUIRE((D{-2,-1}/D{3,4}) == D{0});
        REQUIRE((D{-2,-1}/D{-4,-3}) == D{0});
        REQUIRE((D{-1,2}/D{3,4} == D{0}));
        REQUIRE((D{-1,2}/D{-4,-3} == D{0}));
        REQUIRE((D{1,2}/D{-3,4} == INF));        
    }
}

TEST_CASE("unknown", "[domain]") {
    using D = IntervalDomain;
    auto INF = D::INF_DOMAIN();
    auto UNINIT = D::UNINIT();

    SECTION("addition") {
        REQUIRE((D{1}+UNINIT) == UNINIT);
        REQUIRE((UNINIT+D{1}) == UNINIT);
        REQUIRE((UNINIT+UNINIT) == UNINIT);
        REQUIRE((INF+UNINIT) == UNINIT);
    }

    SECTION("subtraction and negation") {
        REQUIRE((D{1}-UNINIT) == UNINIT);
        REQUIRE((UNINIT-D{1}) == UNINIT);
        REQUIRE((UNINIT-UNINIT) == UNINIT);
        REQUIRE((INF-UNINIT) == UNINIT);
        REQUIRE(-UNINIT == UNINIT);
    }

    SECTION("multiplication") {
        REQUIRE((D{1}*UNINIT) == UNINIT);
        REQUIRE((UNINIT*D{1}) == UNINIT);
        REQUIRE((UNINIT*UNINIT) == UNINIT);
        REQUIRE((INF*UNINIT) == UNINIT);
    }

    SECTION("division") {
        REQUIRE((D{1}/UNINIT) == UNINIT);
        REQUIRE((UNINIT/D{1}) == UNINIT);
        REQUIRE((UNINIT/UNINIT) == UNINIT);
        REQUIRE((INF/UNINIT) == UNINIT);
    }

    SECTION("chained") {
        REQUIRE(D{2,3}*D{5,34}/D{2,3}+UNINIT == UNINIT);
        REQUIRE(D{2,3}*D{5,34}/D{2,3}+INF/UNINIT == UNINIT);
    }
}

TEST_CASE("comparison", "[domain]") {
    using D = IntervalDomain;
    auto INF = D::INF_DOMAIN();
    auto UNINIT = D::UNINIT();

    SECTION("equality") {
        REQUIRE(1 == D{1});
        REQUIRE(1 != D{2});
        REQUIRE(3 != D{1,2});
        REQUIRE(1 == D{1,1});
        REQUIRE(D{1,2} == D{1,2});
        REQUIRE(INF == INF);
        REQUIRE(UNINIT == UNINIT);
        REQUIRE(-INF == -INF);
        REQUIRE(-UNINIT == -UNINIT);
        REQUIRE(1 != INF);
        REQUIRE(1 != UNINIT);
        REQUIRE(INF != UNINIT);
    }
}

