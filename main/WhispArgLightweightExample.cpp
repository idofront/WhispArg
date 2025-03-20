/*
 Copyright (c) 2025 idofront

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

#include <idofront/WhispArg.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    // This example is a lightweight version of WhispArg. It does not support the automatic generation of help messages.
    // If you need help messages, I recommend using the full version of WhispArg. However, a help message can be
    // displayed if you write help arguments manually.
    try
    {
        auto helpWidthArgument = idofront::whisparg::Argument<uint16_t>::New('w', "help-width")
                                     .Description("Description is meanless for lightweight usage.")
                                     .Default(80);
        auto helpWidthOpt = idofront::whisparg::Parse(argc, argv, helpWidthArgument);
        if (helpWidthOpt.has_value())
        {
            std::cout << "Help width: " << helpWidthOpt.value() << "\n" << std::flush;
        }
        else
        {
            // notes: In help-width argument, never reach here because it has default value.
            std::cerr << "Failed to parse the help width.\n" << std::flush;
            return 1;
        }

        auto noDescriptionArgumentOpt = idofront::whisparg::Parse(
            argc, argv, idofront::whisparg::Argument<idofront::whisparg::type::Flag>::New('n', "no-description"));

        auto lengthArgumentOpt =
            idofront::whisparg::Parse(argc, argv, idofront::whisparg::Argument<uint8_t>::New('l', "length").Default(1));

        // notes: The message argument has no default value, so it is required. In this case, if the message argument is
        // not specified, an exception will be thrown at get value.
        auto messageArgumentOpt =
            idofront::whisparg::Parse(argc, argv, idofront::whisparg::Argument<std::string>::New("message"));

        // notes: The required argument is set as required, so it must be specified. In this case, if the required
        // argument is not specified, an exception will be thrown at call Parse().
        auto requiredArgumentOpt = idofront::whisparg::Parse(
            argc, argv,
            idofront::whisparg::Argument<idofront::whisparg::type::Flag>::New('r', "required").IsRequired(true));

        // notes: The difference between the case where Default is not specified and the case where IsRequired is
        // specified is the timing at which an exception is thrown. In the case where Default is not specified, the
        // std::optional returned by Parse() is empty, so an exception is thrown by value(). In the case where
        // IsRequired is specified, an exception is thrown at the time of calling Parse().

        if (noDescriptionArgumentOpt.value())
        {
            std::cout << "The \"--no-description\" is example of no description argument on the help message.\n"
                      << std::flush;
            return 0;
        }
        std::cout << "Length: " << (uint64_t)lengthArgumentOpt.value() << "\n" << std::flush;
        if (messageArgumentOpt.has_value())
        {
            std::cout << "Message: " << messageArgumentOpt.value() << "\n" << std::flush;
        }
        else
        {
            throw std::runtime_error("Failed to parse the message argument.");
        }
        if (requiredArgumentOpt.has_value())
        {
            std::cout << "Required: " << requiredArgumentOpt.value() << "\n" << std::flush;
        }
        else
        {
            throw std::runtime_error("Failed to parse the required argument.");
        }
    }
    catch (const std::exception &e)
    {
        auto message = std::string("Error: ") + e.what() + "\n";
        std::cerr << message << std::flush;
        return 1;
    }

    return 0;
}
