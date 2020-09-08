#include <lib/lib.hpp>

#include <boost/test/unit_test.hpp>

#include <functional>

using xzr::lib::uptr_ref;

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
BOOST_AUTO_TEST_SUITE(suite_uptr_ref)

BOOST_AUTO_TEST_CASE(test_uptr_ref)
{
    {
        auto i = uptr_ref<II>();
        {
            std::unique_ptr<I> a = i.move();
            BOOST_TEST(i.get() == a.get());

            auto b = std::move(a);
            BOOST_TEST(!a.get());
            BOOST_TEST(b.get());
            BOOST_TEST(i.get());
            BOOST_TEST(i.get() == b.get());
        } // c is destroyed and hence calls delete on given ptr
        // so this MIGHT sefault
        // BOOST_TEST(i.get());
        // this WILL segfault
        // BOOST_TEST(i->value() == 0);
    }
    {
        auto i = uptr_ref<II>(1);
        BOOST_TEST(i.get()->value() == 1);
        BOOST_TEST(i->value() == 1);
    }
    {
        auto i = uptr_ref<II>(1);
        static_cast<void>(i.move());
        BOOST_CHECK_THROW(i.move(), std::runtime_error);
    }
    {
        auto i = uptr_ref<II>(1);
        auto a = A{i.move()};
        BOOST_TEST(a.value() == 1);
        i->value(2);
        BOOST_TEST(a.value() == 2);
    }
}

BOOST_AUTO_TEST_SUITE_END()
} // namespace
