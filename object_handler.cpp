#include "object_handler.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>

/**
 * @brief Initialize an object.
 * @param object_ptr Pointer to object
 */
void object_init(object_t *object_ptr)
{
    uint8_t first_color_byte = 0x5B;
    uint8_t third_color_byte = 0x6D;
    object_ptr->color[0] = first_color_byte;
    object_ptr->color[2] = third_color_byte;
    if (object_ptr->type == 3)
    {
        object_ptr->category = 2;
    }
    else // Type 1 or 2
    {
        object_ptr->category = 1;
    }
}

/**
 * @brief Update object with data.
 * @param object_list Vector with objects.
 * @param id Id for object.
 * @param x_cord X coordinate for object.
 * @param y_cord Y coordinate for object.
 * @param type Object type.
 * @return Index for object in object_list.
 */
uint16_t object_rx_update(std::vector<object_t> &object_list, uint64_t id, uint32_t x_cord, uint32_t y_cord, uint8_t type)
{

    uint16_t index = 0xFFFF;

    for (uint8_t i = 0; i < object_list.size(); i++)
    {
        if (object_list[i].id == id)
        {
            index = i;
        }
    }
    if (index == 0xFFFF || object_list.empty())
    {
        object_list.push_back(object_t(id, x_cord, y_cord, type));
        index = (object_list.size() - 1);
        object_init(&object_list[index]);
    }
    else
    {
        object_list[index].id = id;
        object_list[index].x_cord = x_cord;
        object_list[index].y_cord = y_cord;
        object_list[index].type = type;
    }
    return index;
}

/**
 * @brief Calculate distance to designated location.
 * @param x_cord X coordinate.
 * @param y_cord Y coordinate.
 * @return Distance to designated location.
 */
static float_t object_distance_to_designated(uint32_t x_cord, uint32_t y_cord)
{
    uint8_t designated_cord = 150;
    int16_t x_distance = (x_cord - designated_cord);
    int16_t y_distance = (y_cord - designated_cord);
    float_t distance = sqrt(x_distance * x_distance + y_distance * y_distance);
    return distance;
}

/**
 * @brief Update object color based on distance to designated location.
 * @param object_ptr Pointer to object to update.
 */
void object_color_update(object_t *object_ptr)
{
    float_t distance = object_distance_to_designated(object_ptr->x_cord, object_ptr->y_cord);

    uint8_t color_byte = 0;
    uint8_t red_byte = 0x31;
    uint8_t yellow_byte = 0x33;
    uint8_t blue_byte = 0x34;
    if (object_ptr->category == 2)
    {
        if (distance < 100)
        {
            color_byte = red_byte;
        }
        else
        {
            color_byte = yellow_byte;
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
            color_byte = yellow_byte;
        }
        else
        {
            color_byte = blue_byte;
        }
    }
    object_ptr->color[1] = color_byte;
}