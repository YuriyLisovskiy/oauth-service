/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"

// STL
#include <memory>

// xalwart
#include <xalwart.orm/repository.h>

// oauth-service
#include "./controllers/hello.h"
#include "./commands/client.h"
#include "./services/client_service.h"


void ModuleConfig::urlpatterns()
{
	this->url<HelloController>(R"(oauth2/hello/?)", "hello");
}

void ModuleConfig::commands()
{
	this->command<ClientCommand>(
		this->settings->LOGGER,
		std::make_shared<ClientService>(
			std::make_shared<xw::orm::Repository>(this->settings->DB.get()), this->settings->TIMEZONE
		)
	);
}
