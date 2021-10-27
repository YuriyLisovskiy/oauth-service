/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"

// xalwart
#include <xalwart.orm/repository.h>

// oauth-service
#include "./services/user_service.h"
#include "./controllers/user_controller.h"
#include "./controllers/users_controller.h"


UsersModuleConfig::UsersModuleConfig(xw::conf::Settings* settings) : xw::conf::ModuleConfig(settings)
{
	this->_user_service = std::make_shared<UserService>(
		std::make_shared<xw::orm::Repository>(this->settings->DB.get()), this->settings->TIMEZONE
	);
}

void UsersModuleConfig::urlpatterns()
{
	this->url<UsersController>(R"(users/?)", "users", this->_user_service);
	this->url<UserController, long long int>(R"(users/<id>([0-9]+)/?)", "user", this->_user_service);
}
