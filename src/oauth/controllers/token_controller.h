/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.crypto/interfaces.h>
#include <xalwart/controllers/controller.h>

// oauth-service
#include "../services/interfaces.h"


class TokenController : public xw::ctrl::Controller<>
{
public:
	explicit inline TokenController(const xw::ILogger* logger) :
		xw::ctrl::Controller<>({"post"}, logger)
	{
	}

	std::unique_ptr<xw::http::IResponse> post(xw::http::IRequest* request) const override;

	std::unique_ptr<xw::http::IResponse> dispatch(xw::http::IRequest* request) const override;

	inline TokenController& set_client_service(std::shared_ptr<IClientService> client_service)
	{
		this->_client_service = std::move(client_service);
		return *this;
	}

	inline TokenController& set_signature_algorithm(
		std::shared_ptr<xw::crypto::ISignatureAlgorithm> signature_algorithm
	)
	{
		this->_signature_algorithm = std::move(signature_algorithm);
		return *this;
	}

	inline TokenController& set_jwt_period(const xw::dt::Timedelta& jwt_period)
	{
		this->_jwt_period = jwt_period;
		return *this;
	}

	inline TokenController& set_subject(std::string subject)
	{
		this->_subject = std::move(subject);
		return *this;
	}

	inline TokenController& set_issuer(std::string issuer)
	{
		this->_issuer = std::move(issuer);
		return *this;
	}

private:
	std::shared_ptr<IClientService> _client_service;
	std::shared_ptr<xw::crypto::ISignatureAlgorithm> _signature_algorithm;
	xw::dt::Timedelta _jwt_period;
	std::string _subject;
	std::string _issuer;

	void _set_required_headers(xw::http::IResponse* response) const;
};
