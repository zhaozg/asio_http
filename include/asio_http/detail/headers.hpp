#if !defined(ASIO_HTTP_DETAIL_HEADERS_INCLUDED_H_)
#define ASIO_HTTP_DETAIL_HEADERS_INCLUDED_H_

#include <vector>
#include <string>
#include <ostream>

inline bool iequals(const std::string& a, const std::string& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

struct Header
{
  std::string name;
  std::string value;
	Header(const std::string &name,const std::string &value)
		: name(name), value(value) {};
	Header(const char* name, const char * value)
		: name(name), value(value) {};

	bool is(const char*name)
	{
		std::string s(name);
		return is(s);
	}

	bool is(const std::string &name)
	{
		return iequals(this->name, name);
	}

	std::string str()
	{
		return name+": "+value;
	}
};

inline std::ostream& operator<<(std::ostream& os, const Header& header)
{
	os << header.name << ": " << header.value << "\r\n";
	return os;
}

class Headers: public std::vector<Header>
{
public:
	Headers() {};
	virtual ~Headers() {};

public:
	void push_back( const std::string& name, const std::string &value )
	{
		const Header hdr(name, value);
		std::vector<Header>::push_back(hdr);
	}

	void push_back( const char* name, const char *value )
	{
		const Header hdr(name, value);
		std::vector<Header>::push_back(hdr);
	}

	Headers::iterator erase(const char* name)
	{
		const std::string s(name);
		return erase(s);
	}

	Headers::iterator erase(const std::string &name)
	{
		Headers::iterator i = begin();
		while(!i->is(name)) i++;

		i = std::vector<Header>::erase(i);
		while(i!=end() && !i->is(name)) i++;
		return i;
	}
};

inline std::ostream& operator<<(std::ostream& os, const Headers& headers)
{
	for(Headers::const_iterator i = headers.cbegin(); i != headers.cend(); i++)
	{
		os << *i;
	}
	os << "\r\n";
	return os;
}


#endif /* ASIO_HTTP_DETAIL_HEADERS_INCLUDED_H_ */

