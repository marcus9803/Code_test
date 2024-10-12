#include <iostream>
#include <string>
#include <stdint.h>
#include "object_handler.hpp"
#include "server_input.hpp"

#include <asio.hpp>

const uint8_t type_count = 3;

object_t object_list[type_count] = {0};

int main()
{
    uint8_t object_count = sizeof(object_list) / sizeof(object_list[0]);
    uint64_t id = 0;
    uint32_t x_cord = 0, y_cord = 0;
    uint8_t type = 0;

    object_init(object_list, object_count);

    asio::ip::tcp::iostream input("localhost", "5463"); // TODO: Check for errors
    std::string str;
    std::getline(input, str);
    extract_server_input_content(str, &id, &x_cord, &y_cord, &type);

    printf("%llu \n", id);
    printf("%lu \n", x_cord);
    printf("%lu \n", y_cord);
    printf("%lu \n", type);

    object_rx_update(&object_list[type - 1], id, x_cord, y_cord);
    object_color_update(&object_list[type - 1]);

    printf("%x \n", object_list[type - 1].color[0]);
    printf("%x \n", object_list[type - 1].color[1]);
    printf("%x \n", object_list[type - 1].color[2]);

    asio::io_context io_context;
    asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 9090));

    asio::ip::tcp::iostream client_data;
    acceptor.accept(*client_data.rdbuf());

    client_data << "Hello, client!\n";
}