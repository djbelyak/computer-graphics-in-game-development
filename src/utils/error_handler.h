#pragma once

#include <stdexcept>
#include <string>

#define THROW_ERROR(x)                            \
	{                                             \
		std::string message(x);                   \
		message.append(" (at ")                   \
				.append(__FILE__)                 \
				.append(":")                      \
				.append(std::to_string(__LINE__)) \
				.append(")")                      \
				.append("\n");                    \
		throw std::runtime_error(message);        \
	}
