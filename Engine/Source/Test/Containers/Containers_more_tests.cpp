#include "../Core/TestCore.h"
#include "L20_Platform/L32_Containers/Array.h"
#include "L20_Platform/L32_Containers/Stack.h"
#include "L20_Platform/L32_Containers/Set.h"
#include "L20_Platform/L32_Containers/Map.h"
#include "L20_Platform/L32_Containers/UnorderedMap.h"
#include "L20_Platform/L32_Containers/UnorderedSet.h"
#include "L20_Platform/L32_Containers/DoublyLinkedList.h"
#include <string>
#include <vector>

using namespace ReiToEngine;

RT_TEST_ST("Containers", ArrayFillAndSwap, "container","array"){
    Array<int,5> a{}; a.fill(7); for(auto v:a) RT_EXPECT_TRUE(v==7);
    Array<int,5> b{}; for(int i=0;i<5;++i) b[i]=i;
    a.swap(b);
    for(int i=0;i<5;++i) RT_EXPECT_TRUE(a[i]==i);
}

RT_TEST_ST("Containers", StackPushPopTop, "container","stack"){
    Stack<std::string> st; RT_EXPECT_TRUE(st.empty());
    st.push("a"); st.emplace(2,'b'); // "bb"
    RT_EXPECT_TRUE(st.size()==2);
    RT_EXPECT_TRUE(st.top()=="bb");
    st.pop(); RT_EXPECT_TRUE(st.top()=="a");
}

RT_TEST_ST("Containers", SetInsertFindErase, "container","set"){
    Set<int> s; for(int i=0;i<10;++i) s.insert(i);
    RT_EXPECT_TRUE(s.size()==10);
    RT_EXPECT_TRUE(s.contains(3));
    s.erase(3); RT_EXPECT_TRUE(!s.contains(3) && s.size()==9);
}

RT_TEST_ST("Containers", MapInsertAccessErase, "container","map"){
    Map<std::string,int> m; m.emplace("a",1); m["b"]=2; m.emplace("c",3);
    RT_EXPECT_TRUE(m.size()==3);
    RT_EXPECT_TRUE(m.contains("b"));
    RT_EXPECT_TRUE(m.at("a")==1);
    m.erase("b"); RT_EXPECT_TRUE(!m.contains("b") && m.size()==2);
}

RT_TEST_ST("Containers", UnorderedMapBasicOps, "container","umap"){
    UnorderedMap<int,std::string> um; for(int i=0;i<32;++i) um.emplace(i, std::to_string(i));
    RT_EXPECT_TRUE(um.size()==32);
    RT_EXPECT_TRUE(um.contains(7));
    um.erase(7); RT_EXPECT_TRUE(!um.contains(7));
}

RT_TEST_ST("Containers", UnorderedSetInsertErase, "container","uset"){
    UnorderedSet<int> us; for(int i=0;i<32;++i) us.insert(i);
    RT_EXPECT_TRUE(us.size()==32);
    RT_EXPECT_TRUE(us.contains(15));
    us.erase(15); RT_EXPECT_TRUE(!us.contains(15));
}

RT_TEST_ST("Containers", DListPushPopIter, "container","dlist"){
    DoublyLinkedList<int> dl; for(int i=0;i<5;++i) dl.push_back(i);
    RT_EXPECT_TRUE(dl.size()==5);
    int expect=0; for(auto it=dl.begin(); it!=dl.end(); ++it,++expect) RT_EXPECT_TRUE(*it==expect);
    dl.pop_front(); dl.pop_back(); // remove 0 and 4
    RT_EXPECT_TRUE(dl.size()==3 && dl.front()==1 && dl.back()==3);
}
