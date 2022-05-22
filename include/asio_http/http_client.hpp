#if !defined(ASIO_HTTP_HTTP_CLIENT_INCLUDED_H_)
#define ASIO_HTTP_HTTP_CLIENT_INCLUDED_H_

#include <asio.hpp>
#include "http_client_connection.hpp"

template <typename Protocol>
struct basic_http_client
	: asio::io_service::service
{
	static asio::io_service::id id;
	typedef Protocol protocol_type;
	basic_http_client(asio::io_service & io_service);
	void shutdown_service();
	template <typename BodyHandler, typename DoneHandler>
	std::shared_ptr<
		http_client_connection<
			Protocol,
			BodyHandler,
			DoneHandler
		>
	> create_request(const std::string & url, const Headers & headers,
			             BodyHandler body_handler, DoneHandler done_handler);
};

#include "http_client-inl.hpp"

typedef basic_http_client<asio::ip::tcp::socket> http_client;

#endif
