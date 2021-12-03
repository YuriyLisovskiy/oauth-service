/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// oauth-service
#include "../mixins.h"
#include "../../config/common_controllers.h"
#include "../../oauth/mixins.h"


class UserController : public JWTAuthRequiredController<long long int>,
					   public UserServiceMixin,
					   public ClientServiceMixin
{
public:
	explicit inline UserController(const xw::ILogger* logger, const OAuthConfig& oauth_config) :
		JWTAuthRequiredController<long long int>({"get", "put", "delete"}, logger, oauth_config)
	{
	}

	std::unique_ptr<xw::http::IResponse> get(xw::http::IRequest* request, long long id) const override;

	std::unique_ptr<xw::http::IResponse> put(xw::http::IRequest* request, long long id) const override;

	std::unique_ptr<xw::http::IResponse> delete_(xw::http::IRequest* request, long long id) const override;

protected:
	[[nodiscard]]
	inline bool client_exists(const std::string& id) const override
	{
		return !this->client_service->get_by_id(id).is_null();
	}
};
