/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// oauth-service
#include "../mixins.h"
#include "../../config/common_controllers.h"
#include "../../oauth/mixins.h"


class UsersController : public JWTAuthRequiredController<>,
                        public UserServiceMixin,
                        public ClientServiceMixin
{
public:
	explicit inline UsersController(const xw::ILogger* logger, const OAuthConfig& oauth_config) :
		JWTAuthRequiredController<>({"get", "post"}, logger, oauth_config)
	{
	}

	std::unique_ptr<xw::http::IResponse> get(xw::http::IRequest* request) const override;

	std::unique_ptr<xw::http::IResponse> post(xw::http::IRequest* request) const override;

protected:
	[[nodiscard]]
	bool client_exists(const std::string& id) const override
	{
		return !this->client_service->get_by_id(id).is_null();
	}
};
