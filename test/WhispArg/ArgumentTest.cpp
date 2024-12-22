#include <gtest/gtest.h>
#include <idofront/WhispArg.hpp>
#include <optional>
#include <sstream>
#include <string>

using namespace idofront::whisparg;

TEST(ArgumentTest, CreateArgumentWithShortName)
{
    // Arrange
    auto arg = Argument<int>::New('n', "number");

    // Act & Assert
    EXPECT_EQ("number", arg.Name());
    EXPECT_EQ("n", arg.ShortName());
    EXPECT_FALSE(arg.IsRequired());          // default: false
    EXPECT_FALSE(arg.Default().has_value()); // Default is not set
    EXPECT_FALSE(arg.Value().has_value());   // Value is not set
}

TEST(ArgumentTest, CreateArgumentWithoutShortName)
{
    // Arrange
    auto arg = Argument<std::string>::New("message");

    // Act & Assert
    EXPECT_EQ("message", arg.Name());
    EXPECT_EQ("", arg.ShortName());
}

TEST(ArgumentTest, SetDescription)
{
    // Arrange
    auto arg = Argument<int>::New('n', "number").Description("An integer number argument");

    // Act & Assert
    EXPECT_EQ("An integer number argument", arg.Description());
}

TEST(ArgumentTest, SetDefaultValue)
{
    // Arrange
    auto arg = Argument<int>::New('n', "number").Default(42);

    // Act & Assert
    ASSERT_TRUE(arg.Default().has_value());
    EXPECT_EQ(42, arg.Default().value());
    EXPECT_EQ(42, arg.Value().value()); // Value is not set, so Default value is returned
}

TEST(ArgumentTest, RequiredArgument)
{
    // Arrange
    auto arg = Argument<int>::New('n', "number").IsRequired(true);

    // Act & Assert
    EXPECT_TRUE(arg.IsRequired());
}

TEST(ArgumentTest, UpdateValue)
{
    // Arrange
    auto arg = Argument<int>::New('n', "number").Default(10);

    // Defaule value is set to 10
    EXPECT_EQ(10, arg.Value().value());

    // Act
    auto updated = Argument<int>::Update(arg, std::optional<int>(99));

    // Assert
    ASSERT_TRUE(updated.Value().has_value());
    EXPECT_EQ(99, updated.Value().value());   //  Updated value is set
    EXPECT_EQ(10, updated.Default().value()); //  Default value is not changed
}

TEST(ArgumentTest, ValueWithoutDefaultIsEmpty)
{
    // Arrange
    auto arg = Argument<std::string>::New("title");

    // Act & Assert
    // Default value is not set
    EXPECT_FALSE(arg.Value().has_value());
}

TEST(ArgumentTest, ToHelpStringForNonFlag)
{
    // Arrange
    auto arg = Argument<int>::New('n', "number").Description("An integer argument").Default(42);

    // Act
    std::string helpStr = arg.ToHelpString();

    // Assert
    EXPECT_NE(std::string::npos, helpStr.find("-n, --number <NUMBER>"));
    EXPECT_NE(std::string::npos, helpStr.find("An integer argument"));
    EXPECT_NE(std::string::npos, helpStr.find("(Default: 42)"));
}

TEST(ArgumentTest, ToHelpStringForFlag)
{
    // Arrange
    // Template argument is Flag
    auto arg = Argument<type::Flag>::New('f', "force").Description("Forces an action").Default(type::Flag::False);

    // Act
    std::string helpStr = arg.ToHelpString();

    // Assert
    // The short name is displayed
    EXPECT_NE(std::string::npos, helpStr.find("-f, --force"));
    // If the Flag type is used, the <FORCE> is not displayed
    EXPECT_EQ(std::string::npos, helpStr.find("<FORCE>"));
    // The description and default value are displayed
    EXPECT_NE(std::string::npos, helpStr.find("Forces an action"));
    // The default value is displayed
    EXPECT_NE(std::string::npos, helpStr.find("(Default: false)"));
}
