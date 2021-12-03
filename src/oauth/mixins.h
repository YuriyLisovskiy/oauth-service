/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// oauth-service
#include "./services/interfaces.h"


class ClientServiceMixin
{
public:
	inline void set_client_service(const std::shared_ptr<IClientService>& service)
	{
		xw::require_non_null(service.get(), "'client_service' is nullptr", _ERROR_DETAILS_);
		this->client_service = service;
	}

protected:
	std::shared_ptr<IClientService> client_service = nullptr;
};
