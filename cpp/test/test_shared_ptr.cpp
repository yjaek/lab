#include "gtest/gtest.h"

#include "lib/SharedPtr.h"

class Foo
{
    std::string a;
    int b;

public:
    Foo(const std::string& a, const int b) : a(a), b(b) {}

    std::string getString() const { return a; }

    int getInt() const { return b; }
};

TEST(SharedPtrTest, Construct)
{
    SharedPtr<int> a;
    EXPECT_EQ(a.get(), nullptr);

    SharedPtr<int> b{new int{15}};
    EXPECT_NE(b.get(), nullptr);
    EXPECT_EQ(*b, 15);
}

TEST(SharedPtrTest, CopyConstructAndAssign)
{
    SharedPtr<int> a{new int{3}};
    SharedPtr<int> b{a};
    EXPECT_EQ(b.get(), a.get());

    SharedPtr<int> c;
    c = b;
    EXPECT_EQ(c.get(), b.get());
}

TEST(SharedPtrTest, MoveConstructAndAssign)
{
    SharedPtr<int> a{new int{1}};
    EXPECT_NE(a.get(), nullptr);

    auto* underlying = a.get();
    SharedPtr<int> b{std::move(a)};
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(b.get(), underlying);

    SharedPtr<int> c;
    c = std::move(b);
    EXPECT_EQ(b.get(), nullptr);
    EXPECT_EQ(c.get(), underlying);
}

TEST(SharedPtrTest, MakeShared)
{
    SharedPtr<Foo> fooPtr = makeShared<Foo>("a", 1);
    EXPECT_EQ(fooPtr->getString(), "a");
    EXPECT_EQ(fooPtr->getInt(), 1);
}
