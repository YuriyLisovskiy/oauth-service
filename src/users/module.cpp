/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"

// oauth-service
#include "./services/user_service.h"
#include "./controllers/user_controller.h"
#include "./controllers/users_controller.h"
#include "../config/settings/settings.h"
#include "../oauth/services/client_service.h"


void UsersModuleConfig::configure()
{
	this->_repository = std::make_shared<xw::orm::Repository>(this->settings->DB.get());
	this->_user_service = std::make_shared<UserService>(this->_repository, this->settings->TIMEZONE);
	this->_client_service = std::make_shared<ClientService>(this->_repository, this->settings->TIMEZONE);
	this->set_ready();
}

void UsersModuleConfig::urlpatterns()
{
	this->url_func<UsersController>(R"(?)", "users", [this](const auto* settings) -> auto
	{
		auto* project_settings = (Settings*)settings;
		auto controller = UsersController(project_settings->LOGGER.get(), project_settings->OAUTH);
		controller.set_user_service(this->_user_service);
		controller.set_client_service(this->_client_service);
		return controller;
	});
	this->url_func<UserController, long long int>(
		R"(<id>([0-9]+)/?)", "user", [this](const auto* settings) -> auto
		{
			auto* project_settings = (Settings*)settings;
			auto controller = UserController(project_settings->LOGGER.get(), project_settings->OAUTH);
			controller.set_user_service(this->_user_service);
			controller.set_client_service(this->_client_service);
			return controller;
		}
	);
}
