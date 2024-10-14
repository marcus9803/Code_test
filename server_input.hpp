#ifndef SERVER_INPUT_HPP
#define SERVER_INPUT_HPP

#include <string>
#include <stdint.h>
#include <asio.hpp>
#include <iostream>
#include "object_handler.hpp"

void handle_read(const asio::error_code &error, std::size_t bytes_transferred, std::shared_ptr<asio::streambuf> buffer, asio::ip::tcp::socket &socket, std::vector<object_t> &object_list);
bool extract_server_input_content(std::string str_, uint64_t *id_ptr, uint32_t *x_cord_ptr, uint32_t *y_cord_ptr, uint8_t *type_ptr);

#endif // End of SERVER_INPUT_HPP