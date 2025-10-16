#ifndef PLATFORM_CONTAINERS_MAP_H
#define PLATFORM_CONTAINERS_MAP_H

#include "L0_Macro/Include.h"
#include "L20_Platform/L23_MemoryManager/Memory.h"
#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"
#include <initializer_list>
#include <utility>
#include <type_traits>
#include <stdexcept>

namespace ReiToEngine {

// 红黑树有序关联容器：std::map 常用接口/语义
template <typename Key, typename T, typename Compare = void>
class Map {
public:
    struct value_type { Key first; T second; };
    using size_type = u64;

private:
    enum Color : u8 { RED=0, BLACK=1 };
    struct Node {
        Node* parent; Node* left; Node* right; Color color;
        value_type kv;
    };
    struct DefaultLess { b8 operator()(const Key& a, const Key& b) const { return a < b; } };
    using KeyCompare = std::conditional_t<std::is_void_v<Compare>, DefaultLess, Compare>;

public:
    class iterator {
    public:
        iterator():_map(nullptr),_node(nullptr){}
        iterator(Map* m, Node* n):_map(m),_node(n){}
        value_type& operator*() const { return _node->kv; }
        value_type* operator->() const { return &_node->kv; }
        iterator& operator++(){ _node = _map->successor(_node); return *this; }
        b8 operator==(const iterator& o) const { return _node==o._node; }
        b8 operator!=(const iterator& o) const { return !(*this==o); }
    private: Map* _map; Node* _node; friend class Map; };

    class const_iterator {
    public:
        const_iterator():_map(nullptr),_node(nullptr){}
        const_iterator(const Map* m, const Node* n):_map(m),_node(const_cast<Node*>(n)){}
        const value_type& operator*() const { return _node->kv; }
        const value_type* operator->() const { return &_node->kv; }
        const_iterator& operator++(){ _node = _map->successor(_node); return *this; }
        b8 operator==(const const_iterator& o) const { return _node==o._node; }
        b8 operator!=(const const_iterator& o) const { return !(*this==o); }
    private: const Map* _map; Node* _node; friend class Map; };

    Map() = default;
    ~Map(){ destroy_and_free(); }
    Map(std::initializer_list<std::pair<Key, T>> init){ for(const auto& kv : init) emplace(kv.first, kv.second); }
    Map(const Map& o){ copy_from(o); }
    Map& operator=(const Map& o){ if(this!=&o){ destroy_and_free(); copy_from(o);} return *this; }
    Map(Map&& o) noexcept { move_from(o); }
    Map& operator=(Map&& o) noexcept { if(this!=&o){ destroy_and_free(); move_from(o);} return *this; }

    [[nodiscard]] size_type size() const noexcept { return _size; }
    [[nodiscard]] b8 empty() const noexcept { return _size==0; }
    void clear() noexcept { destroy_nodes(_root); _root=nullptr; _size=0; }

    iterator begin() noexcept { return iterator(this, minimum(_root)); }
    iterator end()   noexcept { return iterator(this, nullptr); }
    const_iterator begin() const noexcept { return const_iterator(this, minimum(_root)); }
    const_iterator end()   const noexcept { return const_iterator(this, nullptr); }
    const_iterator cbegin() const noexcept { return const_iterator(this, minimum(_root)); }
    const_iterator cend()   const noexcept { return const_iterator(this, nullptr); }

    iterator find(const Key& key) noexcept { return iterator(this, find_node(key)); }
    const_iterator find(const Key& key) const noexcept { return const_iterator(this, find_node(key)); }
    b8 contains(const Key& key) const noexcept { return find_node(key)!=nullptr; }
    size_type count(const Key& key) const noexcept { return contains(key)?1:0; }

    // 有序操作
    iterator lower_bound(const Key& key) noexcept { return iterator(this, lower_bound_node(key)); }
    iterator upper_bound(const Key& key) noexcept { return iterator(this, upper_bound_node(key)); }

    // 访问
    T& operator[](const Key& key){ auto [it,ok] = emplace(key, T()); (void)ok; return (*it).second; }
    T& at(const Key& key){ Node* n=find_node(key); if(n) return n->kv.second; throw std::out_of_range("Map::at: key not found"); }
    const T& at(const Key& key) const { Node* n=find_node(key); if(n) return n->kv.second; throw std::out_of_range("Map::at: key not found"); }

    // 插入
    std::pair<iterator,bool> insert(const value_type& v){ return emplace(v.first, v.second); }
    std::pair<iterator,bool> insert(value_type&& v){ return emplace(static_cast<Key&&>(v.first), static_cast<T&&>(v.second)); }
    std::pair<iterator,bool> insert(const Key& k, const T& t){ return emplace(k,t); }

    template<typename... Args>
    std::pair<iterator,bool> emplace(const Key& key, Args&&... args){
        Node* y=nullptr; Node* x=_root; KeyCompare cmp{};
        while(x){
            y=x;
            if (!cmp(x->kv.first, key) && !cmp(key, x->kv.first)) return { iterator(this,x), false }; // equal
            x = cmp(key, x->kv.first) ? x->left : x->right;
        }
        Node* z = create_node(key, static_cast<Args&&>(args)...);
        z->parent = y;
        if(!y) _root=z;
        else if (KeyCompare{}(z->kv.first, y->kv.first)) y->left=z; else y->right=z;
        insert_fixup(z);
        ++_size;
        return { iterator(this,z), true };
    }

    // 删除
    size_type erase(const Key& key){ Node* z=find_node(key); if(!z) return 0; erase_node(z); return 1; }
    iterator erase(iterator pos){ if(pos==end()) return pos; Node* n=pos._node; Node* nxt = successor(n); erase_node(n); return iterator(this,nxt); }

private:
    // 工具：比较与等价
    static b8 less_than(const Key& a, const Key& b){ return KeyCompare{}(a,b); }
    static b8 equal_key(const Key& a, const Key& b){ return !less_than(a,b) && !less_than(b,a); }

    // 节点与内存
    template<typename... Args>
    Node* create_node(const Key& k, Args&&... args){
        Node* n = (Node*)GetMemoryManager().Allocate(sizeof(Node), (u8)alignof(Node), RT_MEMORY_TAG::CONTAINER);
        n->parent=n->left=n->right=nullptr; n->color=RED;
        ::new ((void*)&n->kv.first) Key(k);
        ::new ((void*)&n->kv.second) T(static_cast<Args&&>(args)...);
        return n;
    }
    void destroy_node(Node* n){
        if(!n) return;
        if constexpr(!std::is_trivially_destructible_v<Key>) n->kv.first.~Key();
        if constexpr(!std::is_trivially_destructible_v<T>)   n->kv.second.~T();
        GetMemoryManager().Free(n, sizeof(Node), RT_MEMORY_TAG::CONTAINER);
    }
    void destroy_nodes(Node* n){ if(!n) return; destroy_nodes(n->left); destroy_nodes(n->right); destroy_node(n); }
    void destroy_and_free(){ destroy_nodes(_root); _root=nullptr; _size=0; }

    void copy_from(const Map& o){ for(const auto& kv : o){ emplace(kv.first, kv.second); } }
    void move_from(Map& o) noexcept { _root=o._root; _size=o._size; o._root=nullptr; o._size=0; }

    // 树操作
    static Color color_of(Node* n){ return n? n->color : BLACK; }
    static Node* minimum(Node* n){ if(!n) return nullptr; while(n->left) n=n->left; return n; }
    static Node* maximum(Node* n){ if(!n) return nullptr; while(n->right) n=n->right; return n; }
    Node* successor(Node* n) const { if(!n) return nullptr; if(n->right) return minimum(n->right); Node* p=n->parent; while(p && n==p->right){ n=p; p=p->parent; } return p; }

    void left_rotate(Node* x){ Node* y=x->right; x->right=y->left; if(y->left) y->left->parent=x; y->parent=x->parent; if(!x->parent) _root=y; else if(x==x->parent->left) x->parent->left=y; else x->parent->right=y; y->left=x; x->parent=y; }
    void right_rotate(Node* x){ Node* y=x->left; x->left=y->right; if(y->right) y->right->parent=x; y->parent=x->parent; if(!x->parent) _root=y; else if(x==x->parent->right) x->parent->right=y; else x->parent->left=y; y->right=x; x->parent=y; }

    void insert_fixup(Node* z){
        while(z->parent && z->parent->color==RED){
            Node* p = z->parent; Node* g = p->parent;
            if(p==g->left){
                Node* y = g->right; // 叔
                if(color_of(y)==RED){ p->color=BLACK; y->color=BLACK; g->color=RED; z=g; }
                else {
                    if(z==p->right){ z=p; left_rotate(z); p=z->parent; g=p->parent; }
                    p->color=BLACK; g->color=RED; right_rotate(g);
                }
            } else {
                Node* y = g->left;
                if(color_of(y)==RED){ p->color=BLACK; y->color=BLACK; g->color=RED; z=g; }
                else {
                    if(z==p->left){ z=p; right_rotate(z); p=z->parent; g=p->parent; }
                    p->color=BLACK; g->color=RED; left_rotate(g);
                }
            }
        }
        _root->color=BLACK;
    }

    void transplant(Node* u, Node* v){ if(!u->parent) _root=v; else if(u==u->parent->left) u->parent->left=v; else u->parent->right=v; if(v) v->parent=u->parent; }

    Node* find_node(const Key& key) const {
        Node* cur=_root; KeyCompare cmp{};
        while(cur){ if(!cmp(cur->kv.first,key) && !cmp(key,cur->kv.first)) return cur; cur = cmp(key, cur->kv.first) ? cur->left : cur->right; }
        return nullptr;
    }

    Node* lower_bound_node(const Key& key) const {
        Node* cur=_root; Node* res=nullptr; KeyCompare cmp{};
        while(cur){ if(!cmp(cur->kv.first,key)){ res=cur; cur=cur->left; } else { cur=cur->right; } }
        return res;
    }
    Node* upper_bound_node(const Key& key) const {
        Node* cur=_root; Node* res=nullptr; KeyCompare cmp{};
        while(cur){ if(cmp(key, cur->kv.first)){ res=cur; cur=cur->left; } else { cur=cur->right; } }
        return res;
    }

    void erase_node(Node* z){
        Node* y = z; Color y_orig = y->color; Node* x = nullptr; Node* x_parent = nullptr;
        if(!z->left){ x = z->right; x_parent = z->parent; transplant(z, z->right); }
        else if(!z->right){ x = z->left; x_parent = z->parent; transplant(z, z->left); }
        else {
            y = minimum(z->right); y_orig = y->color; x = y->right; x_parent = y->parent;
            if(y->parent == z){ if(x) x->parent = y; x_parent = y; }
            else { transplant(y, y->right); y->right = z->right; y->right->parent = y; }
            transplant(z, y); y->left = z->left; y->left->parent = y; y->color = z->color;
        }
        // 析构节点 z
        destroy_node(z); --_size;
        if(y_orig == BLACK) delete_fixup(x, x_parent);
    }

    void delete_fixup(Node* x, Node* x_parent){
        while((x != _root) && color_of(x)==BLACK){
            if(x == (x_parent? x_parent->left : nullptr)){
                Node* w = x_parent? x_parent->right : nullptr;
                if(color_of(w)==RED){ w->color=BLACK; x_parent->color=RED; left_rotate(x_parent); w=x_parent->right; }
                if(color_of(w?w->left:nullptr)==BLACK && color_of(w?w->right:nullptr)==BLACK){ if(w) w->color=RED; x = x_parent; x_parent = x_parent? x_parent->parent : nullptr; }
                else {
                    if(color_of(w?w->right:nullptr)==BLACK){ if(w && w->left) w->left->color=BLACK; if(w) w->color=RED; right_rotate(w); w=x_parent? x_parent->right:nullptr; }
                    if(w) w->color = color_of(x_parent);
                    if(x_parent) x_parent->color=BLACK;
                    if(w && w->right) w->right->color=BLACK;
                    left_rotate(x_parent);
                    x = _root; x_parent=nullptr;
                }
            } else {
                Node* w = x_parent? x_parent->left : nullptr;
                if(color_of(w)==RED){ w->color=BLACK; x_parent->color=RED; right_rotate(x_parent); w=x_parent->left; }
                if(color_of(w?w->right:nullptr)==BLACK && color_of(w?w->left:nullptr)==BLACK){ if(w) w->color=RED; x = x_parent; x_parent = x_parent? x_parent->parent : nullptr; }
                else {
                    if(color_of(w?w->left:nullptr)==BLACK){ if(w && w->right) w->right->color=BLACK; if(w) w->color=RED; left_rotate(w); w=x_parent? x_parent->left:nullptr; }
                    if(w) w->color = color_of(x_parent);
                    if(x_parent) x_parent->color=BLACK;
                    if(w && w->left) w->left->color=BLACK;
                    right_rotate(x_parent);
                    x = _root; x_parent=nullptr;
                }
            }
        }
        if(x) x->color=BLACK;
    }

private:
    Node* _root = nullptr;
    size_type _size = 0;
};

} // namespace ReiToEngine

#endif
