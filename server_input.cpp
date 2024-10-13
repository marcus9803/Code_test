#include "server_input.hpp"

#include <stdint.h>
#include <string>
#include <regex>
#include <asio.hpp>
#include <iostream>
#include "object_handler.hpp"

void read_server_input(asio::ip::tcp::iostream &server_data_, asio::io_context &io_context_, object_t object_list_[])
{
    auto read_timer = std::make_shared<asio::steady_timer>(io_context_);
    // auto str = std::make_shared<std::string>();
    uint64_t id = 0;
    uint32_t x_cord = 0, y_cord = 0;
    uint8_t type = 0;

    std::function<void()> read_data = [&, read_timer]()
    {
        std::string str;
        if (std::getline(server_data_, str))
        {
            extract_server_input_content(str, &id, &x_cord, &y_cord, &type);
            printf("%d \n", x_cord);
            // std::cout << str << std::endl;
            //  Schedule the next read immediately
            read_timer->expires_after(std::chrono::milliseconds(10));
            read_timer->async_wait([&, read_timer](const asio::error_code &error)
                                   {
                /* extract_server_input_content(*str, &id, &x_cord, &y_cord, &type);
                object_rx_update(&object_list_[type - 1], id, x_cord, y_cord);
                object_color_update(&object_list_[type - 1]); */
                if (!error)
                {
                    read_data();
                } });
        }
        else
        {
            std::cerr << "Failed to read from server. Stopping." << std::endl;
        }
    };
    read_data();
}

void extract_server_input_content(std::string str_, uint64_t *id_ptr, uint32_t *x_cord_ptr, uint32_t *y_cord_ptr, uint8_t *type_ptr)
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
