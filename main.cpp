#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <stdint.h>
#include "object_handler.h"

#include <asio.hpp>

const uint8_t type_count = 3;

object_t object_list[type_count] = {0};

int main()
{
    asio::ip::tcp::iostream input("localhost", "5463");
    std::string str;
    std::getline(input, str);
    std::cout << str << '\n';

    std::regex delimiter("[;=]"); // Regular expression for delimiters

    std::sregex_token_iterator tokenIt(str.begin(), str.end(), delimiter, -1);
    std::sregex_token_iterator end;

    std::vector<std::string> tokens;
    for (; tokenIt != end; ++tokenIt)
    {
        tokens.push_back(*tokenIt); // Add each token to the vector
    }
    uint64_t id = std::stoull(tokens[1]);
    uint32_t x_cord = std::stoull(tokens[3]);
    uint32_t y_cord = std::stoull(tokens[5]);
    uint8_t type = std::stoull(tokens[7]); // TODO: Check max uint8

    printf("%llu \n", id);
    printf("%lu \n", x_cord);
    printf("%lu \n", y_cord);
    printf("%lu \n", type);

    object_update(&object_list[type - 1], id, x_cord, y_cord, type);

    /*  // Print the tokens
     for (const auto &token : tokens)
     {
         std::cout << token << std::endl;
     }
     std::cout << tokens[1] << std::endl; */

    /*  for (std::string str; std::getline(input, str);)
         std::cout << str << '\n'; */
}