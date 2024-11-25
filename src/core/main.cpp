#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

const std::string response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 14\r\n"
    "\r\n"
    "Hello, World!!";

int main()
{
        try
        {

                boost::asio::io_context io_context; // Boost.Asio I/O context

                // acceptor to listen to port 8080
                tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

                std::cout << "Server is running on port 8080..." << std::endl;

                for (;;)
                {
                        tcp::socket socket(io_context); // socket for incoming connection

                        acceptor.accept(socket); // wait for client to connect

                        // read http request
                        boost::asio::streambuf request;
                        boost::asio::read_until(socket, request, "\r\n\r\n");

                        // send response to client
                        boost::asio::write(socket, boost::asio::buffer(response));

                        socket.close();
                }
        }
        catch (const std::exception &e)
        {
                std::cerr << "Error:" << e.what() << '\n';
        }

        return 1;
}