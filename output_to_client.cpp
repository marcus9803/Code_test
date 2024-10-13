#include "output_to_client.hpp"

#include <iostream>
#include <asio.hpp>
#include <vector>
#include <cstring>
#include "object_handler.hpp"

const uint16_t client_output_time_interval = 1667;

std::vector<uint8_t> serialize_object(object_t &obj)
{
    std::vector<uint8_t> buffer(sizeof(object_t));
    std::memcpy(buffer.data(), &obj, sizeof(object_t));
    return buffer;
}

void fixed_time_output_to_client(asio::ip::tcp::iostream &client_data, asio::steady_timer &timer, std::vector<object_t> &object_list_)
{
    for (uint16_t i = 0; i < object_list_.size(); i++)
    {
        auto binary_data = serialize_object(object_list_[i]);
        client_data.write(reinterpret_cast<const char *>(binary_data.data()), binary_data.size());
    }

    // Write the binary data to the stream

    // client_data.flush(); // Is this necessary?

    timer.expires_after(std::chrono::milliseconds(client_output_time_interval));

    timer.async_wait([&client_data, &timer, &object_list_](const asio::error_code &error)
                     {
        if (!error) {
            fixed_time_output_to_client(client_data, timer, object_list_);
        } });
}