/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart/conf/module.h>

// oauth-service
#include "./services/interfaces.h"


class UsersModuleConfig : public xw::conf::ModuleConfig
{
public:
	explicit UsersModuleConfig(xw::conf::Settings* settings);

	void urlpatterns() override;

private:
	std::shared_ptr<IUserService> _user_service = nullptr;
};
