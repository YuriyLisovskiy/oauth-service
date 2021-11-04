/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>
#include <string>

// xalwart
#include <xalwart/conf/module.h>

// oauth-service
#include "./services/interfaces.h"


class OAuthModuleConfig : public xw::conf::ModuleConfig
{
public:
	explicit OAuthModuleConfig(const std::string& registration_name, xw::conf::Settings* settings);

	void configure() override;

	void urlpatterns() override;

	void commands() override;

private:
	std::shared_ptr<IClientService> _client_service;
};
