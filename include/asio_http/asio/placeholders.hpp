#if !defined(ASIO_HTTP_ASIO_PLACEHOLDERS_INCLUDED_H_)
#define ASIO_HTTP_ASIO_PLACEHOLDERS_INCLUDED_H_

#include <functional>

namespace asio {
	
	// In standalone mode, ASIO doesn't define placeholders.
	// https://github.com/chriskohlhoff/asio/issues/83
	namespace placeholders {
		
		const decltype(std::placeholders::_1) error{};
		const decltype(std::placeholders::_2) bytes_transferred{};
		const decltype(std::placeholders::_2) iterator{};
		const decltype(std::placeholders::_2) results{};
		const decltype(std::placeholders::_2) endpoint{};
		const decltype(std::placeholders::_2) signal_number{};
		
	}   // end namespace placeholders
	
}   // end namespace asio

#endif /* ASIO_HTTP_ASIO_PLACEHOLDERS_INCLUDED_H_ */