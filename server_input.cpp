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

void handle_read(const asio::error_code &error, std::size_t bytes_transferred, std::shared_ptr<asio::streambuf> buffer, asio::ip::tcp::socket &socket, object_t object_list_[])
{
    if (!error)
    {
        std::istream is(buffer.get());
        std::string str;

        while (std::getline(is, str))
        {
            std::cout << "Received string: " << str << std::endl;

            extract_server_input_content(str, &id, &x_cord, &y_cord, &type);
            object_rx_update(&object_list_[type - 1], id, x_cord, y_cord);
            object_color_update(&object_list_[type - 1]);
        }

        // Continue reading
        asio::async_read_until(socket, *buffer, '\n',
                               [&socket, buffer, object_list_](const asio::error_code &ec, std::size_t bytes_transferred)
                               {
                                   handle_read(ec, bytes_transferred, buffer, socket, object_list_);
                               });
    }
    else
    {
        std::cerr << "Error during read: " << error.message() << std::endl;
    }
}

void extract_server_input_content(std::string str_, uint64_t *id_ptr, uint32_t *x_cord_ptr, uint32_t *y_cord_ptr, uint8_t *type_ptr)
{
    try
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
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
