#if !defined(ASIO_HTTP_HTTP_CLIENT_CONNECTION_INCLUDED_H_)
#define ASIO_HTTP_HTTP_CLIENT_CONNECTION_INCLUDED_H_

#include <memory>
#include <system_error>
#include <asio.hpp>
#include <asio_http/asio/placeholders.hpp>

#include "detail/headers.hpp"

template <typename T, typename BodyHandler, typename DoneHandler>
struct http_client_connection
	: std::enable_shared_from_this<
		http_client_connection<
			T,
			BodyHandler,
			DoneHandler
		>
	>
{
	typedef std::shared_ptr<http_client_connection> pointer;
	http_client_connection(asio::io_service & io_service,
		std::string url,
		const Headers& headers,
		BodyHandler body_handler, DoneHandler done_handler);
	~http_client_connection();
	void start();
	void resolve_handler(const std::error_code& ec,
		asio::ip::tcp::resolver::iterator i);
	void connect_handler(const std::error_code& ec,
		asio::ip::tcp::resolver::iterator i);
	void start_read();
	void write_handler(const std::error_code& error,
		std::size_t bytes_transferred);
	void read_handler(const std::error_code& error,
		std::size_t bytes_transferred);
	static int on_body(http_parser * parser, const char * at, size_t length);
	static int on_message_complete(http_parser * parser);
	static int on_status(http_parser * parser, const char * at, size_t length);
	asio::io_service & io_service_;
	asio::ip::tcp::resolver resolver_;
	T socket_;
	std::string url_;
	Headers headers_;
	http_parser_url parsed_url_;
	BodyHandler body_handler_;
	DoneHandler done_handler_;
	asio::streambuf request_buffer_;
	asio::streambuf response_buffer_;
	http_parser_settings settings_;
	http_parser parser_;
	/**
	 * Status string. For example "OK"
	 */
	std::string status_;
	inline
	unsigned int get_status_code() const
	{
		assert(parser_.type == HTTP_RESPONSE);
		return parser_.status_code;
	}
	inline
	const std::string & get_status() const
	{
		return status_;
	}
};

#include "http_client_connection-inl.hpp"

#endif
