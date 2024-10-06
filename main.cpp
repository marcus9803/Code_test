#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <stdio.h>

#include <asio.hpp>

typedef struct
{
    uint64_t id;
    uint32_t x_cord;
    uint32_t y_cord;
    uint8_t type;
    uint8_t color[3];
} object_t;

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

    /*  // Print the tokens
     for (const auto &token : tokens)
     {
         std::cout << token << std::endl;
     }
     std::cout << tokens[1] << std::endl; */

    /*  for (std::string str; std::getline(input, str);)
         std::cout << str << '\n'; */
}

// g++ -I../../asio/asio/include main.cpp -o test.exe -lws2_32