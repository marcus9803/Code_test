#include "output_to_client.hpp"

#include <iostream>
#include <asio.hpp>
#include <vector>
#include <cstring>
#include "object_handler.hpp"

const uint16_t client_output_time_interval = 1667;

struct preamble_t
{
    uint32_t preamble;
    uint32_t object_count;
};

preamble_t client_preamble = {0xFE00, 0};

static std::vector<uint8_t> serialize_preamble(preamble_t &input)
{
    std::vector<uint8_t> buffer(sizeof(preamble_t));
    std::memcpy(buffer.data(), &input, sizeof(preamble_t));
    return buffer;
}

static std::vector<uint8_t> serialize_object(object_t &object)
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
}

void fixed_time_output_to_client(asio::ip::tcp::iostream &client_data, asio::steady_timer &timer, std::vector<object_t> &object_list_)
{
    // Write the preamble data in binary form to the client stream
    client_preamble.object_count = object_list_.size();
    std::vector<uint8_t> preamble_data = serialize_preamble(client_preamble);
    client_data.write(reinterpret_cast<const char *>(preamble_data.data()), preamble_data.size());

    // Write each object in binary form  to the client stream
    std::vector<uint8_t> binary_data;
    for (uint16_t i = 0; i < object_list_.size(); i++)
    {
        binary_data = serialize_object(object_list_[i]);
        client_data.write(reinterpret_cast<const char *>(binary_data.data()), binary_data.size());
    }

    timer.expires_after(std::chrono::milliseconds(client_output_time_interval));

    timer.async_wait([&client_data, &timer, &object_list_](const asio::error_code &error)
                     {
        if (!error) {
            fixed_time_output_to_client(client_data, timer, object_list_);
        }
        else
        {
            std::clog << "Error in output to client: " << error.message() << std::endl;
        } });
}