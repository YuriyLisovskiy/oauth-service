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
#include "./commands/client/client.h"


void ModuleConfig::urlpatterns()
{
	this->url<HelloController>(R"(oauth2/hello/?)", "hello");
}

void ModuleConfig::commands()
{
	this->command<ClientCommand>(
		this, this->settings, std::make_unique<ClientService>(
			this->settings, std::make_shared<xw::orm::Repository>(this->settings->DB.get())
		)
	);
}
