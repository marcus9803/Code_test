#include "server_input.hpp"

#include <stdint.h>
#include <string>
#include <regex>
#include <asio.hpp>

void read_server_input(std::string str_, uint64_t *id_ptr, uint32_t *x_cord_ptr, uint32_t *y_cord_ptr, uint8_t *type_ptr)
{
    std::regex delimiter("[;=]"); // Regular expression for delimiters

    std::sregex_token_iterator tokenIt(str_.begin(), str_.end(), delimiter, -1);
    std::sregex_token_iterator end;

    std::vector<std::string> tokens;
    for (; tokenIt != end; ++tokenIt)
    {
        tokens.push_back(*tokenIt); // Add each token to the vector
    }
    *id_ptr = std::stoull(tokens[1]);
    *x_cord_ptr = std::stoull(tokens[3]);
    *y_cord_ptr = std::stoull(tokens[5]);
    *type_ptr = std::stoull(tokens[7]); // TODO: Check max uint8
}
