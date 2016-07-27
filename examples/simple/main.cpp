#include <iostream>
#include <asio.hpp>
#include <asio_http/http_server.hpp>
#include <asio_http/http_client.hpp>

using namespace asio::ip;

struct http_request_handler
{
	typedef basic_http_connection<http_request_handler> connection;
	void operator()(const connection::pointer & ptr)
	{
		ptr->send_response(200, "Hello world!");
	}
};

struct client_body_handler
{
	typedef void result_type;
	std::string & str_;
	client_body_handler(std::string & str)
	: str_(str)
	{
	}
	void operator()(const std::error_code & ec, const asio::const_buffer & buffer)
	{
		const char * data = asio::buffer_cast<const char *>(buffer);
		std::size_t size = asio::buffer_size(buffer);
		std::string chunk(data, data + size);
		std::cout << "chunk[" << chunk << "]" << std::endl;
		str_ += chunk;
	}
};

struct client_done_handler
{
	std::string & str_;
	client_done_handler(std::string & str)
	: str_(str)
	{
	}
	void operator()(const std::error_code & ec)
	{
		std::cout << "done [" << str_ << "]" << std::endl;
	}
};

typedef	http_client_connection<
http_client::protocol_type,
client_body_handler,
client_done_handler> client_connection;

int
main(int argc, char * argv[])
{
	asio::io_service io_svc;
	
	// Server
	http_server<http_request_handler> server(io_svc, tcp::endpoint(tcp::v4(), 5000));
	
	// Client
	http_client & client = asio::use_service<http_client>(io_svc);
	std::string data;
	client_body_handler body(data);
	client_done_handler done(data);
	client_connection::pointer connection = client.create_request("http://127.0.0.1:5000/", body, done);
	connection->start();
	
	
	io_svc.run();
	return 0;
}