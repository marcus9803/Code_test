#include "object_handler.hpp"

#include <stdint.h>
#include "stdio.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>

uint8_t red_byte = 0x31;
uint8_t yellow_byte = 0x33;
uint8_t blue_byte = 0x34;
uint8_t first_color_byte = 0x5B;
uint8_t third_color_byte = 0x6D;

static float_t object_distance_to_designated(uint32_t x_cord_, uint32_t y_cord_);

void object_init(object_t *object_ptr)
{
    object_ptr->color[0] = first_color_byte;
    object_ptr->color[2] = third_color_byte;
    if (object_ptr->type == 3)
    {
        object_ptr->category = 2;
    }
    else
    {
        object_ptr->category = 1;
    }
}

uint16_t object_rx_update(std::vector<object_t> &object_list_, uint64_t id_, uint32_t x_cord_, uint32_t y_cord_, uint8_t type_)
{

    uint16_t index = 0xFFFF;

    for (uint8_t i = 0; i < object_list_.size(); i++)
    {
        if (object_list_[i].id == id_)
        {
            index = i;
        }
    }
    if (index == 0xFFFF || object_list_.empty())
    {
        object_list_.push_back(object_t(id_, x_cord_, y_cord_, type_));
        index = (object_list_.size() - 1);
        object_init(&object_list_[index]);
    }
    else
    {
        object_list_[index].id = id_;
        object_list_[index].x_cord = x_cord_;
        object_list_[index].y_cord = y_cord_;
        object_list_[index].type = type_;
    }
    return index;
}

static float_t object_distance_to_designated(uint32_t x_cord_, uint32_t y_cord_)
{
    uint8_t designated_cord = 150;
    int16_t x_distance = (x_cord_ - designated_cord);
    int16_t y_distance = (y_cord_ - designated_cord);
    float_t distance = sqrt(x_distance * x_distance + y_distance * y_distance);
    return distance;
}

void object_color_update(object_t *object_ptr)
{
    float_t distance = object_distance_to_designated(object_ptr->x_cord, object_ptr->y_cord);

    uint8_t color_byte = 0;
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