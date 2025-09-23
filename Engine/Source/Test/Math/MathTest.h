// Adapter to new core test framework. Tests can continue to use RT_TEST/RT_ASSERT_* macros.
#pragma once
#include "../Core/TestCore.h"
#include <cmath>

// Legacy style: RT_TEST(TestName) { ... }
// We map each RT_TEST to a default suite "Math" for grouping.

namespace RTMathAdapter {
inline RTTestCore::Suite& suite() {
    static bool init = [](){
        static int s_autoreg = [](){
            auto s = std::make_unique<RTTestCore::Suite>("Math");
            RTTestCore::Registry::instance().add(std::move(s));
            return 0;
        }();
        (void)s_autoreg; return true; }();
    (void)init;
    for (auto& sp : RTTestCore::Registry::instance().suites()) {
        if (sp->name()=="Math") return *sp;
    }
    // Fallback (should not happen)
    static RTTestCore::Suite fallback("Math");
    return fallback;
}
}

#define RT_TEST(name) \
    static void name(); \
    static int _auto_##name = [](){ RTMathAdapter::suite().add(#name, &name); return 0; }(); \
    static void name()

#define RT_ASSERT_TRUE(cond) RT_EXPECT_TRUE(cond)
#define RT_ASSERT_NEAR(a,b,eps) RT_EXPECT_NEAR(a,b,eps)
