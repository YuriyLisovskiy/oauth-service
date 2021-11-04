/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart/http/exceptions.h>


class UserNotFoundException : public xw::http::exc::NotFound
{
public:
	explicit inline UserNotFoundException(
		long long int id, int line=0, const char* function="", const char* file=""
	) : xw::http::exc::NotFound(
		"user with id '" + std::to_string(id) + "' is not found", line, function, file
	)
	{
	}
};
