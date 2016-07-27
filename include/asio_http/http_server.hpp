#if !defined(ASIO_HTTP_HTTP_SERVER_INCLUDED_H_)
#define ASIO_HTTP_HTTP_SERVER_INCLUDED_H_

#include <functional>
#include <system_error>
#include <asio.hpp>
#include <asio_http/http_server_connection.hpp>
#include <asio_http/aux/logging.hpp>
#include <asio_http/asio/placeholders.hpp>

/**
 * HTTP server implementation
 */
template <typename RequestHandler>
class http_server
{
private:
	asio::io_service & io_svc_;
	/**
	 * It waits for sockets
	 */

	asio::ip::tcp::acceptor acceptor_;
	RequestHandler request_handler_;
public:
	typedef basic_http_connection<RequestHandler> connection_type;
	http_server(asio::io_service & io_svc,
				asio::ip::tcp::endpoint endpoint_,
				RequestHandler request_handler = RequestHandler());
	/**
	 * Start asynchronous accept.
	 */
	void start_accept();
	/**
	 * Stop accepting new connections
	 */
	void stop_accept();
	/**
	 * New client connected
	 */
	void handle_accept(typename connection_type::pointer new_connection,
					   const std::error_code& error);
	void handle_request(typename connection_type::pointer connection);
	inline asio::ip::tcp::acceptor & get_acceptor()
	{
		return acceptor_;
	}
};

#include "http_server-inl.hpp"

#endif /* ASIO_HTTP_HTTP_SERVER_INCLUDED_H_ */