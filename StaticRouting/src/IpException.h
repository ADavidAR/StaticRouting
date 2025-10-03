#pragma once

#include <stdexcept>
#include <string>

class IpException : public std::exception
{
public:
	explicit IpException(const std::string& m): m_msg(m) {}

	const char* what() const noexcept override { return m_msg.c_str(); }

private:
	std::string m_msg;
};