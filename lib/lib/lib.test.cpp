#include <lib/lib.hpp>

#include <boost/test/unit_test.hpp>

using xzr::lib::add;
using xzr::lib::rvalue_ref;

struct I
{
    virtual int value() = 0;
    virtual void value(int) = 0;
    virtual ~I() = default;
};

struct II : I
{
    II() = default;
    explicit II(int v)
        : value_{v}
    {
    }
    int value() override
    {
        return value_;
    }
    void value(int v) override
    {
        value_ = v;
    }
    int value_{0};
};

struct A
{
    using Dep = std::unique_ptr<I>;
    explicit A(Dep dep)
        : dep_{std::move(dep)}
    {
    }
    int value()
    {
        return dep_->value();
    }
    Dep dep_{nullptr};
};

namespace
{
BOOST_AUTO_TEST_SUITE(lib_tests)

BOOST_AUTO_TEST_CASE(lib_add)
{
    BOOST_TEST(add(1, 3) == 4);
}

BOOST_AUTO_TEST_CASE(lib_unique_ptr)
{
    {
        auto a = A{std::make_unique<II>()};
        BOOST_TEST(a.value() == 0);
    }
    // {
    //     auto i = std::make_unique<II>();
    //     auto a = A{std::move(i)};
    //     i->value(1);
    //     BOOST_TEST(a.value() == 1);
    // }
    {
        auto i = rvalue_ref<II>();
        auto a = i.move();
        BOOST_TEST(a->value() == 0);
        i.get()->value(1);
        BOOST_TEST(a->value() == 1);
        auto b = std::move(a);
        BOOST_TEST(b->value() == 1);
        i.get()->value(2);
        BOOST_TEST(b->value() == 2);
        {
            auto a = A{std::move(b)};
            BOOST_TEST(a.value() == 2);
            i.get()->value(3);
            BOOST_TEST(a.value() == 3);
        }
    }
    {
        auto i = rvalue_ref<II>();
        auto a = A{i.move()};
        BOOST_TEST(a.value() == 0);
        i.get()->value(1);
        BOOST_TEST(a.value() == 1);
    }
    {
        auto i = rvalue_ref<II>(1);
        auto a = A{i.move()};
        BOOST_TEST(a.value() == 1);
    }
}

BOOST_AUTO_TEST_SUITE_END()
} // namespace
