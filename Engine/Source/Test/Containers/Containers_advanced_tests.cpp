#include "../Core/TestCore.h"
#include "L20_Platform/L24_Containers/UnorderedMap.h"
#include "L20_Platform/L24_Containers/Deque.h"
#include "L20_Platform/L24_Containers/Set.h"
#include <string>

using namespace ReiToEngine;

// UnorderedMap rehash growth: 插入超过当前 bucket 负载，验证元素仍可访问
RT_TEST_ST("Containers", UnorderedMapRehashGrowth, "container","umap","advanced"){
    UnorderedMap<int,int> um;
    // 预估：插入较多元素触发多次扩容 (假设内部使用负载因子策略)
    const int N=512;
    for(int i=0;i<N;++i) um.emplace(i, i*i);
    RT_EXPECT_TRUE(um.size()==(u64)N);
    // 访问若干散列点
    for(int k: {0,1,7,63,128,255,511}){ RT_EXPECT_TRUE(um.contains(k)); RT_EXPECT_TRUE(um.at(k)==k*k); }
}

// Deque wrap stress: 前后交替 push/pop 触发多次环绕，保持逻辑顺序正确
RT_TEST_ST("Containers", DequeWrapStress, "container","deque","advanced"){
    Deque<int> dq;
    // 先 push_back 一批
    for(int i=0;i<50;++i) dq.push_back(i);
    // 交替 push_front / pop_back
    for(int r=0;r<40;++r){
        dq.push_front(-r-1);
        dq.pop_back();
    }
    // 此时队列前面有 -40..-1，后面 0..9 (共 50) 减去 40 pop_back => 10 元素, plus 40 front => size=50 again
    RT_EXPECT_TRUE(dq.size()==50);
    int expect=-40; int count=0;
    for(auto it=dq.begin(); it!=dq.end(); ++it,++count){
        if(count<40){ RT_EXPECT_TRUE(*it==expect); ++expect; }
        else { int idx=count-40; RT_EXPECT_TRUE(*it==idx); }
    }
}

// Set duplicate insertion: 多次插入同值不增长，删除后可重新插入
RT_TEST_ST("Containers", SetDuplicateInsertBehavior, "container","set","advanced"){
    Set<int> s; s.insert(42); s.insert(42); s.insert(42);
    RT_EXPECT_TRUE(s.size()==1);
    s.erase(42); RT_EXPECT_TRUE(s.size()==0);
    s.insert(42); RT_EXPECT_TRUE(s.size()==1);
}
