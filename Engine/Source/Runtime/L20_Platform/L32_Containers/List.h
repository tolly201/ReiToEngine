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
    void clear(){data.clear();}
    template <typename... Args>
    T& emplace_back(Args&&... args){
        return data.emplace_back(std::forward<Args>(args)...);
    }
    void push_back(const T& value)
    {
        data.push_back(value);
        return ;
    }
    T& at(u64 index)
    {
        return data.at(index);
    }
    T& operator[](u64 index)
    {
        return data[index];
    }
    T& front()
    {
        return data.front();
    }
    T& back()
    {
        return data.back();
    }
    u64 size() const
    {
        return data.size();
    }
    b8 empty() const
    {
        return data.empty();
    }

    auto begin() { return data.begin(); }
    auto end()   { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end()   const { return data.end(); }

protected:
    u64 length;
    std::vector<T> data;
};
}

#endif
