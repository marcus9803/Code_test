#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include <stdint.h>

typedef struct
{
    uint64_t id;
    uint32_t x_cord;
    uint32_t y_cord;
    uint8_t type;
    uint8_t color[3];
    uint8_t category;
} object_t;

void object_update(object_t *object_ptr, uint64_t id_, uint32_t x_cord_, uint32_t y_cord_, uint8_t type_);

#endif // End of OBJECT_HANDLER_H