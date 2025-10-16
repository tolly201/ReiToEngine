#ifndef PLATFORM_CONTAINERS_DOUBLY_LINKED_LIST_H
#define PLATFORM_CONTAINERS_DOUBLY_LINKED_LIST_H

#include "L0_Macro/Include.h"
#include "L20_Platform/L23_MemoryManager/Memory.h"
#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"
#include <type_traits>
#include <initializer_list>

namespace ReiToEngine {

template <typename T>
class DoublyLinkedList {
public:
    using value_type = T; using size_type = u64;

    // 内部节点类型（在迭代器之前定义，避免前置声明的可见性/访问级别冲突）
    struct Node { Node* prev; Node* next; T v; };

    DoublyLinkedList() = default;
    ~DoublyLinkedList(){ clear(); }
    DoublyLinkedList(std::initializer_list<T> init){ for(const auto& v:init) push_back(v); }
    DoublyLinkedList(const DoublyLinkedList& o){ for(const auto& v:o) push_back(v); }
    DoublyLinkedList& operator=(const DoublyLinkedList& o){ if(this!=&o){ clear(); for(const auto& v:o) push_back(v);} return *this; }
    DoublyLinkedList(DoublyLinkedList&& o) noexcept { _head=o._head; _tail=o._tail; _size=o._size; o._head=o._tail=nullptr; o._size=0; }
    DoublyLinkedList& operator=(DoublyLinkedList&& o) noexcept { if(this!=&o){ clear(); _head=o._head; _tail=o._tail; _size=o._size; o._head=o._tail=nullptr; o._size=0; } return *this; }

    [[nodiscard]] size_type size() const noexcept { return _size; }
    [[nodiscard]] b8 empty() const noexcept { return _size==0; }

    void clear() noexcept { Node* n=_head; while(n){ Node* nx=n->next; destroy_node(n); n=nx; } _head=_tail=nullptr; _size=0; }

    // 访问
    T& front(){ return _head->v; } const T& front() const { return _head->v; }
    T& back(){ return _tail->v; } const T& back() const { return _tail->v; }

    // 修改
    void push_back(const T& v){ emplace_back(v); }
    void push_back(T&& v){ emplace_back(static_cast<T&&>(v)); }
    template<typename... Args>
    T& emplace_back(Args&&... args){ Node* n=create_node(static_cast<Args&&>(args)...); link_back(n); return n->v; }

    void push_front(const T& v){ emplace_front(v); }
    void push_front(T&& v){ emplace_front(static_cast<T&&>(v)); }
    template<typename... Args>
    T& emplace_front(Args&&... args){ Node* n=create_node(static_cast<Args&&>(args)...); link_front(n); return n->v; }

    void pop_back(){ if(!_tail) return; Node* n=_tail; unlink(n); destroy_node(n); }
    void pop_front(){ if(!_head) return; Node* n=_head; unlink(n); destroy_node(n); }

    // 迭代器（双向）
    class iterator {
    public:
        iterator():_node(nullptr){}
        explicit iterator(void* p):_node((Node*)p){}
        T& operator*() const { return ((Node*)_node)->v; }
        T* operator->() const { return &((Node*)_node)->v; }
        iterator& operator++(){ _node=((Node*)_node)->next; return *this; }
        iterator operator++(int){ iterator t=*this; ++*this; return t; }
        iterator& operator--(){ _node=((Node*)_node)->prev; return *this; }
        iterator operator--(int){ iterator t=*this; --*this; return t; }
        b8 operator==(const iterator& o) const { return _node==o._node; }
        b8 operator!=(const iterator& o) const { return !(*this==o); }
    private: void* _node; friend class DoublyLinkedList; };

    class const_iterator {
    public:
        const_iterator():_node(nullptr){}
        explicit const_iterator(const void* p):_node((const Node*)p){}
        const T& operator*() const { return ((const Node*)_node)->v; }
        const T* operator->() const { return &((const Node*)_node)->v; }
        const_iterator& operator++(){ _node=((const Node*)_node)->next; return *this; }
        const_iterator operator++(int){ const_iterator t=*this; ++*this; return t; }
        const_iterator& operator--(){ _node=((const Node*)_node)->prev; return *this; }
        const_iterator operator--(int){ const_iterator t=*this; --*this; return t; }
        b8 operator==(const const_iterator& o) const { return _node==o._node; }
        b8 operator!=(const const_iterator& o) const { return !(*this==o); }
    private: const Node* _node; friend class DoublyLinkedList; };

    iterator begin() noexcept { return iterator(_head); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator begin() const noexcept { return const_iterator(_head); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }

    // 插入/删除 by 迭代器
    iterator insert(iterator pos, const T& v){ Node* n=create_node(v); link_before(pos._node,n); return iterator(n); }
    iterator insert(iterator pos, T&& v){ Node* n=create_node(static_cast<T&&>(v)); link_before(pos._node,n); return iterator(n); }
    iterator erase(iterator pos){ if(pos==end()) return pos; Node* n=(Node*)pos._node; iterator it((void*)n->next); unlink(n); destroy_node(n); return it; }

private:
    // 节点已在 public 区域定义

    template<typename... Args>
    Node* create_node(Args&&... args){ Node* n=(Node*)GetMemoryManager().Allocate(sizeof(Node),(u8)alignof(Node),RT_MEMORY_TAG::CONTAINER); n->prev=nullptr; n->next=nullptr; ::new ((void*)&n->v) T(static_cast<Args&&>(args)...); ++_size; return n; }
    void destroy_node(Node* n){ if(!n) return; if constexpr(!std::is_trivially_destructible_v<T>) n->v.~T(); GetMemoryManager().Free(n,sizeof(Node),RT_MEMORY_TAG::CONTAINER); --_size; }

    void link_back(Node* n){ n->prev=_tail; n->next=nullptr; if(_tail) _tail->next=n; else _head=n; _tail=n; }
    void link_front(Node* n){ n->next=_head; n->prev=nullptr; if(_head) _head->prev=n; else _tail=n; _head=n; }
    void link_before(void* pos_node, Node* n){ if(!pos_node){ link_back(n); return; } Node* p=(Node*)pos_node; n->next=p; n->prev=p->prev; if(p->prev) p->prev->next=n; else _head=n; p->prev=n; }
    void unlink(Node* n){ if(n->prev) n->prev->next=n->next; else _head=n->next; if(n->next) n->next->prev=n->prev; else _tail=n->prev; }

private:
    Node* _head=nullptr; Node* _tail=nullptr; size_type _size=0;
};

} // namespace ReiToEngine

#endif
