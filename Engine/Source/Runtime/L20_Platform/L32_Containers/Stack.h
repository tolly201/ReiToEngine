#ifndef PLATFORM_CONTAINERS_STACK_H
#define PLATFORM_CONTAINERS_STACK_H
#include "IContainer.h"
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API stack : public IContainer<T>
{
private:
    constexpr static u32 GROWTH_FACTOR = 10;
public:
    stack();
    ~stack();
    vold clear();
    u64 size() const;
    T& emplace_back(Args&&... args);
    T& push_back(const T& value);
    T& at(u64 index);
    T& operator[](u64 index);
    T& front();
    T& back();
    u64 size() const;
    b8 empty() const;
    T& insert(u64 pos, const T& value);
    T& erase(u64 pos);
protected:
    u64 length;
};
}

#endif
