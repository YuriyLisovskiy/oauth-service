/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart/http/exceptions.h>


class ValidationError : public xw::http::exc::BadRequest
{
public:
	explicit inline ValidationError(
		const std::string& message, int line=0, const char* function="", const char* file=""
	) : xw::http::exc::BadRequest(message, line, function, file)
	{
	}
};
