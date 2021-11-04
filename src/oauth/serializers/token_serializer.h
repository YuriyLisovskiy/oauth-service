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


// TODO: tests required
class TokenSerializer : public ModelSerializer<Token, std::string, std::string, std::string>
{
public:
	explicit inline TokenSerializer(std::shared_ptr<IClientService> client_service) :
		ModelSerializer(
			{.name="grant_type", .required=true},
			{.name="client_id", .required=true},
			{.name="client_secret", .required=true}
		),
		_client_service(std::move(client_service))
	{
		xw::require_non_null(this->_client_service.get(), "client service is nullptr", _ERROR_DETAILS_);
	}

	inline Token process(
		std::optional<std::string> grant_type,
		std::optional<std::string> client_id,
		std::optional<std::string> client_secret
	) override
	{
		auto client = this->_client_service->get_by_id(client_id.value());
		if (client.client_secret != client_secret.value())
		{
			throw IncorrectClientSecretError(client.client_id, _ERROR_DETAILS_);
		}

		auto now = xw::dt::Datetime::utc_now();
		auto expires_in = (time_t)(now + this->_jwt_period).timestamp();
		nlohmann::json claims = {
			{xw::crypto::jwt::iat, now.timestamp()},
			{xw::crypto::jwt::exp, expires_in},
			{xw::crypto::jwt::iss, this->_issuer},
			{xw::crypto::jwt::sub, this->_subject},
			{"client_id", client.client_id}
//			{"grant_type", grant_type.value()},
		};
		auto access_token = xw::crypto::jwt::sign(this->_signature_algorithm.get(), claims);
		return {access_token, "Bearer", expires_in};
	}

	inline TokenSerializer& set_signature_algorithm(
		const std::shared_ptr<xw::crypto::ISignatureAlgorithm>& signature_algorithm
	)
	{
		this->_signature_algorithm = signature_algorithm;
		return *this;
	}

	inline TokenSerializer& set_jwt_period(const xw::dt::Timedelta& jwt_period)
	{
		this->_jwt_period = jwt_period;
		return *this;
	}

	inline TokenSerializer& set_subject(const std::string& subject)
	{
		this->_subject = subject;
		return *this;
	}

	inline TokenSerializer& set_issuer(const std::string& issuer)
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
