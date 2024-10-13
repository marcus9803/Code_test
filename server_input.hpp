
#include <string>
#include <stdint.h>
#include <asio.hpp>
#include <iostream>
#include "object_handler.hpp"

void read_server_input(asio::ip::tcp::iostream &server_data_, asio::io_context &io_context_);

void extract_server_input_content(std::string str_, uint64_t *id_ptr, uint32_t *x_cord_ptr, uint32_t *y_cord_ptr, uint8_t *type_ptr);