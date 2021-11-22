/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"

// STL
#include <memory>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.crypto/hmac.h>
#include <xalwart.orm/repository.h>

// oauth-service
#include "./commands/client.h"
#include "./services/client_service.h"
#include "./controllers/token_controller.h"
#include "../config/settings/settings.h"


OAuthModuleConfig::OAuthModuleConfig(
	const std::string& registration_name, xw::conf::Settings* settings
) : xw::conf::ModuleConfig(registration_name, settings)
{
}

void OAuthModuleConfig::configure()
{
	this->_client_service = std::make_shared<ClientService>(
		std::make_shared<xw::orm::Repository>(this->settings->DB.get()), this->settings->TIMEZONE
	);
	this->set_ready();
}

void OAuthModuleConfig::urlpatterns()
{
	this->url_func<TokenController>(R"(token/?)", "token", [this](const auto* settings) -> auto
	{
		auto* project_settings = (Settings*)settings;
		return TokenController(project_settings->LOGGER.get())
			.set_client_service(this->_client_service)
			.set_signature_algorithm(project_settings->OAUTH.SIGNER)
			.set_jwt_period(project_settings->OAUTH.JWT.PERIOD)
			.set_subject(project_settings->OAUTH.JWT.SUBJECT)
			.set_issuer(project_settings->OAUTH.JWT.ISSUER);
	});
}

void OAuthModuleConfig::commands()
{
	this->command<ClientCommand>(this->settings->LOGGER, this->_client_service);
}
