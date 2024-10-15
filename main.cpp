#include <iostream>
#include <string>
#include <stdint.h>
#include "object_handler.hpp"
#include "server_input.hpp"
#include "output_to_client.hpp"
#include <fstream>
#include <asio.hpp>

int main()
{
    try
    {
        std::ofstream log_file("errorlog.txt");
        std::clog.rdbuf(log_file.rdbuf());

        std::vector<object_t> object_list;
        asio::io_context io_context;
        asio::ip::tcp::socket socket(io_context);

        socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 5463));

        // Create a buffer to hold the data
        auto buffer = std::make_shared<asio::streambuf>();

        // Asyncronous read from server with callback when read complete.
        asio::async_read_until(socket, *buffer, '\n',
                               [&socket, buffer, &object_list](const asio::error_code &ec, std::size_t bytes_transferred)
                               {
                                   handle_read(ec, bytes_transferred, buffer, socket, object_list);
                               });

        // Improvement: Async acceptor for multiple clients
        // Set up an acceptor for clients to connect via port 9090
        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 9090));

        asio::ip::tcp::iostream client_data;
        acceptor.accept(*client_data.rdbuf());

        // Set up a timer for output to client
        asio::steady_timer timer(io_context, std::chrono::milliseconds(client_output_time_interval));

        fixed_time_output_to_client(client_data, timer, object_list);

        io_context.run(); // Required for asynchronous asio operations
    }
    catch (const std::exception &e)
    {
        std::clog << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}