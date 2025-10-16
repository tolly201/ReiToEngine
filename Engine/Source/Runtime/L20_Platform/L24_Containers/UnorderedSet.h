#ifndef PLATFORM_CONTAINERS_UNORDERED_SET_H
#define PLATFORM_CONTAINERS_UNORDERED_SET_H

#include "L0_Macro/Include.h"
#include "L20_Platform/L23_MemoryManager/Memory.h"
#include <initializer_list>
#include <type_traits>

namespace ReiToEngine {

// 开放寻址哈希集合：std::unordered_set 风格
template <typename Key, typename Hash = void>
class UnorderedSet {
public:
    using value_type = Key; using size_type = u64;

    UnorderedSet() = default;
    ~UnorderedSet(){ destroy_and_free(); }
    UnorderedSet(std::initializer_list<value_type> init){ reserve(static_cast<size_type>(init.size()*2)); for (const auto& v : init) insert(v); }
    UnorderedSet(const UnorderedSet& o){ copy_from(o); }
    UnorderedSet& operator=(const UnorderedSet& o){ if(this!=&o){ destroy_and_free(); copy_from(o);} return *this; }
    UnorderedSet(UnorderedSet&& o) noexcept { move_from(o); }
    UnorderedSet& operator=(UnorderedSet&& o) noexcept { if(this!=&o){ destroy_and_free(); move_from(o);} return *this; }

    [[nodiscard]] size_type size() const noexcept { return _size; }
    [[nodiscard]] b8 empty() const noexcept { return _size==0; }
    void clear() noexcept { destroy_slots(); }
    void reserve(size_type new_cap){ rehash_to_at_least(new_cap); }

    class iterator {
    public:
        iterator():_set(nullptr),_idx(0){}
        iterator(UnorderedSet* s, size_type i):_set(s),_idx(i){ adv(); }
        value_type& operator*() const { return _set->_slots[_idx].v; }
        value_type* operator->() const { return &_set->_slots[_idx].v; }
        iterator& operator++(){ ++_idx; adv(); return *this; }
        b8 operator==(const iterator& o) const { return _set==o._set && _idx==o._idx; }
        b8 operator!=(const iterator& o) const { return !(*this==o); }
    private:
        void adv(){ while(_set && _idx<_set->_cap && _set->_slots[_idx].state!=OCCUPIED) ++_idx; }
        UnorderedSet* _set; size_type _idx; friend class UnorderedSet;
    };
    class const_iterator {
    public:
        const_iterator():_set(nullptr),_idx(0){}
        const_iterator(const UnorderedSet* s, size_type i):_set(s),_idx(i){ adv(); }
        const value_type& operator*() const { return _set->_slots[_idx].v; }
        const value_type* operator->() const { return &_set->_slots[_idx].v; }
        const_iterator& operator++(){ ++_idx; adv(); return *this; }
        b8 operator==(const const_iterator& o) const { return _set==o._set && _idx==o._idx; }
        b8 operator!=(const const_iterator& o) const { return !(*this==o); }
    private:
        void adv(){ while(_set && _idx<_set->_cap && _set->_slots[_idx].state!=OCCUPIED) ++_idx; }
        const UnorderedSet* _set; size_type _idx; friend class UnorderedSet;
    };

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

    std::pair<iterator, bool> insert(const value_type& v){ ensure_capacity_for_insert(); size_type idx; b8 f=probe_find_or_first_empty(v,idx); if(f) return { iterator(this,idx), false }; ::new ((void*)&_slots[idx].v) value_type(v); _slots[idx].state=OCCUPIED; ++_size; return { iterator(this,idx), true }; }
    std::pair<iterator, bool> insert(value_type&& v){ ensure_capacity_for_insert(); size_type idx; b8 f=probe_find_or_first_empty(v,idx); if(f) return { iterator(this,idx), false }; ::new ((void*)&_slots[idx].v) value_type(static_cast<value_type&&>(v)); _slots[idx].state=OCCUPIED; ++_size; return { iterator(this,idx), true }; }

    size_type erase(const value_type& v){ size_type i=find_index(v); if(i==_cap) return 0; destroy_slot(i); _slots[i].state=TOMBSTONE; --_size; return 1; }
    iterator erase(iterator pos){ if(pos==end()) return pos; size_type i=pos._idx; destroy_slot(i); _slots[i].state=TOMBSTONE; --_size; ++i; return iterator(this,i); }

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
    struct Slot { value_type v; SlotState state; };
    static constexpr f32 MAX_LOAD = 0.70f;

    Slot* _slots = nullptr; size_type _cap = 0; size_type _size = 0;

    size_type mask() const { return _cap?(_cap-1):0; }
    static size_type round_up_pow2(size_type x){ if(x<4) return 4; --x; x|=x>>1; x|=x>>2; x|=x>>4; x|=x>>8; x|=x>>16; if constexpr(sizeof(size_type)==8) x|=x>>32; return ++x; }
    void ensure_capacity_for_insert(){ if(_cap==0 || (_size+1) > static_cast<size_type>(_cap * MAX_LOAD)) rehash_to_at_least(_cap?(_cap*2):8); }

    u64 hasher_key(const value_type& v) const {
        Hasher h{};
        if constexpr (std::is_same_v<Hasher, DefaultHashIntPtr>) {
            if constexpr (std::is_integral_v<value_type>) return h(static_cast<u64>(v));
            else if constexpr (std::is_enum_v<value_type>) return h(static_cast<u64>(static_cast<std::underlying_type_t<value_type>>(v)));
            else if constexpr (std::is_pointer_v<value_type>) return h(reinterpret_cast<const void*>(v));
            else if constexpr (std::is_same_v<value_type,const char*> || std::is_same_v<value_type,char*>) return h(static_cast<const char*>(v));
            else { static_assert(!std::is_same_v<Hasher, DefaultHashIntPtr>, "UnorderedSet requires custom Hash for this value type"); return 0; }
        } else { return static_cast<u64>(h(v)); }
    }
    b8 probe_find_or_first_empty(const value_type& v, size_type& out) const {
        if(_cap==0){ out=0; return false; }
        size_type i=hasher_key(v)&mask(); size_type first_tomb=_cap; for(;;){ auto st=_slots[i].state; if(st==EMPTY){ out=(first_tomb!=_cap)?first_tomb:i; return false; } if(st==TOMBSTONE){ if(first_tomb==_cap) first_tomb=i; } else { if(_slots[i].v==v){ out=i; return true; } } i=(i+1)&mask(); }
    }
    size_type find_index(const value_type& v) const { if(_cap==0) return _cap; size_type i=hasher_key(v)&mask(); for(;;){ auto st=_slots[i].state; if(st==EMPTY) return _cap; if(st==OCCUPIED && _slots[i].v==v) return i; i=(i+1)&mask(); } }

    void destroy_slot(size_type i){ if(_slots[i].state==OCCUPIED){ if constexpr (!std::is_trivially_destructible_v<value_type>) _slots[i].v.~value_type(); } }
    void destroy_slots(){ if(!_slots) return; for(size_type i=0;i<_cap;++i) if(_slots[i].state==OCCUPIED) destroy_slot(i); _size=0; for(size_type i=0;i<_cap;++i) _slots[i].state=EMPTY; }
    void destroy_and_free(){ if(_slots){ destroy_slots(); GetMemoryManager().Free(_slots,_cap*sizeof(Slot),RT_MEMORY_TAG::CONTAINER); _slots=nullptr; _cap=0; } }
    void copy_from(const UnorderedSet& o){ if(o._cap==0){ _slots=nullptr; _cap=0; _size=0; return; } _cap=o._cap; _slots=(Slot*)GetMemoryManager().Allocate(_cap*sizeof(Slot),(u8)alignof(Slot),RT_MEMORY_TAG::CONTAINER); for(size_type i=0;i<_cap;++i){ _slots[i].state=o._slots[i].state; if(o._slots[i].state==OCCUPIED){ ::new ((void*)&_slots[i].v) value_type(o._slots[i].v); } } _size=o._size; }
    void move_from(UnorderedSet& o) noexcept { _slots=o._slots; _cap=o._cap; _size=o._size; o._slots=nullptr; o._cap=0; o._size=0; }
    void rehash_to_at_least(size_type min_cap){ size_type new_cap = round_up_pow2((size_type)(min_cap/MAX_LOAD)+1); if(new_cap<=_cap) return; Slot* ns=(Slot*)GetMemoryManager().Allocate(new_cap*sizeof(Slot),(u8)alignof(Slot),RT_MEMORY_TAG::CONTAINER); for(size_type i=0;i<new_cap;++i) ns[i].state=EMPTY; Slot* old=_slots; size_type oc=_cap; _slots=ns; _cap=new_cap; size_type os=_size; _size=0; if(old){ for(size_type i=0;i<oc;++i) if(old[i].state==OCCUPIED){ size_type idx; probe_find_or_first_empty(old[i].v, idx); ::new ((void*)&_slots[idx].v) value_type(static_cast<value_type&&>(old[i].v)); _slots[idx].state=OCCUPIED; ++_size; } GetMemoryManager().Free(old, oc*sizeof(Slot), RT_MEMORY_TAG::CONTAINER);} (void)os; }
};

} // namespace ReiToEngine

#endif
