#if !defined(ASIO_HTTP_HTTP_SERVER_CONNECTION_H_INCLUDED_)
#define ASIO_HTTP_HTTP_SERVER_CONNECTION_H_INCLUDED_

#include <string>
#include <memory>
#include <map>
#include <system_error>
#include <asio.hpp>
#include <asio_http/aux/logging.hpp>
#include <asio_http/asio/placeholders.hpp>
#include "http_parser.h"
#include "detail/status_codes.hpp"

template <typename SocketType>
class basic_http_connection
	: public std::enable_shared_from_this<basic_http_connection<SocketType> >
{
public:
	typedef std::shared_ptr<basic_http_connection<SocketType> > pointer;

	static pointer create(asio::io_service& io_service, SocketType * handler)
	{
		return pointer(new basic_http_connection(io_service, handler));
	}

	asio::ip::tcp::socket& get_socket()
	{
		return socket_;
	}
	void start();
	http_parser_settings settings_;
	/**
	 * Send HTTP response.
	 */
	void send_response(int status_code, std::string message);
	inline const std::string & get_request_url() const
	{
		return request_url_;
	}
	typedef std::multimap<std::string, std::string> headers_type;
	const headers_type & get_headers() const
	{
		return headers_;
	}
	int get_request_method() const
	{
		return parser_.method;
	}
	const std::string & get_request_body() const
	{
		return request_body_;
	}
	~basic_http_connection();
private:
	SocketType * handler_;
	basic_http_connection(asio::io_service& io_service,
		SocketType * handler);

	void handle_write(const std::error_code& error_code /*error*/,
		size_t /*bytes_transferred*/);
	asio::ip::tcp::socket socket_;
	/*
	 * HTTP stuff
	 */
	http_parser parser_;
	static int on_message_begin(http_parser * parser);
	static int on_header_field(http_parser * parser, const char * at, size_t length);
	static int on_header_value(http_parser * parser, const char * at, size_t length);
  	static int on_headers_complete(http_parser * parser);
  	std::string request_body_;
  	static int on_body(http_parser * parser, const char * at, size_t length);
	/**
	 * HTTP parser encountered something that appears to be URL.
	 * This callback might be called multiple times, but in our
	 * case this gets called just once.
	 */
	static int on_url(http_parser* parser, const char *at, size_t length);
	/**
	 * Received complete HTTP request
	 */
	static int on_message_complete(http_parser * parser);
	/**
	 * Temporary socket data is stored here.
	 */
	asio::streambuf buffer_;
	/**
	 * Temporary buffer for static responses
	 */
	asio::streambuf outgoing_buffer_;
	/**
	 * Received HTTP header.
	 */
	void handler(const std::error_code& e, std::size_t size);
	/**
	 * Executes request handler after successful request
	 */
	void process_request();
	/**
	 * Full URL of the current request
	 */
	std::string request_url_;
	enum
	{
		HEADER_START,
		HEADER_FIELD,
		HEADER_VALUE
	};
	int header_state_;
	std::string header_field_;
	std::string header_value_;
	headers_type headers_;
};

#include "http_server_connection-inl.hpp"

typedef basic_http_connection<asio::ip::tcp::socket> http_connection;

#endif /* ASIO_HTTP_HTTP_SERVER_CONNECTION_H_INCLUDED_ */
