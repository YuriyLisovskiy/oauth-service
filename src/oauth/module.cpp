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
	// TODO: move jwt parameters to settings.
	this->url_func<TokenController>(R"(token/?)", "token", [this](const auto* settings) -> auto
	{
		return TokenController(settings->LOGGER.get())
			.set_client_service(this->_client_service)
			.set_signature_algorithm(std::make_shared<xw::crypto::HS256>("secret_key"))
			.set_jwt_period(xw::dt::Timedelta(7)) // 7 days
			.set_subject("oauth")
			.set_issuer("oauth-service");
	});
}

void OAuthModuleConfig::commands()
{
	this->command<ClientCommand>(this->settings->LOGGER, this->_client_service);
}
