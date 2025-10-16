#include "../Core/TestCore.h"
#include "L20_Platform/L24_Containers/Deque.h"
#include "L20_Platform/L24_Containers/List.h"
#include <string>

using namespace ReiToEngine;

RT_TEST_ST("Containers", DequeBasicOps, "container","deque"){
    Deque<int> dq;
    RT_EXPECT_TRUE(dq.empty());
    for(int i=0;i<10;++i) dq.push_back(i);
    RT_EXPECT_TRUE(dq.size()==10);
    RT_EXPECT_TRUE(dq.front()==0 && dq.back()==9);
    dq.pop_front(); // remove 0
    dq.pop_back();  // remove 9
    RT_EXPECT_TRUE(dq.size()==8);
    // 迭代验证顺序（逻辑顺序）
    int expect=1; for(auto it=dq.begin(); it!=dq.end(); ++it,++expect){ RT_EXPECT_TRUE(*it==expect); }
}

RT_TEST_ST("Containers", DequeWrapAndGrow, "container","deque","growth"){
    Deque<int> dq;
    // 制造前后 push_front/back 混合触发 head/tail wrap
    for(int i=0;i<8;++i) dq.push_back(i); // 初始容量可能 8
    for(int i=0;i<4;++i) dq.push_front(-1-i); // 触发环绕或扩容
    for(int i=8;i<20;++i) dq.push_back(i);
    // 检验内容：不严格验证内部布局，只验证 size 和一些位置值
    RT_EXPECT_TRUE(dq.size()== (8 + 4 + (20-8)) ); // 8 + 4 + 12 = 24
    RT_EXPECT_TRUE(dq.front()== -4);
    RT_EXPECT_TRUE(dq.back()==19);
}

RT_TEST_ST("Containers", ListPushAndIter, "container","list"){
    List<std::string> ls;
    ls.push_back("a"); ls.push_back("b"); ls.push_back("c");
    RT_EXPECT_TRUE(ls.size()==3 && ls.capacity()>=3);
    std::string concat; for(auto& s: ls) concat += s;
    RT_EXPECT_TRUE(concat=="abc");
}

RT_TEST_ST("Containers", ListEraseUnstable, "container","list","erase"){
    List<int> ls; for(int i=0;i<6;++i) ls.push_back(i); // [0 1 2 3 4 5]
    ls.erase_unstable(2); // remove value 2, move last(5) to pos2
    RT_EXPECT_TRUE(ls.size()==5);
    // 验证：位置2 现在是 5，且不存在重复 5 两次（最后一个已被搬迁覆盖）
    int count5=0; for(u64 i=0;i<ls.size();++i){ if(ls[i]==5) ++count5; }
    RT_EXPECT_TRUE(ls[2]==5 && count5==1);
}
