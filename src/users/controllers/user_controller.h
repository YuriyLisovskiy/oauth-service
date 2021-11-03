/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart/controllers/controller.h>

// oauth-service
#include "../services/interfaces.h"


class UserController : public xw::ctrl::Controller<long long int>
{
public:
	explicit inline UserController(
		const xw::ILogger* logger, std::shared_ptr<IUserService> user_service
	) :
		xw::ctrl::Controller<long long int>({"get", "put", "delete"}, logger)
	{
		this->_user_service = std::move(user_service);
		xw::require_non_null(this->_user_service.get(), "'user_service' is nullptr", _ERROR_DETAILS_);
	}

	std::unique_ptr<xw::http::IResponse> get(
		xw::http::IRequest* request, long long int id
	) const override;

	std::unique_ptr<xw::http::IResponse> put(
		xw::http::IRequest* request, long long int id
	) const override;

	std::unique_ptr<xw::http::IResponse> delete_(
		xw::http::IRequest* request, long long int id
	) const override;

private:
	std::shared_ptr<IUserService> _user_service = nullptr;
};
