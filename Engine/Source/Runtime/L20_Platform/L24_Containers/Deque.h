#ifndef PLATFORM_CONTAINERS_DEQUE_H
#define PLATFORM_CONTAINERS_DEQUE_H

#include "L0_Macro/Include.h"
#include "L20_Platform/L23_MemoryManager/Memory.h"
#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"
#include <type_traits>
#include <initializer_list>

namespace ReiToEngine {

template <typename T>
class Deque {
public:
    using value_type = T; using size_type = u64; using difference_type = i64;

    Deque() = default;
    ~Deque(){ destroy_and_free(); }
    Deque(std::initializer_list<T> init){ reserve(static_cast<size_type>(init.size())); for(const auto& v:init) push_back(v); }
    Deque(const Deque& o){ copy_from(o); }
    Deque& operator=(const Deque& o){ if(this!=&o){ destroy_and_free(); copy_from(o);} return *this; }
    Deque(Deque&& o) noexcept { move_from(o); }
    Deque& operator=(Deque&& o) noexcept { if(this!=&o){ destroy_and_free(); move_from(o);} return *this; }

    // 容量
    [[nodiscard]] size_type size() const noexcept { return _size; }
    [[nodiscard]] b8 empty() const noexcept { return _size==0; }
    [[nodiscard]] size_type capacity() const noexcept { return _cap; }
    void clear() noexcept { destroy_range(0,_size); _size=0; _head=0; _tail=0; }
    void reserve(size_type new_cap){ if(new_cap>_cap) reallocate(grow_to_fit(new_cap)); }

    // 访问
    T& operator[](size_type i){ return _data[( _head + i) & mask()]; }
    const T& operator[](size_type i) const { return _data[( _head + i) & mask()]; }
    T& front(){ return _data[_head]; }
    const T& front() const { return _data[_head]; }
    T& back(){ return _data[(_tail - 1) & mask()]; }
    const T& back() const { return _data[(_tail - 1) & mask()]; }

    // 修改
    void push_back(const T& v){ emplace_back(v); }
    void push_back(T&& v){ emplace_back(static_cast<T&&>(v)); }
    template<typename... Args>
    T& emplace_back(Args&&... args){ ensure_capacity_for_insert(); T* slot=&_data[_tail]; ::new ((void*)slot) T(static_cast<Args&&>(args)...); _tail=( _tail + 1) & mask(); ++_size; return *slot; }

    void push_front(const T& v){ emplace_front(v); }
    void push_front(T&& v){ emplace_front(static_cast<T&&>(v)); }
    template<typename... Args>
    T& emplace_front(Args&&... args){ ensure_capacity_for_insert(); _head=( _head - 1) & mask(); T* slot=&_data[_head]; ::new ((void*)slot) T(static_cast<Args&&>(args)...); ++_size; return *slot; }

    void pop_back(){ if(_size==0) return; _tail=( _tail - 1) & mask(); _data[_tail].~T(); --_size; }
    void pop_front(){ if(_size==0) return; _data[_head].~T(); _head=( _head + 1) & mask(); --_size; }

    // 迭代器（随机访问迭代器）
    class iterator {
    public:
        using difference_type = i64; using value_type = T; using pointer = T*; using reference = T&;
        iterator():_dq(nullptr),_i(0){}
        iterator(Deque* d,size_type i):_dq(d),_i(i){}
        reference operator*() const { return (*_dq)[_i]; }
        pointer operator->() const { return &(*_dq)[_i]; }
        iterator& operator++(){ ++_i; return *this; }
        iterator operator++(int){ iterator t=*this; ++*this; return t; }
        iterator& operator--(){ --_i; return *this; }
        iterator operator--(int){ iterator t=*this; --*this; return t; }
        iterator& operator+=(difference_type n){ _i+=n; return *this; }
        iterator& operator-=(difference_type n){ _i-=n; return *this; }
        iterator operator+(difference_type n) const { return iterator(_dq,_i+n); }
        iterator operator-(difference_type n) const { return iterator(_dq,_i-n); }
        difference_type operator-(const iterator& o) const { return static_cast<difference_type>(_i)-static_cast<difference_type>(o._i); }
        b8 operator==(const iterator& o) const { return _dq==o._dq && _i==o._i; }
        b8 operator!=(const iterator& o) const { return !(*this==o); }
    private:
        Deque* _dq; size_type _i; friend class Deque;
    };
    class const_iterator {
    public:
        using difference_type = i64; using value_type = T; using pointer = const T*; using reference = const T&;
        const_iterator():_dq(nullptr),_i(0){}
        const_iterator(const Deque* d,size_type i):_dq(d),_i(i){}
        reference operator*() const { return (*_dq)[_i]; }
        pointer operator->() const { return &(*_dq)[_i]; }
        const_iterator& operator++(){ ++_i; return *this; }
        const_iterator operator++(int){ const_iterator t=*this; ++*this; return t; }
        const_iterator& operator--(){ --_i; return *this; }
        const_iterator operator--(int){ const_iterator t=*this; --*this; return t; }
        const_iterator& operator+=(difference_type n){ _i+=n; return *this; }
        const_iterator& operator-=(difference_type n){ _i-=n; return *this; }
        const_iterator operator+(difference_type n) const { return const_iterator(_dq,_i+n); }
        const_iterator operator-(difference_type n) const { return const_iterator(_dq,_i-n); }
        difference_type operator-(const const_iterator& o) const { return static_cast<difference_type>(_i)-static_cast<difference_type>(o._i); }
        b8 operator==(const const_iterator& o) const { return _dq==o._dq && _i==o._i; }
        b8 operator!=(const const_iterator& o) const { return !(*this==o); }
    private:
        const Deque* _dq; size_type _i; friend class Deque;
    };

    iterator begin() noexcept { return iterator(this,0); }
    iterator end() noexcept { return iterator(this,_size); }
    const_iterator begin() const noexcept { return const_iterator(this,0); }
    const_iterator end() const noexcept { return const_iterator(this,_size); }
    const_iterator cbegin() const noexcept { return const_iterator(this,0); }
    const_iterator cend() const noexcept { return const_iterator(this,_size); }

private:
    static size_type round_up_pow2(size_type x){ if(x<4) return 4; --x; x|=x>>1; x|=x>>2; x|=x>>4; x|=x>>8; x|=x>>16; if constexpr(sizeof(size_type)==8) x|=x>>32; return ++x; }
    size_type mask() const { return _cap?(_cap-1):0; }
    void ensure_capacity_for_insert(){ if(_cap==0 || (_size+1)>_cap) reallocate(_cap?(_cap*2):8); }

    void destroy_and_free(){ clear(); if(_data){ GetMemoryManager().Free(_data,_cap*sizeof(T),RT_MEMORY_TAG::CONTAINER); _data=nullptr; _cap=0; _head=0; _tail=0; } }
    void copy_from(const Deque& o){ if(o._cap==0){ _data=nullptr; _cap=0; _head=_tail=_size=0; return; } _cap=o._cap; _head=o._head; _tail=o._tail; _size=o._size; _data=(T*)GetMemoryManager().Allocate(_cap*sizeof(T),(u8)alignof(T),RT_MEMORY_TAG::CONTAINER); // 以逻辑顺序拷贝
        for(size_type i=0;i<_size;++i){ ::new ((void*)&_data[( _head + i) & mask()]) T(o[i]); } }
    void move_from(Deque& o) noexcept { _data=o._data; _cap=o._cap; _head=o._head; _tail=o._tail; _size=o._size; o._data=nullptr; o._cap=o._head=o._tail=o._size=0; }

    void destroy_range(size_type b, size_type e){ for(size_type i=b;i<e;++i){ size_type idx=( _head + i) & mask(); if constexpr(!std::is_trivially_destructible_v<T>) _data[idx].~T(); } }

    size_type grow_to_fit(size_type need){ size_type cap=_cap?_cap:8; while(cap<need) cap*=2; return cap; }

    void reallocate(size_type new_cap){
        new_cap = round_up_pow2(new_cap);
        T* nm = (T*)GetMemoryManager().Allocate(new_cap*sizeof(T),(u8)alignof(T),RT_MEMORY_TAG::CONTAINER);
        // 以逻辑顺序搬迁到新缓冲（从 head 开始连续布局）
        const size_type old_size = _size;
        const size_type old_cap  = _cap;
        const size_type old_mask = old_cap ? (old_cap - 1) : 0;
        for(size_type i=0;i<old_size;++i){
            ::new ((void*)(nm+i)) T(static_cast<T&&>((*this)[i]));
        }
        // 析构并释放旧缓冲
        if(_data){
            if constexpr(!std::is_trivially_destructible_v<T>){
                for(size_type i=0;i<old_size;++i){
                    size_type idx = (_head + i) & old_mask;
                    _data[idx].~T();
                }
            }
            GetMemoryManager().Free(_data, old_cap*sizeof(T), RT_MEMORY_TAG::CONTAINER);
        }
        // 更新为规范化布局：head=0, tail=size
        _data = nm; _cap = new_cap; _head = 0; _size = old_size; _tail = old_size;
    }

private:
    T* _data=nullptr; size_type _cap=0; size_type _head=0; size_type _tail=0; size_type _size=0;
};

} // namespace ReiToEngine

#endif
