#ifndef PLATFORM_CONTAINERS_LIST_H
#define PLATFORM_CONTAINERS_LIST_H
#include "IContainer.h"
#include <utility>
#include <vector>
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API List : public IContainer<List<T>>
{
private:
    constexpr static u32 GROWTH_FACTOR = 10;
public:
    List() = default;
    ~List() = default;
    void clear(){_data.clear();}
    template <typename... Args>
    T& emplace_back(Args&&... args){
        return _data.emplace_back(std::forward<Args>(args)...);
    }
    void push_back(const T& value)
    {
        _data.push_back(value);
        return ;
    }
    T& at(u64 index)
    {
        return _data.at(index);
    }
    T& operator[](u64 index)
    {
        return _data[index];
    }
    T& front()
    {
        return _data.front();
    }
    T& back()
    {
        return _data.back();
    }
    u64 size() const
    {
        return _data.size();
    }
    b8 empty() const
    {
        return _data.empty();
    }

    auto begin() { return _data.begin(); }
    auto end()   { return _data.end(); }
    auto begin() const { return _data.begin(); }
    auto end()   const { return _data.end(); }
    T* data() {
        return _data.empty() ? nullptr : _data.data();
    }
    const T* data() const {
        return _data.empty() ? nullptr : _data.data();
    }

    void resize(u32 new_size)
    {
        _data.resize(new_size);
    }
protected:
    u64 length;
    std::vector<T> _data;
};
}

#endif
