/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// oauth-service
#include "./services/interfaces.h"


class UserServiceMixin
{
public:
	inline void set_user_service(const std::shared_ptr<IUserService>& service)
	{
		xw::require_non_null(service.get(), "'user_service' is nullptr", _ERROR_DETAILS_);
		this->user_service = service;
	}

protected:
	std::shared_ptr<IUserService> user_service = nullptr;
};
