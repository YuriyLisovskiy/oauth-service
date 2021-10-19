/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// oauth-service
#include "./subcommand.h"


class UpdateClientCommand final : public ClientSubcommand
{
public:
	inline explicit UpdateClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	) :
		ClientSubcommand(
			config, settings, "update", "Regenerates secret key of service client",
			"", std::move(client_service)
		)
	{
	}

protected:
	bool handle() final;
};
