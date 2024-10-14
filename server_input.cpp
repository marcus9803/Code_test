#include "server_input.hpp"

#include <stdint.h>
#include <string>
#include <regex>
#include <asio.hpp>
#include <iostream>
#include "object_handler.hpp"

void handle_read(const asio::error_code &error, std::size_t bytes_transferred, std::shared_ptr<asio::streambuf> buffer, asio::ip::tcp::socket &socket, std::vector<object_t> &object_list)
{
    if (!error)
    {
        std::istream is(buffer.get());
        std::string str;

        uint64_t id = 0;
        uint32_t x_cord = 0, y_cord = 0;
        uint8_t type = 0;

        while (std::getline(is, str))
        {
            if (extract_server_input_content(str, &id, &x_cord, &y_cord, &type))
            {
                uint16_t index = object_rx_update(object_list, id, x_cord, y_cord, type);
                object_color_update(&object_list[index]);
            }
        }

        // Continue reading
        asio::async_read_until(socket, *buffer, '\n',
                               [&socket, buffer, &object_list](const asio::error_code &ec, std::size_t bytes_transferred)
                               {
                                   handle_read(ec, bytes_transferred, buffer, socket, object_list);
                               });
    }
    else
    {
        std::clog << "Error during read from server: " << error.message() << std::endl;
    }
}

bool extract_server_input_content(std::string str, uint64_t *id_ptr, uint32_t *x_cord_ptr, uint32_t *y_cord_ptr, uint8_t *type_ptr)
{
    try
    {
        if (str.empty())
        {
            std::clog << "Error: Input string is empty." << std::endl;
            return false;
        }
        std::regex delimiter("[;=]"); // Regular expression for delimiters

        std::sregex_token_iterator tokenIt(str.begin(), str.end(), delimiter, -1);
        std::sregex_token_iterator end;

        std::vector<std::string> tokens;
        for (; tokenIt != end; ++tokenIt)
        {
            if (tokenIt->str().empty())
            {
                std::clog << "Error: Content missing in read from server." << std::endl;
                return false;
            }
            tokens.push_back(*tokenIt); // Add each token to the vector
        }

        if (tokens.size() < 8)
        {
            std::clog << "Error: Content missing in read from server." << std::endl;
            return false;
        }

        *id_ptr = std::stoull(tokens[1]);
        *x_cord_ptr = std::stoul(tokens[3]);
        *y_cord_ptr = std::stoul(tokens[5]);
        uint64_t type_value = std::stoull(tokens[7]);
        if (type_value > std::numeric_limits<uint8_t>::max())
        {
            std::clog << "Error in server input or read from server." << std::endl;
        }
        *type_ptr = static_cast<uint8_t>(type_value);
        return true;
    }
    catch (const std::exception &e)
    {
        std::clog << "Error server extract: " << e.what() << std::endl;
        return false;
    }
}
