#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include "object_handler.hpp"
#include "server_input.hpp"

#include <asio.hpp>

const uint8_t type_count = 3;

object_t object_list[type_count] = {0};

int main()
{
    uint8_t object_count = sizeof(object_list) / sizeof(object_list[0]);
    object_init(object_list, object_count);

    uint64_t id = 0;
    uint32_t x_cord = 0, y_cord = 0;
    uint8_t type = 0;

    asio::ip::tcp::iostream input("localhost", "5463");
    std::string str;
    std::getline(input, str);
    read_server_input(str, &id, &x_cord, &y_cord, &type);

    printf("%llu \n", id);
    printf("%lu \n", x_cord);
    printf("%lu \n", y_cord);
    printf("%lu \n", type);

    object_rx_update(&object_list[type - 1], id, x_cord, y_cord);
    object_color_update(&object_list[type - 1]);

    printf("%x \n", object_list[type - 1].color[0]);
    printf("%x \n", object_list[type - 1].color[1]);
    printf("%x \n", object_list[type - 1].color[2]);
}