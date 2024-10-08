#include <stdint.h>
#include "object_handler.h"

void object_update(object_t *object_ptr, uint64_t id_, uint32_t x_cord_, uint32_t y_cord_, uint8_t type_)
{
    object_ptr->id = id_;
    object_ptr->x_cord = x_cord_;
    object_ptr->y_cord = y_cord_;
    object_ptr->type = type_;
}