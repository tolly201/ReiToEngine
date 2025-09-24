#pragma once
#include <string>
#include <vector>
// =========================
// 强化版轻量测试框架 (Extensible)
// 支持特性：
//  - Suite / Case 注册
//  - 默认套件 RT_TEST_DEFAULT_SUITE 可配置（兼容旧 RT_TEST 单参数宏风格）
//  - Case 元数据：tags / file / line
//  - 过滤：按 suite / tag / case name 子串（由 main.cpp 解析实现）
//  - 将来可扩展：Fixture / 参数化 / 期望失败 (xfail)
// =========================

#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace RTTestCore {

using TestFn = std::function<void()>;

struct Case {
    std::string name;      // 短名称
    TestFn      fn;        // 执行体
    std::vector<std::string> tags; // 标签
    std::string file;      // 源文件
    int         line = 0;  // 行号
};

class Suite {
public:
    explicit Suite(std::string name) : name_(std::move(name)) {}
    void add(const std::string& name, TestFn fn, std::vector<std::string> tags, const char* file, int line) {
        cases_.push_back(Case{name, std::move(fn), std::move(tags), file, line});
    }
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
    Suite& get_or_create(const std::string& name){
        for(auto& s: suites_) if(s->name()==name) return *s;
        auto ns = std::make_unique<Suite>(name); auto* raw=ns.get(); suites_.push_back(std::move(ns)); return *raw;
    }
private:
    std::vector<std::unique_ptr<Suite>> suites_;
};

inline void log_info(const std::string& m){ std::cout << "[INFO] " << m << "\n"; }
inline void log_error(const std::string& m){ std::cout << "[ERROR] " << m << "\n"; }

class Timer { public: Timer():t0(std::chrono::steady_clock::now()){} double ms() const {return std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-t0).count(); } private: std::chrono::steady_clock::time_point t0; };

struct Result { std::string fullName; std::string suite; std::string caseName; bool pass; std::string msg; double ms; std::vector<std::string> tags; };

struct Filters {
    std::vector<std::string> suiteNames;  // 精确匹配
    std::vector<std::string> tagIncludes; // 任意一个 tag 命中即可
    std::vector<std::string> caseSubstr;  // 名称包含子串
};

inline bool match_filters(const Suite& s, const Case& c, const Filters& f){
    if(!f.suiteNames.empty() && std::find(f.suiteNames.begin(), f.suiteNames.end(), s.name())==f.suiteNames.end()) return false;
    if(!f.caseSubstr.empty()){
        bool ok=false; for(auto& sub: f.caseSubstr){ if(c.name.find(sub)!=std::string::npos) { ok=true; break; } } if(!ok) return false;
    }
    if(!f.tagIncludes.empty()){
        bool ok=false; for(auto& need: f.tagIncludes){ for(auto& t: c.tags){ if(t==need){ ok=true; break; } } if(ok) break; } if(!ok) return false;
    }
    return true;
}

inline std::vector<Result> run_all(const Filters& filters){
    std::vector<Result> out;
    for(auto& sp: Registry::instance().suites()){
        Suite& s=*sp;
        bool anyPrinted=false;
        for(auto& c: s.cases()){
            if(!match_filters(s,c,filters)) continue;
            if(!anyPrinted){ log_info("Running suite: "+s.name()); anyPrinted=true; }
            Timer tm; bool ok=true; std::string emsg;
            try { c.fn(); }
            catch(const std::exception& e){ ok=false; emsg=e.what(); }
            catch(...){ ok=false; emsg="unknown"; }
            double t=tm.ms();
            std::cout << (ok?"[PASS] ":"[FAIL] ") << s.name() << "::" << c.name << " (" << t << " ms)";
            if(!c.tags.empty()){
                std::cout << " [tags:"; for(size_t i=0;i<c.tags.size();++i){ if(i) std::cout<<","; std::cout<<c.tags[i]; } std::cout << "]"; }
            std::cout << "\n";
            if(!ok) log_error(emsg + " @" + c.file + ":" + std::to_string(c.line));
            out.push_back({s.name()+"::"+c.name, s.name(), c.name, ok, emsg, t, c.tags});
        }
    }
    return out;
}

inline int summarize_and_exit(const std::vector<Result>& results){
    size_t pass=0, fail=0; double total=0, minv=results.empty()?0:results[0].ms, maxv=minv;
    for(auto& r:results){ (r.pass?++pass:++fail); total+=r.ms; minv=std::min(minv,r.ms); maxv=std::max(maxv,r.ms);}
    log_info("====================");
    log_info("Total: "+std::to_string(results.size())+", Passed: "+std::to_string(pass)+", Failed: "+std::to_string(fail));
    if(!results.empty()) log_info("Time: total "+std::to_string(total)+" ms, avg "+std::to_string(total/results.size())+" ms, min "+std::to_string(minv)+" ms, max "+std::to_string(maxv)+" ms");
    return fail?1:0;
}

// ============ Registration Helpers ============
inline void RegisterCase(const char* suite, const char* caseName, TestFn fn, std::vector<std::string> tags, const char* file, int line){
    Registry::instance().get_or_create(suite).add(caseName, std::move(fn), std::move(tags), file, line);
}

#define RT_TEST_ST(SuiteName, TestName, ...) \
    static void TestName(); \
    static int TestName##_autoreg = (RTTestCore::RegisterCase(SuiteName, #TestName, &TestName, std::vector<std::string>{__VA_ARGS__}, __FILE__, __LINE__), 0); \
    static void TestName()

// 传统 Suite/Case 批量注册方式（保持向后兼容）
#define RT_SUITE(SuiteName) \
    static void SuiteName##_register(RTTestCore::Suite& suite); \
    static int SuiteName##_autoreg = [](){ \
        auto& _s = RTTestCore::Registry::instance().get_or_create(#SuiteName); \
        SuiteName##_register(_s); \
        return 0; \
    }(); \
    static void SuiteName##_register(RTTestCore::Suite& suite)

#define RT_CASE(SuiteName, CaseName) suite.add(#CaseName, [](){
#define RT_END_CASE }, std::vector<std::string>{}, __FILE__, __LINE__);

// 断言宏（防重复求值）
#define RT_EXPECT_TRUE(cond) do{ auto _c=(cond); if(!_c) throw std::runtime_error(std::string("EXPECT_TRUE failed: ")+ #cond); }while(0)
#define RT_EXPECT_EQ(a,b) do{ auto _ea=(a); auto _eb=(b); if(!(_ea==_eb)) throw std::runtime_error("EXPECT_EQ failed"); }while(0)
#define RT_EXPECT_NEAR(a,b,eps) do{ auto _ea=(a); auto _eb=(b); auto _e=(eps); if(std::fabs(_ea-_eb)>_e) throw std::runtime_error("EXPECT_NEAR failed"); }while(0)

} // namespace RTTestCore
