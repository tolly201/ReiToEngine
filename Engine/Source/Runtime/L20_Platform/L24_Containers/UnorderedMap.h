#ifndef PLATFORM_CONTAINERS_UNORDERED_MAP_H
#define PLATFORM_CONTAINERS_UNORDERED_MAP_H

#include "L0_Macro/Include.h"
#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"
#include "L20_Platform/L23_MemoryManager/Memory.h"
#include <initializer_list>
#include <utility>
#include <type_traits>
#include <stdexcept>

namespace ReiToEngine {

// 开放寻址哈希表（线性探测）：提供与 std::unordered_map 的常用接口命名/语义一致。
template <typename Key, typename T, typename Hash = void>
class UnorderedMap {
public:
    struct value_type { Key first; T second; };
    using size_type = u64;

private:
    struct DefaultHashIntPtr {
        static u64 mix64(u64 x){ x^=x>>33; x*=0xff51afd7ed558ccdULL; x^=x>>33; x*=0xc4ceb9fe1a85ec53ULL; x^=x>>33; return x; }
        u64 operator()(u64 v) const { return mix64(v); }
        u64 operator()(i64 v) const { return mix64(static_cast<u64>(v)); }
        u64 operator()(u32 v) const { return mix64(v); }
        u64 operator()(i32 v) const { return mix64(static_cast<u64>(static_cast<u32>(v))); }
        u64 operator()(const void* p) const { return mix64(reinterpret_cast<u64>(p)); }
        u64 operator()(const char* s) const { if(!s) return 0; const u64 O=1469598103934665603ULL,P=1099511628211ULL; u64 h=O; while(*s){ h^=(u8)*s++; h*=P; } return mix64(h);} };
    using Hasher = std::conditional_t<std::is_void_v<Hash>, DefaultHashIntPtr, Hash>;
    enum SlotState : u8 { EMPTY=0, OCCUPIED=1, TOMBSTONE=2 };
    struct Slot { value_type kv; SlotState state; };
    static constexpr f32 MAX_LOAD = 0.70f;

public:
    class iterator {
    public:
        iterator():_map(nullptr),_idx(0){}
        iterator(UnorderedMap* m,size_type i):_map(m),_idx(i){ adv(); }
        value_type& operator*() const { return _map->_slots[_idx].kv; }
        value_type* operator->() const { return &_map->_slots[_idx].kv; }
        iterator& operator++(){ ++_idx; adv(); return *this; }
        b8 operator==(const iterator& o) const { return _map==o._map && _idx==o._idx; }
        b8 operator!=(const iterator& o) const { return !(*this==o); }
    private:
        void adv(){ while(_map && _idx<_map->_cap && _map->_slots[_idx].state!=OCCUPIED) ++_idx; }
        UnorderedMap* _map; size_type _idx; friend class UnorderedMap;
    };
    class const_iterator {
    public:
        const_iterator():_map(nullptr),_idx(0){}
        const_iterator(const UnorderedMap* m,size_type i):_map(m),_idx(i){ adv(); }
        const value_type& operator*() const { return _map->_slots[_idx].kv; }
        const value_type* operator->() const { return &_map->_slots[_idx].kv; }
        const_iterator& operator++(){ ++_idx; adv(); return *this; }
        b8 operator==(const const_iterator& o) const { return _map==o._map && _idx==o._idx; }
        b8 operator!=(const const_iterator& o) const { return !(*this==o); }
    private:
        void adv(){ while(_map && _idx<_map->_cap && _map->_slots[_idx].state!=OCCUPIED) ++_idx; }
        const UnorderedMap* _map; size_type _idx; friend class UnorderedMap;
    };

    UnorderedMap() = default;
    ~UnorderedMap(){ destroy_and_free(); }
    UnorderedMap(std::initializer_list<std::pair<Key, T>> init){ reserve(static_cast<size_type>(init.size()*2)); for(const auto& kv:init) insert(kv.first, kv.second); }
    UnorderedMap(const UnorderedMap& o){ copy_from(o); }
    UnorderedMap& operator=(const UnorderedMap& o){ if(this!=&o){ destroy_and_free(); copy_from(o);} return *this; }
    UnorderedMap(UnorderedMap&& o) noexcept { move_from(o); }
    UnorderedMap& operator=(UnorderedMap&& o) noexcept { if(this!=&o){ destroy_and_free(); move_from(o);} return *this; }

    [[nodiscard]] size_type size() const noexcept { return _size; }
    [[nodiscard]] b8 empty() const noexcept { return _size==0; }
    void clear() noexcept { destroy_slots(); }
    void reserve(size_type new_cap){ rehash_to_at_least(new_cap); }

    iterator begin() noexcept { return iterator(this,0); }
    iterator end()   noexcept { return iterator(this,_cap); }
    const_iterator begin() const noexcept { return const_iterator(this,0); }
    const_iterator end()   const noexcept { return const_iterator(this,_cap); }
    const_iterator cbegin() const noexcept { return const_iterator(this,0); }
    const_iterator cend()   const noexcept { return const_iterator(this,_cap); }

    iterator find(const Key& key) noexcept { return iterator(this, find_index(key)); }
    const_iterator find(const Key& key) const noexcept { return const_iterator(this, find_index(key)); }
    b8 contains(const Key& key) const noexcept { return find_index(key)!=_cap; }
    size_type count(const Key& key) const noexcept { return contains(key)?1:0; }

    T& operator[](const Key& key){ ensure_capacity_for_insert(); size_type idx; b8 f=probe_find_or_first_empty(key,idx); if(f) return _slots[idx].kv.second; ::new ((void*)&_slots[idx].kv.first) Key(key); ::new ((void*)&_slots[idx].kv.second) T(); _slots[idx].state=OCCUPIED; ++_size; return _slots[idx].kv.second; }
    T& at(const Key& key){ size_type i=find_index(key); if(i!=_cap) return _slots[i].kv.second; throw std::out_of_range("UnorderedMap::at: key not found"); }
    const T& at(const Key& key) const { size_type i=find_index(key); if(i!=_cap) return _slots[i].kv.second; throw std::out_of_range("UnorderedMap::at: key not found"); }

    std::pair<iterator,bool> insert(const value_type& v){ return emplace(v.first, v.second); }
    std::pair<iterator,bool> insert(value_type&& v){ return emplace(static_cast<Key&&>(v.first), static_cast<T&&>(v.second)); }
    std::pair<iterator,bool> insert(const Key& k, const T& t){ return emplace(k,t); }
    template<typename... Args>
    std::pair<iterator,bool> emplace(const Key& key, Args&&... args){ ensure_capacity_for_insert(); size_type idx; b8 f=probe_find_or_first_empty(key,idx); if(f) return { iterator(this,idx), false }; ::new ((void*)&_slots[idx].kv.first) Key(key); ::new ((void*)&_slots[idx].kv.second) T(static_cast<Args&&>(args)...); _slots[idx].state=OCCUPIED; ++_size; return { iterator(this,idx), true }; }

    size_type erase(const Key& key){ size_type i=find_index(key); if(i==_cap) return 0; destroy_slot(i); _slots[i].state=TOMBSTONE; --_size; return 1; }
    iterator erase(iterator pos){ if(pos==end()) return pos; size_type i=pos._idx; destroy_slot(i); _slots[i].state=TOMBSTONE; --_size; ++i; return iterator(this,i); }

private:
    size_type mask() const { return _cap?(_cap-1):0; }
    static size_type round_up_pow2(size_type x){ if(x<4) return 4; --x; x|=x>>1; x|=x>>2; x|=x>>4; x|=x>>8; x|=x>>16; if constexpr(sizeof(size_type)==8) x|=x>>32; return ++x; }
    void ensure_capacity_for_insert(){ if(_cap==0 || (_size+1) > static_cast<size_type>(_cap*MAX_LOAD)) rehash_to_at_least(_cap?(_cap*2):8); }

    u64 hasher_key(const Key& key) const {
        Hasher h{};
        if constexpr (std::is_same_v<Hasher, DefaultHashIntPtr>) {
            if constexpr (std::is_integral_v<Key>) return h(static_cast<u64>(key));
            else if constexpr (std::is_enum_v<Key>) return h(static_cast<u64>(static_cast<std::underlying_type_t<Key>>(key)));
            else if constexpr (std::is_pointer_v<Key>) return h(reinterpret_cast<const void*>(key));
            else if constexpr (std::is_same_v<Key,const char*> || std::is_same_v<Key,char*>) return h(static_cast<const char*>(key));
            else { static_assert(!std::is_same_v<Hasher, DefaultHashIntPtr>, "UnorderedMap requires a custom Hash for this Key type"); return 0; }
        } else { return static_cast<u64>(h(key)); }
    }
    b8 probe_find_or_first_empty(const Key& key, size_type& out_idx) const {
        if(_cap==0){ out_idx=0; return false; }
        size_type i = hasher_key(key) & mask(); size_type first_tomb=_cap; for(;;){ auto st=_slots[i].state; if(st==EMPTY){ out_idx=(first_tomb!=_cap)?first_tomb:i; return false; } if(st==TOMBSTONE){ if(first_tomb==_cap) first_tomb=i; } else { if(_slots[i].kv.first==key){ out_idx=i; return true; } } i=(i+1)&mask(); }
    }
    size_type find_index(const Key& key) const { if(_cap==0) return _cap; size_type i=hasher_key(key)&mask(); for(;;){ auto st=_slots[i].state; if(st==EMPTY) return _cap; if(st==OCCUPIED && _slots[i].kv.first==key) return i; i=(i+1)&mask(); } }

    void destroy_slot(size_type i){ if(_slots[i].state==OCCUPIED){ if constexpr(!std::is_trivially_destructible_v<Key>) _slots[i].kv.first.~Key(); if constexpr(!std::is_trivially_destructible_v<T>) _slots[i].kv.second.~T(); } }
    void destroy_slots(){ if(!_slots) return; for(size_type i=0;i<_cap;++i) if(_slots[i].state==OCCUPIED) destroy_slot(i); _size=0; for(size_type i=0;i<_cap;++i) _slots[i].state=EMPTY; }
    void destroy_and_free(){ if(_slots){ destroy_slots(); GetMemoryManager().Free(_slots,_cap*sizeof(Slot),RT_MEMORY_TAG::CONTAINER); _slots=nullptr; _cap=0; } }
    void copy_from(const UnorderedMap& o){ if(o._cap==0){ _slots=nullptr; _cap=0; _size=0; return; } _cap=o._cap; _slots=(Slot*)GetMemoryManager().Allocate(_cap*sizeof(Slot),(u8)alignof(Slot),RT_MEMORY_TAG::CONTAINER); for(size_type i=0;i<_cap;++i){ _slots[i].state=o._slots[i].state; if(o._slots[i].state==OCCUPIED){ ::new ((void*)&_slots[i].kv.first) Key(o._slots[i].kv.first); ::new ((void*)&_slots[i].kv.second) T(o._slots[i].kv.second); } } _size=o._size; }
    void move_from(UnorderedMap& o) noexcept { _slots=o._slots; _cap=o._cap; _size=o._size; o._slots=nullptr; o._cap=0; o._size=0; }
    void rehash_to_at_least(size_type min_cap){ size_type new_cap = round_up_pow2((size_type)(min_cap/MAX_LOAD)+1); if(new_cap<=_cap) return; Slot* ns=(Slot*)GetMemoryManager().Allocate(new_cap*sizeof(Slot),(u8)alignof(Slot),RT_MEMORY_TAG::CONTAINER); for(size_type i=0;i<new_cap;++i) ns[i].state=EMPTY; Slot* old=_slots; size_type oc=_cap; _slots=ns; _cap=new_cap; size_type os=_size; _size=0; if(old){ for(size_type i=0;i<oc;++i) if(old[i].state==OCCUPIED){ size_type idx; probe_find_or_first_empty(old[i].kv.first, idx); ::new ((void*)&_slots[idx].kv.first) Key(static_cast<Key&&>(old[i].kv.first)); ::new ((void*)&_slots[idx].kv.second) T(static_cast<T&&>(old[i].kv.second)); _slots[idx].state=OCCUPIED; ++_size; } GetMemoryManager().Free(old, oc*sizeof(Slot), RT_MEMORY_TAG::CONTAINER);} (void)os; }

private:
    Slot* _slots = nullptr; size_type _cap = 0; size_type _size = 0;
};

} // namespace ReiToEngine

#endif
