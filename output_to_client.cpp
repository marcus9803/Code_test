#include "output_to_client.hpp"

#include <iostream>
#include <asio.hpp>
#include <vector>
#include <cstring>
#include "object_handler.hpp"

const uint16_t client_output_time_interval = 1667;
uint32_t preamble = 0xFE00;

static std::vector<uint8_t> serialize_object(object_t &obj)
{
    std::vector<uint8_t> buffer(sizeof(object_t));
    std::memcpy(buffer.data(), &obj, sizeof(object_t));
    return buffer;
}

static std::vector<uint8_t> serialize_uint32_t(uint32_t &input)
{
    std::vector<uint8_t> buffer(sizeof(uint32_t));
    std::memcpy(buffer.data(), &input, sizeof(uint32_t));
    return buffer;
}

/* std::vector<uint8_t> serialize_object(const object_t &object)
{
    std::vector<uint8_t> buffer;

    buffer.reserve(sizeof(object.id) + sizeof(object.x_cord) + sizeof(object.y_cord) + sizeof(object.type) + sizeof(object.color));

    auto append_to_buffer = [&buffer](const void *data, size_t size)
    {
        const uint8_t *byte_data = reinterpret_cast<const uint8_t *>(data);
        buffer.insert(buffer.end(), byte_data, byte_data + size);
    };

    append_to_buffer(&object.id, sizeof(object.id));
    append_to_buffer(&object.x_cord, sizeof(object.x_cord));
    append_to_buffer(&object.y_cord, sizeof(object.y_cord));
    append_to_buffer(&object.type, sizeof(object.type));
    append_to_buffer(object.color, sizeof(object.color));

    return buffer;
} */

void fixed_time_output_to_client(asio::ip::tcp::iostream &client_data, asio::steady_timer &timer, std::vector<object_t> &object_list_)
{
    auto binary_data = serialize_uint32_t(preamble);
    client_data.write(reinterpret_cast<const char *>(binary_data.data()), binary_data.size());

    uint32_t object_count = object_list_.size();
    binary_data = serialize_uint32_t(object_count);
    client_data.write(reinterpret_cast<const char *>(binary_data.data()), binary_data.size());

    for (uint16_t i = 0; i < object_list_.size(); i++)
    {
        binary_data = serialize_object(object_list_[i]);
        client_data.write(reinterpret_cast<const char *>(binary_data.data()), binary_data.size());
    }

    // Write the binary data to the stream

    // client_data.flush();

    timer.expires_after(std::chrono::milliseconds(client_output_time_interval));

    timer.async_wait([&client_data, &timer, &object_list_](const asio::error_code &error)
                     {
        if (!error) {
            fixed_time_output_to_client(client_data, timer, object_list_);
        } });
}