#pragma once

#include <memory>

namespace xzr
{
namespace lib
{
inline namespace v1
{
/// \brief returns the sum of a and b.
///
/// Just a simple sumation function with a nice documentation.
int add(int a, int b);
template <class A>
struct rvalue_ref_wrapper
{
    std::unique_ptr<A> move()
    {
        return std::unique_ptr<A>{ptr_};
    }
    A* get()
    {
        return ptr_;
    }
    A* ptr_{};
};

template <class A>
inline rvalue_ref_wrapper<A> rvalue_ref()
{
    return rvalue_ref_wrapper<A>{new A{}};
}
} // namespace v1
} // namespace lib
} // namespace xzr
