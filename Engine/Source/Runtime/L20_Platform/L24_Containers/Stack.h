#ifndef PLATFORM_CONTAINERS_STACK_H
#define PLATFORM_CONTAINERS_STACK_H

#include "L0_Macro/Include.h"
#include "L20_Platform/L24_Containers/List.h"

namespace ReiToEngine {

// 简化版 stack，底层使用 List，暴露常用接口与 std::stack 一致命名/语义
template <typename T>
class Stack /* : public IContainer<Stack<T>> */ {
public:
    using value_type = T;
    using size_type = u64;

    Stack() = default;

    [[nodiscard]] b8 empty() const noexcept { return _data.empty(); }
    [[nodiscard]] size_type size() const noexcept { return _data.size(); }
    void clear() noexcept { _data.clear(); }

    value_type& top() noexcept { return _data.back(); }
    const value_type& top() const noexcept { return _data.back(); }

    void push(const value_type& v) { _data.push_back(v); }
    void push(value_type&& v) { _data.push_back(static_cast<value_type&&>(v)); }

    template <typename... Args>
    value_type& emplace(Args&&... args) { return _data.emplace_back(static_cast<Args&&>(args)...); }

    void pop() { _data.pop_back(); }

private:
    List<value_type> _data;
};

} // namespace ReiToEngine

#endif
