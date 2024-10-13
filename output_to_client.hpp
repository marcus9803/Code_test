#include <iostream>
#include <asio.hpp>
#include "object_handler.hpp"

extern const uint16_t client_output_time_interval;

void fixed_time_output_to_client(asio::ip::tcp::iostream &client_data, asio::steady_timer &timer, std::vector<object_t> &object_list_);