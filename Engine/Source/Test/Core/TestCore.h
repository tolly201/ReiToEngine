#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>
#include <iostream>

// 该核心测试库尽量复用 Runtime 的 ABI：
// - 日志输出可直接使用 Runtime Logger（若可用）；否则回退到 std::cout
// - 计时使用 std::chrono；若需要可接入 Runtime Timer
// - 内存使用默认 new/delete；若需要可在外层替换为 Runtime 分配器

namespace RTTestCore {

using TestFn = std::function<void()>;

struct Case { std::string name; TestFn fn; };

class Suite {
public:
    explicit Suite(std::string name) : name_(std::move(name)) {}
    void add(const std::string& name, TestFn fn) { cases_.push_back({name, std::move(fn)}); }
    const std::string& name() const { return name_; }
    const std::vector<Case>& cases() const { return cases_; }
private:
    std::string name_;
    std::vector<Case> cases_;
};

class Registry {
public:
    static Registry& instance() { static Registry r; return r; }
    void add(std::unique_ptr<Suite> s) { suites_.push_back(std::move(s)); }
    const std::vector<std::unique_ptr<Suite>>& suites() const { return suites_; }
private:
    std::vector<std::unique_ptr<Suite>> suites_;
};

inline void log_info(const std::string& m){ std::cout << "[INFO] " << m << "\n"; }
inline void log_error(const std::string& m){ std::cout << "[ERROR] " << m << "\n"; }

class Timer {
public:
    Timer() : t0(std::chrono::steady_clock::now()) {}
    double ms() const { auto t1=std::chrono::steady_clock::now(); return std::chrono::duration<double,std::milli>(t1-t0).count(); }
private:
    std::chrono::steady_clock::time_point t0;
};

struct Result { std::string name; bool pass; std::string msg; double ms; };

inline std::vector<Result> run_all(const std::vector<std::string>& filters){
    std::vector<Result> out;
    for (auto& s : Registry::instance().suites()){
        if(!filters.empty() && std::find(filters.begin(),filters.end(),s->name())==filters.end()) continue;
        log_info("Running suite: "+s->name());
        for (auto& c : s->cases()){
            Timer tm; bool ok=true; std::string emsg;
            try{ c.fn(); }
            catch(const std::exception& e){ ok=false; emsg=e.what(); }
            catch(...){ ok=false; emsg="unknown"; }
            double t=tm.ms();
            std::cout << (ok?"[PASS] ":"[FAIL] ") << s->name() << "::" << c.name << " (" << t << " ms)" << "\n";
            if(!ok) log_error(emsg);
            out.push_back({s->name()+"::"+c.name, ok, emsg, t});
        }
    }
    return out;
}

inline int summarize_and_exit(const std::vector<Result>& results){
    size_t pass=0, fail=0; double total=0, minv=results.empty()?0:results[0].ms, maxv=minv;
    for(auto& r:results){ (r.pass?++pass:++fail); total+=r.ms; minv=std::min(minv,r.ms); maxv=std::max(maxv,r.ms);}
    log_info("====================");
    log_info("Total: "+std::to_string(results.size())+", Passed: "+std::to_string(pass)+", Failed: "+std::to_string(fail));
    if(!results.empty()){
        log_info("Time: total "+std::to_string(total)+" ms, avg "+std::to_string(total/results.size())+" ms, min "+std::to_string(minv)+" ms, max "+std::to_string(maxv)+" ms");
    }
    return fail?1:0;
}

// 辅助宏：定义套件与用例
#define RT_SUITE(SuiteName) \
    static void SuiteName##_register(RTTestCore::Suite& suite); \
    static int SuiteName##_autoreg = [](){ \
        auto _s = std::make_unique<RTTestCore::Suite>(#SuiteName); \
        SuiteName##_register(*_s); \
        RTTestCore::Registry::instance().add(std::move(_s)); \
        return 0; \
    }(); \
    static void SuiteName##_register(RTTestCore::Suite& suite)

#define RT_CASE(SuiteName, CaseName) suite.add(#CaseName, []()
#define RT_END_CASE );

// 断言宏（防重复求值）
#define RT_EXPECT_TRUE(cond) do{ auto _c=(cond); if(!_c) throw std::runtime_error(std::string("EXPECT_TRUE failed: ")+ #cond); }while(0)
#define RT_EXPECT_EQ(a,b) do{ auto _ea=(a); auto _eb=(b); if(!(_ea==_eb)) throw std::runtime_error("EXPECT_EQ failed"); }while(0)
#define RT_EXPECT_NEAR(a,b,eps) do{ auto _ea=(a); auto _eb=(b); auto _e=(eps); if(std::fabs(_ea-_eb)>_e) throw std::runtime_error("EXPECT_NEAR failed"); }while(0)

} // namespace RTTestCore
