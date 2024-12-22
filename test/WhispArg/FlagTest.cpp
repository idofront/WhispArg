#include <gtest/gtest.h>
#include <idofront/WhispArg.hpp>

using namespace idofront::whisparg::type;

TEST(FlagTest, DefaultConstructorShouldBeFalse)
{
    Flag f;
    EXPECT_FALSE(f) << "Default-constructed Flag should be false.";
    EXPECT_EQ("false", f.ToString()) << "Expected string representation to be 'false'.";
}

TEST(FlagTest, ConstructorTrueShouldBeTrue)
{
    Flag f(true);
    EXPECT_TRUE(f) << "Flag constructed with true should evaluate to true.";
    EXPECT_EQ("true", f.ToString()) << "Expected string representation to be 'true'.";
}

TEST(FlagTest, StaticFlagTrueIsTrue)
{
    Flag f = Flag::True;
    EXPECT_TRUE(f) << "Flag::True should evaluate to true.";
    EXPECT_EQ("true", f.ToString()) << "Expected string representation to be 'true'.";
}

TEST(FlagTest, StaticFlagFalseIsFalse)
{
    Flag f = Flag::False;
    EXPECT_FALSE(f) << "Flag::False should evaluate to false.";
    EXPECT_EQ("false", f.ToString()) << "Expected string representation to be 'false'.";
}
