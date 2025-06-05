#ifndef PLATFORM_CONTAINERS_VECTOR_H
#define PLATFORM_CONTAINERS_VECTOR_H
#include "IContainer.h"
#include <utility>
#include <vector>
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API vector : public IContainer<T>
{
private:
    constexpr static u32 GROWTH_FACTOR = 10;
public:
    vector() = default;
    ~vector() = default;
    void clear(){data.clear();}
    template <typename... Args>
    T& emplace_back(Args&&... args){
        return data.emplace_back(std::forward<Args>(args)...);
    }
    T& push_back(const T& value)
    {
        return data.push_back(value);
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
protected:
    u64 length;
    std::vector<T> data;
};
}

#endif
