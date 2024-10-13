#include <iostream>
#include <string>
#include <stdint.h>
#include "object_handler.hpp"
#include "server_input.hpp"
#include "output_to_client.hpp"

#include <asio.hpp>

object_t object_list[3] = {0};

void handle_read(const asio::error_code &error, std::size_t bytes_transferred, std::shared_ptr<asio::streambuf> buffer, asio::ip::tcp::socket &socket)
{
    if (!error)
    {
        std::istream is(buffer.get());
        std::string str;

        while (std::getline(is, str))
        {
            std::cout << str << std::endl;
        }

        // Continue reading
        asio::async_read_until(socket, *buffer, '\n',
                               [&socket, buffer](const asio::error_code &ec, std::size_t bytes_transferred)
                               {
                                   handle_read(ec, bytes_transferred, buffer, socket);
                               });
    }
    else
    {
        std::cerr << "Error during read: " << error.message() << std::endl;
    }
}

int main()
{
    try
    {
        // std::vector<object_t> object_list;
        asio::io_context io_context;
        asio::ip::tcp::socket socket(io_context);

        // object_init(object_list, object_count);

        socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 5463));

        // Create a buffer to hold the data
        auto buffer = std::make_shared<asio::streambuf>();

        asio::async_read_until(socket, *buffer, '\n',
                               [&socket, buffer](const asio::error_code &ec, std::size_t bytes_transferred)
                               {
                                   handle_read(ec, bytes_transferred, buffer, socket);
                               });

        // TODO: Async acceptor
        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 9090));

        asio::ip::tcp::iostream client_data;
        acceptor.accept(*client_data.rdbuf());

        asio::steady_timer timer(io_context, std::chrono::milliseconds(client_output_time_interval));

        fixed_time_output_to_client(client_data, timer, object_list[0]);

        io_context.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}