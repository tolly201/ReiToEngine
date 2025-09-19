#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <cmath>

namespace RTTest {
struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> r; return r;
}

inline void registerTest(const std::string& name, std::function<void()> fn) {
    registry().push_back({name, std::move(fn)});
}

inline int runAll() {
    int failed = 0;
    for (auto& t : registry()) {
        try {
            t.fn();
            std::cout << "[PASS] " << t.name << "\n";
        } catch (const std::exception& e) {
            std::cout << "[FAIL] " << t.name << ": " << e.what() << "\n";
            failed++;
        } catch (...) {
            std::cout << "[FAIL] " << t.name << ": unknown error\n";
            failed++;
        }
    }
    std::cout << "-- Summary: " << (registry().size() - failed) << "/" << registry().size() << " passed --\n";
    return failed;
}

struct AutoReg {
    AutoReg(const std::string& name, std::function<void()> fn) {
        registerTest(name, std::move(fn));
    }
};
}

#define RT_ASSERT_TRUE(cond) do { if(!(cond)) throw std::runtime_error("Assertion failed: " #cond); } while(0)
#define RT_ASSERT_NEAR(a,b,eps) do { if(std::fabs((a)-(b)) > (eps)) throw std::runtime_error("Assertion failed: |" #a "-" #b "|>eps"); } while(0)
#define RT_TEST(name) static void name(); static RTTest::AutoReg _auto_##name(#name, name); static void name()
