#ifndef OBJECT_HANDLER_HPP
#define OBJECT_HANDLER_HPP

#include <stdint.h>
#include <vector>

const uint8_t color_n_bytes = 3;

struct object_t
{
    uint64_t id;
    uint32_t x_cord;
    uint32_t y_cord;
    uint8_t type;
    uint8_t color[color_n_bytes];
    uint8_t category;
    object_t(uint64_t id, uint32_t x, uint32_t y, uint8_t t)
        : id(id), x_cord(x), y_cord(y), type(t) {}
};

void object_init(object_t *object_ptr);
void object_color_update(object_t *object_ptr);
uint16_t object_rx_update(std::vector<object_t> &object_list, uint64_t id, uint32_t x_cord, uint32_t y_cord, uint8_t type);

#endif // End of OBJECT_HANDLER_HPP