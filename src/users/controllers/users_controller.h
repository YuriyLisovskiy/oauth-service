/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart/controllers/controller.h>

// oauth-service
#include "../services/interfaces.h"


class UsersController : public xw::ctrl::Controller<>
{
public:
	explicit inline UsersController(
		const xw::ILogger* logger, std::shared_ptr<IUserService> user_service
	) :
		xw::ctrl::Controller<>({"get", "post"}, logger)
	{
		this->_user_service = std::move(user_service);
		xw::require_non_null(this->_user_service.get(), "'user_service' is nullptr", _ERROR_DETAILS_);
	}

	std::unique_ptr<xw::http::IResponse> get(xw::http::IRequest* request) const override;

	std::unique_ptr<xw::http::IResponse> post(xw::http::IRequest* request) const override;

private:
	std::shared_ptr<IUserService> _user_service = nullptr;
};
