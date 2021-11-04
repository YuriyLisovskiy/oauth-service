/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart/http/exceptions.h>


class IncorrectClientSecretError : public xw::http::exc::BadRequest
{
public:
	explicit inline IncorrectClientSecretError(
		const std::string& client_id, int line=0, const char* function="", const char* file=""
	) : xw::http::exc::BadRequest(
		"secret key is incorrect for client with id '" + client_id + "'", line, function, file
	)
	{
	}
};

class ClientNotFoundError : public xw::http::exc::NotFound
{
public:
	explicit inline ClientNotFoundError(
		const std::string& id, int line=0, const char* function="", const char* file=""
	) : xw::http::exc::NotFound(
		"client with id '" + id + "' is not found", line, function, file
	)
	{
	}
};
