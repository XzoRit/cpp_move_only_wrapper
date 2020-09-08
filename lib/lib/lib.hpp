#pragma once

#include <iostream>
#include <memory>

namespace xzr
{
namespace lib
{
inline namespace v1
{
template <class A>
class uptr_ref
{
  public:
    using pointer = A*;
    template <class... Args>
    explicit uptr_ref(Args&&... args)
        : ptr_{new A{std::forward<Args>(args)...}}
    {
    }
    explicit uptr_ref(pointer a)
        : ptr_{a}
    {
    }
    std::unique_ptr<A> move()
    {
        if (moved)
            throw std::runtime_error{"can only be moved once"};
        moved = true;
        return std::unique_ptr<A>{get()};
    }
    pointer get()
    {
        return ptr_;
    }
    pointer operator->()
    {
        return get();
    }
    ~uptr_ref()
    {
        if (!moved)
        {
            delete ptr_;
            ptr_ = nullptr;
        }
    }

    pointer ptr_{nullptr};
    bool moved{false};
};
} // namespace v1
} // namespace lib
} // namespace xzr
