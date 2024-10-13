#include "server_input.hpp"

#include <stdint.h>
#include <string>
#include <regex>
#include <asio.hpp>
#include <iostream>
#include "object_handler.hpp"

uint64_t id = 0;
uint32_t x_cord = 0, y_cord = 0;
uint8_t type = 0;

void handle_read(const asio::error_code &error, std::size_t bytes_transferred, std::shared_ptr<asio::streambuf> buffer, asio::ip::tcp::socket &socket, std::vector<object_t> &object_list_)
{
    if (!error)
    {
        std::istream is(buffer.get());
        std::string str;

        while (std::getline(is, str))
        {
            // std::cout << "Received string: " << str << std::endl;

            if (extract_server_input_content(str, &id, &x_cord, &y_cord, &type))
            {
                uint16_t index = object_rx_update(object_list_, id, x_cord, y_cord, type);
                // object_color_update(&object_list_[index]);
            }
        }

        // Continue reading
        asio::async_read_until(socket, *buffer, '\n',
                               [&socket, buffer, &object_list_](const asio::error_code &ec, std::size_t bytes_transferred)
                               {
                                   handle_read(ec, bytes_transferred, buffer, socket, object_list_);
                               });
    }
    else
    {
        std::cerr << "Error during read: " << error.message() << std::endl;
    }
}

bool extract_server_input_content(std::string str_, uint64_t *id_ptr, uint32_t *x_cord_ptr, uint32_t *y_cord_ptr, uint8_t *type_ptr)
{
    try
    {
        if (str_.empty())
        {
            std::cerr << "Error: Input string is empty." << std::endl;
            return false;
        }
        std::regex delimiter("[;=]"); // Regular expression for delimiters

        std::sregex_token_iterator tokenIt(str_.begin(), str_.end(), delimiter, -1);
        std::sregex_token_iterator end;

        std::vector<std::string> tokens;
        for (; tokenIt != end; ++tokenIt)
        {
            if (tokenIt->str().empty())
            {
                std::cerr << "Error: Empty token found." << std::endl;
                return false;
            }
            tokens.push_back(*tokenIt); // Add each token to the vector
        }

        if (tokens.size() < 8)
        {
            std::cerr << "Error: Not enough tokens extracted from the string." << std::endl;
            return false;
        }

        *id_ptr = std::stoull(tokens[1]);
        *x_cord_ptr = std::stoull(tokens[3]);
        *y_cord_ptr = std::stoull(tokens[5]);
        *type_ptr = std::stoull(tokens[7]); // TODO: Check max uint8
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error server extract: " << e.what() << std::endl;
        return false;
    }
}
