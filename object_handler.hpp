#ifndef OBJECT_HANDLER_HPP
#define OBJECT_HANDLER_HPP

#include <stdint.h>

const uint8_t color_n_bytes = 3;

typedef struct
{
    uint64_t id;
    uint32_t x_cord;
    uint32_t y_cord;
    uint8_t type;
    uint8_t color[color_n_bytes];
    uint8_t category;
} object_t;

void object_init(object_t object_list_[], uint8_t object_count_);
void object_color_update(object_t *object_ptr);
void object_rx_update(object_t *object_ptr, uint64_t id_, uint32_t x_cord_, uint32_t y_cord_);

#endif // End of OBJECT_HANDLER_HPP