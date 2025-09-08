// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include <string>

namespace Balltze {
    std::vector<std::string> split_arguments(std::string command) noexcept {
        // This is the vector to return.
        std::vector<std::string> arguments;

        // This value will be true if we are inside quotes, during which the word will not separate into arguments.
        bool in_quotes = false;

        // If using a backslash, add the next character to the string regardless of what it is.
        bool escape_character = false;

        // Regardless of if there were any characters, there was an argument.
        bool allow_empty_argument = false;

        // Get the command
        std::size_t command_size = command.size();

        // Get the argument
        std::string argument;
        for(std::size_t i = 0; i < command_size; i++) {
            if(escape_character) {
                escape_character = false;
            }
            // Escape character - this will be used to include the next character regardless of what it is
            else if(command[i] == '\\') {
                escape_character = true;
                continue;
            }
            // If a whitespace or octotothorpe is in quotations in the argument, then it is considered part of the argument.
            else if(command[i] == '"') {
                in_quotes = !in_quotes;
                allow_empty_argument = true;
                continue;
            }
            else if((command[i] == ' ' || command[i] == '\r' || command[i] == '\n' || command[i] == '#') && !in_quotes) {
                // Add argument if not empty.
                if(argument != "" || allow_empty_argument) {
                    arguments.push_back(argument);
                    argument = "";
                    allow_empty_argument = false;
                }

                // Terminate if beginning a comment.
                if(command[i] == '#') {
                    break;
                }
                continue;
            }
            argument += command[i];
        }

        // Add the last argument.
        if(argument != "" || allow_empty_argument) {
            arguments.push_back(argument);
        }

        return std::move(arguments);
    }

    std::string unsplit_arguments(const std::vector<std::string> &arguments) noexcept {
        // This is the string to return.
        std::string unsplit;

        for(std::size_t i = 0; i < arguments.size(); i++) {
            // This is a reference to the argument we're dealing with.
            const std::string &argument = arguments[i];

            // This will be the final string we append to the unsplit string.
            std::string argument_final;

            // Set this to true if we need to surround this argument with quotes.
            bool surround_with_quotes = false;

            // Go through each character and add them one-by-one to argument_final.
            for(const char &c : argument) {
                switch(c) {
                    // Backslashes and quotation marks should be escaped.
                    case '\\':
                    case '"':
                        argument_final += '\\';
                        break;

                    // If we're using spaces or octothorpes, the argument should be surrounded with quotation marks. We could escape those, but this is more readable.
                    case '#':
                    case ' ':
                        surround_with_quotes = true;
                        break;

                    default:
                        break;
                }
                argument_final += c;
            }

            if(surround_with_quotes) {
                argument_final = std::string("\"") + argument_final + "\"";
            }

            unsplit += argument_final;

            // Add the space to separate the next argument.
            if(i + 1 < arguments.size()) {
                unsplit += " ";
            }
        }

        return unsplit;
    }
}
