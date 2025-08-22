#ifndef PLATFORM_CONTAINERS_MAP_H
#define PLATFORM_CONTAINERS_MAP_H
#include "IContainer.h"
#include <utility>
#include <map>
namespace ReiToEngine
{
template <typename Key, typename Value>
class RTENGINE_API Map : public IContainer<Map<Key, Value>>
{
private:
    constexpr static u32 GROWTH_FACTOR = 10;
public:
    Map() = default;
    ~Map() = default;

    Map(std::initializer_list<std::pair<const Key, Value>> init) {
        for (const auto& kv : init) {
            data[kv.first] = kv.second;
        }
    }
    void clear(){data.clear();}
    // 插入或更新
    void insert(const Key& key, const Value& value) {
        data[key] = value;
    }

    // 查找，返回指针，找不到返回nullptr
    // Value* find(const Key& key) {
    //     auto it = data.find(key);
    //     if (it != data.end())
    //         return &(it->second);
    //     return nullptr;
    // }

    // const Value* find(const Key& key) const {
    //     auto it = data.find(key);
    //     if (it != data.end())
    //         return &(it->second);
    //     return nullptr;
    // }

    // 下标操作符，插入新值或返回已有值
    Value& operator[](const Key& key) {
        return data[key];
    }

    // at方法，找不到会抛异常
    Value& at(const Key& key) {
        return data.at(key);
    }

    // 删除
    void erase(const Key& key) {
        data.erase(key);
    }

    // 大小
    u64 size() const {
        return data.size();
    }

    // 是否为空
    b8 empty() const {
        return data.empty();
    }

    // 迭代器支持
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto find(const Key& key) {
        return data.find(key);
    }

    auto find(const Key& key) const {
        return data.find(key);
    }


protected:
    u64 length;
    std::map<Key, Value> data;
};
}

#endif
