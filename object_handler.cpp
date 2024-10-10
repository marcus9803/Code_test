#include <stdint.h>
#include "object_handler.hpp"
#include <stdlib.h>
#include <math.h>

uint8_t red_byte = 0x31;
uint8_t yellow_byte = 0x33;
uint8_t blue_byte = 0x34;
uint8_t first_color_byte = 0x5B;
uint8_t third_color_byte = 0x6D;

uint8_t designated_cord = 150;

// static int16_t object_distance_to_centre(uint32_t x_cord_, uint32_t y_cord_);

void object_init(object_t object_list_[], uint8_t object_count_)
{
    for (uint8_t i = 0; i < object_count_; i++)
    {
        object_list_[i].type = i + 1;
        object_list_[i].color[0] = first_color_byte;
        object_list_[i].color[2] = third_color_byte;
        if (object_list_[i].type == 3)
        {
            object_list_[i].category = 2;
        }
        else
        {
            object_list_[i].category = 1;
        }
    }
}

void object_rx_update(object_t *object_ptr, uint64_t id_, uint32_t x_cord_, uint32_t y_cord_)
{
    object_ptr->id = id_;
    object_ptr->x_cord = x_cord_;
    object_ptr->y_cord = y_cord_;
}

static float_t object_distance_to_centre(uint32_t x_cord_, uint32_t y_cord_)
{
    int16_t x_distance = (x_cord_ - designated_cord);
    int16_t y_distance = (y_cord_ - designated_cord);
    float_t distance = sqrt(x_distance * x_distance + y_distance * y_distance);
    return distance;
}

void object_color_update(object_t *object_ptr)
{
    float_t distance = object_distance_to_centre(object_ptr->x_cord, object_ptr->y_cord);

    uint8_t color_byte = 0;
    if (object_ptr->category == 2)
    {
        if (distance >= 100)
        {
            color_byte = yellow_byte;
        }
        else
        {
            color_byte = red_byte;
        }
    }
    else if (object_ptr->type == 1)
    {
        if (distance < 50)
        {
            color_byte = red_byte;
        }
        else if (distance < 75)
        {
            color_byte = yellow_byte;
        }
        else
        {
            color_byte = blue_byte;
        }
    }
    else
    {
        if (distance < 50)
        {
            color_byte = blue_byte;
        }
        else
        {
            color_byte = yellow_byte;
        }
    }
    object_ptr->color[1] = color_byte;
}