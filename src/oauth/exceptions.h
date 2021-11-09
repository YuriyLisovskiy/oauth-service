/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart/http/exceptions.h>


class OAuthError : public xw::http::exc::HttpError
{
public:
	class Value final
	{
	public:
		enum InternalValue
		{
			invalid_request = 0,
			invalid_client,
			invalid_grant,
			invalid_scope,
			unauthorized_client,
			unsupported_grant_type
		};

		Value() = default;

		constexpr inline Value(InternalValue value) : _value(value)
		{
		}

		inline operator InternalValue() const
		{
			return this->_value;
		}

		explicit operator bool() = delete;

		constexpr inline bool operator== (Value a) const
		{
			return this->_value == a._value;
		}

		constexpr inline bool operator!= (Value a) const
		{
			return this->_value != a._value;
		}

		[[nodiscard]]
		inline std::string to_string() const
		{
			switch (this->_value) {
				case Value::invalid_request:
					return "invalid_request";
				case Value::invalid_client:
					return "invalid_client";
				case Value::invalid_grant:
					return "invalid_grant";
				case Value::invalid_scope:
					return "invalid_scope";
				case Value::unauthorized_client:
					return "unauthorized_client";
				case Value::unsupported_grant_type:
					return "unsupported_grant_type";
				default:
					throw xw::ValueError("invalid 'OAuthError::Value' option", _ERROR_DETAILS_);
			}
		}

		[[nodiscard]]
		inline unsigned short int to_status_code() const
		{
			if (this->_value == Value::invalid_client)
			{
				return 401;
			}

			return 400;
		}

	private:
		InternalValue _value;
	};

	explicit inline OAuthError(
		Value value, const std::string& error_description,
		int line=0, const char* function="", const char* file=""
	) : xw::http::exc::HttpError(
		value.to_status_code(), error_description, line, function, file
	)
	{
		this->_value = value;
	}

	[[nodiscard]]
	inline Value get_error_value() const
	{
		return this->_value;
	}

private:
	Value _value;
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
