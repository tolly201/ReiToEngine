#ifndef PLATFORM_CONTAINERS_SET_H
#define PLATFORM_CONTAINERS_SET_H
#include "IContainer.h"
#include <set>
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API Set : public IContainer<Set<T>>
{
private:

public:

    void insert(const T& value) { data.insert(value); }

    // 移除元素
    void erase(const T& value) { data.erase(value); }

    // 查找元素
    bool contains(const T& value) const { return data.find(value) != data.end(); }

    // 清空集合
    void clear() { data.clear(); }

    // 获取元素数量
    size_t size() const { return data.size(); }

    // 支持范围for循环
    auto begin() { return data.begin(); }
    auto end()   { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end()   const { return data.end(); }

protected:
    std::set<T> data;
};
}

#endif
