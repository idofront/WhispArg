/*
 Copyright (c) 2024 idofront

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef IDOFRONT__ARGUMENT__PARSER_HPP
#define IDOFRONT__ARGUMENT__PARSER_HPP

#include <cctype>
#include <functional>
#include <iostream>
#include <locale>
#include <numeric>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace idofront
{
namespace whisparg
{
namespace type
{
/// @brief A class for specifying flag options in the WhispArg.
/// @note Even if you specify a bool in the WhispArg, you need to provide either "true" or "false" as an argument.
///       However, by using the Flag class, you can specify "true" without providing any arguments.
class Flag
{
  public:
    Flag(bool value = false) : _Value(value)
    {
    }

    /// @brief Converts the Flag class to a bool.
    operator bool() const
    {
        return _Value;
    }

    /// @brief A Flag object representing True.
    static const Flag True;

    /// @brief A Flag object representing False.
    static const Flag False;

    /// @brief Converts the Flag object to a string.
    const std::string ToString() const
    {
        return _Value ? "true" : "false";
    }

  private:
    bool _Value;
};

/// @brief Converts the Flag object to a string.
std::ostream &operator<<(std::ostream &os, const Flag &flag)
{
    os << flag.ToString();
    return os;
}

const Flag Flag::True = Flag(true);
const Flag Flag::False = Flag(false);
} // namespace type

/// @brief A class that defines command-line arguments and holds the parsing results.
/// @tparam T The type of the command-line argument.
template <typename T> class Argument
{
  public:
    /// @brief Creates a command-line argument with a short name.
    static Argument New(const char &shortName, const std::string &name)
    {
        return Argument(std::string(1, shortName), name);
    }

    /// @brief Creates a command-line argument without a short name.
    static Argument New(const std::string &name)
    {
        return Argument(name);
    }

    /// @brief Updates the value of a command-line argument.
    /// @note New creates a command-line argument without a value.
    ///       By using Update, you can assign a value to it.
    static Argument Update(const Argument &argument, const std::optional<T> &value)
    {
        auto updatedArgument = argument;
        updatedArgument._Value = value;
        return updatedArgument;
    }

    /// @brief Gets the name of the command-line argument.
    std::string Name() const
    {
        return _Name;
    }

    /// @brief Gets the short name of the command-line argument.
    std::string ShortName() const
    {
        return _ShortName;
    }

    /// @brief Sets the description of the command-line argument.
    Argument Description(const std::string &description)
    {
        _Description = description;
        return *this;
    }

    /// @brief Gets the description of the command-line argument.
    std::string Description() const
    {
        return _Description;
    }

    /// @brief Sets the default value for the command-line argument.
    Argument Default(const T &defaultValue)
    {
        _DefaultValue = defaultValue;
        return *this;
    }

    /// @brief Gets the default value of the command-line argument.
    /// @note Returns std::nullopt if no default value is set.
    std::optional<T> Default() const
    {
        return _DefaultValue;
    }

    /// @brief Specifies whether the command-line argument is required.
    Argument IsRequired(bool isRequired)
    {
        _IsRequired = isRequired;
        return *this;
    }

    /// @brief Checks whether the command-line argument is required.
    bool IsRequired() const
    {
        return _IsRequired;
    }

    /// @brief Gets the value of the command-line argument.
    /// @note If the value is not set, Default() is returned.
    std::optional<T> Value() const
    {
        return this->_Value.has_value() ? this->_Value : this->Default();
    }

    /// @brief Converts the command-line argument’s value to a string for help display.
    const std::string ToHelpString() const
    {
        auto upperCaseName = std::string(_Name);
        std::transform(upperCaseName.begin(), upperCaseName.end(), upperCaseName.begin(),
                       [](char const &c) { return std::toupper(c); });

        auto isFlag = std::is_same_v<T, type::Flag>;

        std::stringstream ss;
        ss << "  ";
        if (!_ShortName.empty())
        {
            ss << "-" << _ShortName << ", ";
        }
        ss << "--" << _Name;
        if (!isFlag)
        {
            ss << " <" << upperCaseName << ">";
        }
        ss << "\n";
        ss << "    " << _Description;
        if (_DefaultValue.has_value())
        {
            ss << " (Default: " << _DefaultValue.value() << ")";
        }
        return ss.str();
    }

  private:
    std::string _Name;
    std::string _ShortName;
    std::string _Description;
    std::optional<T> _DefaultValue;
    bool _IsRequired;
    std::optional<T> _Value;

    Argument(const std::string &shortName, const std::string &name)
        : _Name(name), _ShortName(shortName), _Description(), _DefaultValue(std::nullopt), _IsRequired(false)
    {
    }

    Argument(const std::string &name) : Argument("", name)
    {
    }
};

/// @brief Parses command-line arguments.
/// @tparam T The type of the command-line argument.
/// @param argv A vector of command-line arguments.
/// @param argument The definition of the command-line argument.
/// @param converter A function that converts the command-line argument string into type T.
/// @return The value of the command-line argument (wrapped in std::optional).
template <typename T>
std::optional<T> Parse(std::vector<std::string> argv, const Argument<T> &argument,
                       std::function<T(const std::string &)> converter)
{
    std::size_t argc = argv.size();

    auto argumentName = argument.Name();
    auto actualValue = std::string();

    auto isArgumentMatched = [](const Argument<T> &argument, const std::string &currentArgumentName) {
        switch (currentArgumentName.size())
        {
        case 0:
        case 1:
            return false;
        case 2:
            if (argument.ShortName().empty())
            {
                return false;
            }
            return ("-" + argument.ShortName()) == currentArgumentName;
        default:
            return ("--" + argument.Name()) == currentArgumentName;
        }
    };

    for (auto i = std::size_t(0); i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            auto currentArgumentName = std::string(argv[i]);
            if (isArgumentMatched(argument, currentArgumentName))
            {
                if (std::is_same_v<T, type::Flag>)
                {
                    actualValue = type::Flag::True.ToString();
                }
                else
                {
                    auto nextValueExists = (i + 1) < argc;
                    if (nextValueExists)
                    {
                        actualValue = std::string(nextValueExists ? argv[++i] : "");
                        continue;
                    }
                    else
                    {
                        throw std::invalid_argument("Argument \"" + argumentName + "\" requires a value.");
                    }
                }
            }
        }
    }

    if (actualValue.empty())
    {
        if (argument.IsRequired())
        {
            std::stringstream ss;
            ss << "Argument \"" << argumentName << "\" is required.";
            auto message = ss.str();
            throw std::invalid_argument(message);
        }

        return argument.Default();
    }

    try
    {
        return converter(actualValue);
    }
    catch (const std::exception &e)
    {
        std::stringstream ss;
        ss << "Failed to parse the argument \"" << argumentName << "\": " << e.what();
        auto message = ss.str();
        throw std::invalid_argument(message);
    }
}

/// @brief Parses command-line arguments.
/// @tparam T The type of the command-line argument.
/// @param argc The number of command-line arguments.
/// @param argv The command-line arguments.
/// @param argument The definition of the command-line argument.
/// @param converter A function that converts the command-line argument string to type T.
/// @return The value of the command-line argument (wrapped in std::optional).
template <typename T>
std::optional<T> Parse(int argc, char *argv[], const Argument<T> &argument,
                       std::function<T(const std::string &)> converter)
{
    return Parse(std::vector<std::string>(argv, argv + argc), argument, converter);
}

/// @brief Parses command-line arguments for types that support automatic conversion.
/// @tparam T The type of the command-line argument.
/// @param argv A vector of command-line arguments.
/// @param argument The definition of the command-line argument.
/// @return The value of the command-line argument (wrapped in std::optional).
/// @note Ensures at compile time that the type is supported.
template <typename T> std::optional<T> Parse(std::vector<std::string> argv, const Argument<T> &argument)
{
    static_assert(std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, int32_t> ||
                      std::is_same_v<T, int64_t> || std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> ||
                      std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t> || std::is_same_v<T, float> ||
                      std::is_same_v<T, double> || std::is_same_v<T, long double> || std::is_same_v<T, std::string> ||
                      std::is_same_v<T, bool> || std::is_same_v<T, type::Flag>,
                  "Type not supported automatically. Please provide a converter function.");

    auto getConverter = std::function<std::function<T(const std::string &)>(const Argument<T> &)>(
        [](const Argument<T> &) -> std::function<T(const std::string &)> {
            if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, int32_t> ||
                          std::is_same_v<T, int64_t>)
            {
                return [](const std::string &value) { return std::stoll(value); };
            }
            else if constexpr (std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> ||
                               std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>)
            {
                return [](const std::string &value) { return std::stoull(value); };
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                return [](const std::string &value) { return std::stof(value); };
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return [](const std::string &value) { return std::stod(value); };
            }
            else if constexpr (std::is_same_v<T, long double>)
            {
                return [](const std::string &value) { return std::stold(value); };
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return [](const std::string &value) { return value; };
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return [](const std::string &value) {
                    if (value == "true")
                    {
                        return true;
                    }
                    else if (value == "false")
                    {
                        return false;
                    }
                    else
                    {
                        try
                        {
                            auto integerValue = std::stoll(value);
                            return static_cast<bool>(integerValue);
                        }
                        catch (const std::exception &e)
                        {
                            throw std::invalid_argument("Value must be either \"true\"(1) or \"false\"(0).");
                        }
                    }
                };
            }
            else if constexpr (std::is_same_v<T, type::Flag>)
            {
                return [](const std::string &) { return type::Flag::True; };
            }
            else
            {
                throw std::invalid_argument("Type not supported.");
            }
        });

    auto converter = getConverter(argument);

    return Parse(argv, argument, converter);
}

/// @brief Parses command-line arguments for types that support automatic conversion.
/// @tparam T The type of the command-line argument.
/// @param argc The number of command-line arguments.
/// @param argv The command-line arguments.
/// @param argument The definition of the command-line argument.
/// @return The value of the command-line argument (wrapped in std::optional).
/// @note Ensures at compile time that the type is supported.
template <typename T> std::optional<T> Parse(int argc, char *argv[], const Argument<T> &argument)
{
    return Parse(std::vector<std::string>(argv, argv + argc), argument);
}

/// @brief A preset Argument for the --help option.
inline Argument<type::Flag> Help =
    Argument<type::Flag>::New('h', "help").Description("Show help message.").Default(type::Flag::False);

/// @brief A class that holds information about a command-line argument.
class ArgumentInformation
{
  public:
    /// @brief Creates an ArgumentInformation object from an Argument object.
    template <typename T> static ArgumentInformation New(const Argument<T> &argument)
    {
        auto isFlag = std::is_same_v<T, type::Flag>;
        return ArgumentInformation(argument.Name(), argument.ShortName(), argument.Description(), isFlag,
                                   argument.IsRequired());
    }

    /// @brief Constructs an ArgumentInformation object.
    ArgumentInformation(const std::string &name, const std::string &shortName, const std::string &description,
                        bool isFlag, bool isRequired)
        : _Name(name), _ShortName(shortName), _Description(description), _IsFlag(isFlag), _IsRequired(isRequired)
    {
    }

    /// @brief Name.
    std::string Name() const
    {
        return _Name;
    }

    /// @brief Short name.
    std::string ShortName() const
    {
        return _ShortName;
    }

    /// @brief Description.
    std::string Description() const
    {
        return _Description;
    }

    /// @brief Whether it is a flag.
    bool IsFlag() const
    {
        return _IsFlag;
    }

    /// @brief Whether it is required.
    bool IsRequired() const
    {
        return _IsRequired;
    }

  private:
    std::string _Name;
    std::string _ShortName;
    std::string _Description;
    bool _IsFlag;
    bool _IsRequired;
};

/// @brief A class that parses command-line arguments.
/// @note Although you can parse arguments using the Parse() function directly,
///       using the WhispArg class allows you to generate help messages from multiple command-line arguments.
class WhispArg
{
  public:
    /// @brief Constructs a WhispArg object to parse command-line arguments.
    WhispArg(int argc, char *argv[]) : _ArgumentValues(std::vector<std::string>(argv, argv + argc))
    {
    }

    /// @brief Parses command-line arguments based on the specified Argument object and returns
    ///        an Argument object containing the parsed results.
    template <typename T> Argument<T> Parse(const Argument<T> &argument)
    {
        auto information = ArgumentInformation::New(argument);
        _ArgumentInformations.push_back(information);

        auto result = ::idofront::whisparg::Parse(_ArgumentValues, argument);
        return result.has_value() ? idofront::whisparg::Argument<T>::Update(argument, result) : argument;
    }

    /// @brief Displays a help message based on the Argument objects that have been parsed so far.
    /// @param maxWidth The maximum width of the help message.
    void ShowHelp(std::size_t maxWidth = 80)
    {
        auto helpLines = std::vector<std::string>{
            "Usage: " + _ArgumentValues[0] + " [options]",
            "Options:",
        };

        std::vector<std::tuple<std::string, std::string>> argumentHelps;
        std::transform(_ArgumentInformations.begin(), _ArgumentInformations.end(), std::back_inserter(argumentHelps),
                       [](const ArgumentInformation &information) {
                           auto upperCaseName = std::string(information.Name());
                           std::transform(upperCaseName.begin(), upperCaseName.end(), upperCaseName.begin(),
                                          [](char const &c) { return std::toupper(c); });

                           auto helpString =
                               "--" + information.Name() +
                               (information.ShortName().empty() ? "" : " (-" + information.ShortName() + ")");
                           if (!information.IsFlag())
                           {
                               helpString += " <" + upperCaseName + ">";
                           }

                           return std::make_tuple(helpString, information.Description());
                       });

        auto keysMaxLength = std::size_t(std::accumulate(argumentHelps.begin(), argumentHelps.end(), 0,
                                                         [](std::size_t l, std::tuple<std::string, std::string> tuple) {
                                                             auto helpString = std::get<0>(tuple);
                                                             return std::max(l, helpString.size());
                                                         }));

        auto isOneLine = keysMaxLength < (maxWidth / 3);

        std::for_each(argumentHelps.begin(), argumentHelps.end(), [&](std::tuple<std::string, std::string> tuple) {
            auto helpString = std::get<0>(tuple);
            auto description = std::get<1>(tuple);

            auto leadingString = std::string("");
            auto wrappedDescription = std::vector<std::string>();
            auto indentWidth = std::size_t(0);
            auto descriptionWidth = std::size_t(0);

            if (isOneLine)
            {
                auto padding = std::string(keysMaxLength - helpString.size(), ' ');

                descriptionWidth = maxWidth - keysMaxLength - 2;
                indentWidth = keysMaxLength + 2;
                leadingString = helpString + padding + "  ";
            }
            else
            {
                helpLines.push_back(helpString);

                indentWidth = std::size_t(maxWidth * (1.0 / 20.0));
                descriptionWidth = maxWidth - indentWidth;
                leadingString = std::string(indentWidth, ' ');
            }
            wrappedDescription = WrapLines(description, descriptionWidth);

            if (wrappedDescription.size())
            {
                std::for_each(wrappedDescription.begin(), wrappedDescription.end(), [&](const std::string &line) {
                    helpLines.push_back(leadingString + line);
                    leadingString = std::string(indentWidth, ' ');
                });
            }
            else
            {
                if (isOneLine)
                {
                    helpLines.push_back(leadingString);
                }
            }
        });

        std::for_each(helpLines.begin(), helpLines.end(), [](const std::string &line) {
            std::cout << line << "\n" << std::flush;
        });
    }

  private:
    std::vector<std::string> _ArgumentValues;
    std::vector<ArgumentInformation> _ArgumentInformations;

    /// @brief Wraps text to the specified width.
    /// @note Takes newline characters in @c text into account.
    static std::vector<std::string> WrapLines(const std::string &text, size_t maxWidth)
    {
        std::vector<std::string> lines;
        std::istringstream stream(text);
        std::string line;

        while (std::getline(stream, line))
        {
            auto partialLines = WrapLine(line, maxWidth);
            std::copy(partialLines.begin(), partialLines.end(), std::back_inserter(lines));
        }

        return lines;
    }

    /// @brief Wraps text to the specified width.
    /// @note Newline characters are ignored. If @c text may contain newline characters, WrapLines is recommended
    /// instead.
    static std::vector<std::string> WrapLine(const std::string &text, size_t maxWidth)
    {
        std::vector<std::string> lines;
        std::istringstream words(text);
        std::string word;
        std::string currentLine;

        while (words >> word)
        {
            if (currentLine.length() + word.length() + 1 <= maxWidth)
            {
                if (!currentLine.empty())
                {
                    currentLine += " ";
                }
                currentLine += word;
            }
            else
            {
                if (!currentLine.empty())
                {
                    lines.push_back(currentLine);
                }
                currentLine = word;
            }
        }

        if (!currentLine.empty())
        {
            lines.push_back(currentLine);
        }

        return lines;
    }
};

} // namespace whisparg
} // namespace idofront

#endif
