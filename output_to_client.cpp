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

/**
 * @brief Split input into vector with bytes for the preamble type elements.
 * @param input Preamble object.
 * @return Vector with bytes
 */
static std::vector<uint8_t> serialize_preamble(preamble_t &input)
{
    std::vector<uint8_t> buffer(sizeof(preamble_t));
    std::memcpy(buffer.data(), &input, sizeof(preamble_t));
    return buffer;
}

/**
 * @brief Split input into vector with bytes for the object type elements. Exclude category.
 * @param object Object
 * @return Vector with bytes.
 */
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

/**
 * @brief Output a preamble followed by each object in binary form to the client.
 * @param client_data Client connection.
 * @param timer Timer
 * @param object_list Vector with objects to send to client.
 */
void fixed_time_output_to_client(asio::ip::tcp::iostream &client_data, asio::steady_timer &timer, std::vector<object_t> &object_list)
{
    // Write the preamble data in binary form to the client stream
    client_preamble.object_count = object_list.size();
    std::vector<uint8_t> preamble_data = serialize_preamble(client_preamble);
    client_data.write(reinterpret_cast<const char *>(preamble_data.data()), preamble_data.size());

    // Write each object in binary form  to the client stream
    std::vector<uint8_t> binary_data;
    for (uint16_t i = 0; i < object_list.size(); i++)
    {
        binary_data = serialize_object(object_list[i]);
        client_data.write(reinterpret_cast<const char *>(binary_data.data()), binary_data.size());
    }

    // Set up the timer to expire after x ms.
    timer.expires_after(std::chrono::milliseconds(client_output_time_interval));

    // Asyncronous wait until timer expires.
    timer.async_wait([&client_data, &timer, &object_list](const asio::error_code &error)
                     {
        if (!error) {
            fixed_time_output_to_client(client_data, timer, object_list);
        }
        else
        {
            std::clog << "Error in output to client: " << error.message() << std::endl;
        } });
}