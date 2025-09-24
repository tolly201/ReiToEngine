// 增强版启动器：支持过滤参数
#include "TestCore.h"
#include <iostream>
#include <vector>
#include <string>

// 参数格式：
//  --list / -l                  列出所有
//  suite=Name                   精确匹配一个 Suite，可多次
//  tag=Perf                     匹配含该标签的 case，可多次（或逻辑）
//  case=subStr                  Case 名包含该子串（与其他条件 AND）
//  (无参数)                     运行全部

int main(int argc, char** argv){
    using namespace RTTestCore;
    Filters f; bool list=false;
    for(int i=1;i<argc;++i){
        std::string a=argv[i];
        if(a=="--list"||a=="-l"){ list=true; continue; }
        // 兼容形式： --tag=abc  / tag=abc  / suite=Name / case=sub / 直接写 suiteName
        // 同时支持 tag=a,b,c 逗号分隔（视为多 tag OR）
        auto pos=a.find('=');
        if(pos==std::string::npos){ /* 向后兼容旧用法：视为 suite 过滤 */ f.suiteNames.push_back(a); continue; }
        std::string key=a.substr(0,pos); std::string val=a.substr(pos+1);
        if(key=="--tag") key="tag"; // 容错：用户可能输入 --tag=xxx
        auto push_split = [&](const std::string& v, std::vector<std::string>& dst){
            size_t start=0; while(start<=v.size()){ size_t comma=v.find(',', start); if(comma==std::string::npos){ auto sub=v.substr(start); if(!sub.empty()) dst.push_back(sub); break; } auto sub=v.substr(start, comma-start); if(!sub.empty()) dst.push_back(sub); start=comma+1; }
        };
        if(key=="suite") f.suiteNames.push_back(val); else if(key=="tag") push_split(val, f.tagIncludes); else if(key=="case") f.caseSubstr.push_back(val); else {
            std::cout << "[WARN] Unknown filter key: " << key << "\n"; }
    }
    if(list){
        std::cout << "Available suites/cases (with tags):\n";
        for(auto& s: Registry::instance().suites()){
            std::cout << "- " << s->name() << "\n";
            for(auto& c: s->cases()){
                std::cout << "  * " << c.name;
                if(!c.tags.empty()){ std::cout << " ["; for(size_t i=0;i<c.tags.size();++i){ if(i) std::cout << ","; std::cout << c.tags[i]; } std::cout << "]"; }
                std::cout << "\n";
            }
        }
        return 0;
    }
    auto results = run_all(f);
    return summarize_and_exit(results);
}
