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

#include <idofront/WhispArg.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    try
    {
        auto parser = idofront::argument::Parser(argc, argv);

        auto helpWidthArgument =
            idofront::argument::Argument<uint16_t>::New('w', "help-width")
                .Description(
                    "The width of the help message, and the following is an example of a long text:\n"
                    "One cold rainy day when my father was a little boy, he met an old alley cat on his street. The "
                    "cat was very drippy and uncomfortable so my father said, \"Wouldn't you like to come home with "
                    "me?\"\n"
                    "This surprised the cat—she had never before met anyone who cared about old alley cats—but she "
                    "said, \"I'd be very much obliged if I could sit by a warm furnace, and perhaps have a saucer of "
                    "milk.\"\n"
                    "\"We have a very nice furnace to sit by,\" said my father, \"and I'm sure my mother has an extra "
                    "saucer of milk.\"\n"
                    "(My Father's Dragon by Ruth Stiles Gannett)")
                .Default(80);
        auto helpWidth = parser.Parse(helpWidthArgument);

        auto noDescriptionArgument =
            idofront::argument::Argument<idofront::argument::type::Flag>::New('n', "no-description")
                .Default(idofront::argument::type::Flag::False);
        auto noDescription = parser.Parse(noDescriptionArgument);

        auto lengthArgument = idofront::argument::Argument<uint8_t>::New('l', "length")
                                  .Description("The length of the manager.")
                                  .Default(1);
        auto length = parser.Parse(lengthArgument);

        auto messageArgument = idofront::argument::Argument<std::string>::New("message")
                                   .Description("The message to be published.")
                                   .Default("Hello, world!");
        auto message = parser.Parse(messageArgument);

        auto help = parser.Parse<idofront::argument::type::Flag>(idofront::argument::Help);

        if (help.Value().value())
        {
            parser.ShowHelp(helpWidth.Value().value());
            return 0;
        }
        if (noDescription.Value().value())
        {
            std::cout << "The \"--no-description\" is example of no description argument on the help message.\n"
                      << std::flush;
            return 0;
        }

        std::cout << "Length: " << (uint64_t)length.Value().value() << "\n" << std::flush;
        std::cout << "Message: " << message.Value().value() << "\n" << std::flush;
    }
    catch (const std::exception &e)
    {
        auto message = std::string("Error: ") + e.what() + "\n";
        std::cerr << message << std::flush;
        return 1;
    }

    return 0;
}
