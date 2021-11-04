/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart.base/interfaces/base.h>


// TODO: tests required
struct Token : public xw::IJsonSerializable
{
	std::string access_token;
	std::string token_type;
	time_t expires_in{};

	Token() = default;

	inline Token(std::string access_token, std::string token_type, time_t expires_in) :
		access_token(std::move(access_token)), token_type(std::move(token_type)), expires_in(expires_in)
	{
	}

	[[nodiscard]]
	nlohmann::json to_json() const override
	{
		return {
			{"access_token", this->access_token},
			{"token_type", this->token_type},
			{"expires_in", this->expires_in}
		};
	}
};
