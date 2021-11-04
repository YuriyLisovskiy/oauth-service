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


void UsersModuleConfig::configure()
{
	this->_user_service = std::make_shared<UserService>(
		std::make_shared<xw::orm::Repository>(this->settings->DB.get()), this->settings->TIMEZONE
	);
	this->set_ready();
}

void UsersModuleConfig::urlpatterns()
{
	this->url<UsersController>(R"(?)", "users", this->_user_service);
	this->url<UserController, long long int>(R"(<id>([0-9]+)/?)", "user", this->_user_service);
}