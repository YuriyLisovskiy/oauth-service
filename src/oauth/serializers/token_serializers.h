/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <memory>

// xalwart
#include <xalwart.crypto/jwt.h>

// oauth-service
#include "../../config/common_serializers.h"
#include "../services/client_service.h"
#include "../models/token.h"
#include "../exceptions.h"


class CreateTokenSerializer : public ModelSerializer<Token, std::string, std::string, std::string>
{
public:
	explicit inline CreateTokenSerializer(std::shared_ptr<IClientService> client_service) :
		ModelSerializer(
			{.name="grant_type", .required=true},
			{.name="client_id", .required=true},
			{.name="client_secret", .required=true}
		),
		_client_service(std::move(client_service))
	{
		xw::require_non_null(this->_client_service.get(), "client service is nullptr", _ERROR_DETAILS_);
	}

	inline void on_validation_error(const std::string& field_name) const override
	{
		throw OAuthError(
			OAuthError::Value::invalid_request,
			"Request was missing the '" + field_name + "' parameter.",
			_ERROR_DETAILS_
		);
	}

	inline Token process(
		const std::optional<std::string>& grant_type,
		const std::optional<std::string>& client_id,
		const std::optional<std::string>& client_secret
	) override
	{
		if (grant_type.value() != "client_credentials")
		{
			throw OAuthError(
				OAuthError::Value::unsupported_grant_type,
				"Grant type '" + grant_type.value() + "' is not supported.",
				_ERROR_DETAILS_
			);
		}

		auto client = this->_client_service->get_by_id(client_id.value());
		if (client.is_null())
		{
			throw OAuthError(
				OAuthError::Value::invalid_client,
				"Client with ID '" + client_id.value() + "' is not registered.",
				_ERROR_DETAILS_
			);
		}

		if (client.client_secret != client_secret.value())
		{
			throw OAuthError(
				OAuthError::Value::invalid_client,
				"Secret key is incorrect for client with id '" + client.client_id + "'.",
				_ERROR_DETAILS_
			);
		}

		auto now = xw::dt::Datetime::utc_now();
		nlohmann::json claims = {
			{xw::crypto::jwt::iat, now.timestamp()},
			{xw::crypto::jwt::exp, (time_t)(now + this->_jwt_period).timestamp()},
			{xw::crypto::jwt::iss, this->_issuer},
			{xw::crypto::jwt::sub, this->_subject},
			{"client_id", client.client_id}
		};
		auto access_token = xw::crypto::jwt::sign(this->_signature_algorithm.get(), claims);
		return {access_token, "Bearer", (time_t)this->_jwt_period.total_seconds()};
	}

	inline CreateTokenSerializer& set_signature_algorithm(
		const std::shared_ptr<xw::crypto::ISignatureAlgorithm>& signature_algorithm
	)
	{
		this->_signature_algorithm = signature_algorithm;
		return *this;
	}

	inline CreateTokenSerializer& set_jwt_period(const xw::dt::Timedelta& jwt_period)
	{
		this->_jwt_period = jwt_period;
		return *this;
	}

	inline CreateTokenSerializer& set_subject(const std::string& subject)
	{
		this->_subject = subject;
		return *this;
	}

	inline CreateTokenSerializer& set_issuer(const std::string& issuer)
	{
		this->_issuer = issuer;
		return *this;
	}

private:
	std::shared_ptr<IClientService> _client_service;
	std::shared_ptr<xw::crypto::ISignatureAlgorithm> _signature_algorithm;
	xw::dt::Timedelta _jwt_period;
	std::string _subject;
	std::string _issuer;
};
