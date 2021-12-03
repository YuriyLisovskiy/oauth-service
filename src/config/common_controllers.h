/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart.crypto/jwt.h>
#include <xalwart/controllers/controller.h>

// oauth-service
#include "./settings/types.h"


template <typename ...URLArgsT>
class JWTAuthRequiredController : public xw::ctrl::Controller<URLArgsT...>
{
public:
	inline std::unique_ptr<xw::http::IResponse> dispatch(xw::http::IRequest* request, URLArgsT ...args) const
	{
		auto token = this->_retrieve_token(request->get_header("Authorization", ""));
		if (token.empty() || !this->_verify_signature(token))
		{
			return this->get_unauthorized_response();
		}

		auto claims = this->_retrieve_claims(token);
		if (claims.is_null() ||	!this->_verify_issuer(claims[xw::crypto::jwt::iss]))
		{
			return this->get_unauthorized_response();
		}

		if (!this->_verify_audience(claims[xw::crypto::jwt::aud]))
		{
			return this->get_unauthorized_response();
		}

		if (this->_token_expired(claims[xw::crypto::jwt::exp]))
		{
			return this->get_unauthorized_response();
		}

		return xw::ctrl::Controller<URLArgsT...>::dispatch(request, std::forward<URLArgsT>(args)...);
	}

protected:
	inline explicit JWTAuthRequiredController(
		const std::vector<std::string>& allowed_methods,
		const xw::ILogger* logger,
		OAuthConfig oauth_config
	) :
		xw::ctrl::Controller<URLArgsT...>(allowed_methods, logger),
		_oauth_config(std::move(oauth_config))
	{
	}

	[[nodiscard]]
	virtual inline std::unique_ptr<xw::http::IResponse> get_unauthorized_response() const
	{
		return std::make_unique<xw::http::JsonResponse>(nlohmann::json{
			{"message", "Requires authentication"},
			{"documentation_url", "https://github.com/YuriyLisovskiy/oauth-service/blob/master/README.md#api"}
		}, 401);
	}

	[[nodiscard]]
	virtual bool client_exists(const std::string& id) const = 0;

private:
	OAuthConfig _oauth_config;

	[[nodiscard]]
	inline std::string _retrieve_token(const std::string& authorization) const
	{
		auto auth_pair = xw::str::split(xw::str::trim(authorization), ' ', 2);
		if (auth_pair.size() != 2 || auth_pair[0] != this->_oauth_config.JWT.TOKEN_TYPE)
		{
			return "";
		}

		return auth_pair[1];
	}

	[[nodiscard]]
	inline nlohmann::json _retrieve_claims(const std::string& token) const
	{
		try
		{
			return std::get<1>(xw::crypto::jwt::decode(token));
		}
		catch (const xw::ArgumentError&)
		{
		}

		return {nlohmann::json::value_t::null};
	}

	[[nodiscard]]
	inline bool _verify_signature(const std::string& token) const
	{
		try
		{
			return xw::crypto::jwt::verify_signature(this->_oauth_config.SIGNER.get(), token);
		}
		catch (const xw::ArgumentError&)
		{
		}

		return false;
	}

	[[nodiscard]]
	inline bool _verify_issuer(const nlohmann::json& issuer) const
	{
		return !issuer.is_null() && issuer.get<std::string>() == this->_oauth_config.JWT.ISSUER;
	}

	[[nodiscard]]
	inline bool _verify_audience(const nlohmann::json& audience) const
	{
		return !audience.is_null() && this->client_exists(audience.get<std::string>());
	}

	[[nodiscard]]
	inline bool _token_expired(const nlohmann::json& exp) const
	{
		if (exp.is_null())
		{
			return false;
		}

		auto now = xw::dt::Datetime::utc_now();
		return exp.is_number() && exp.get<time_t>() < (time_t)now.timestamp();
	}
};
