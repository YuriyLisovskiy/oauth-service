/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// oauth-service
#include "./subcommand.h"


class NewClientCommand final : public ClientSubcommand
{
public:
	inline explicit NewClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	) :
		ClientSubcommand(
			config, settings, "new", "Creates service client",
			"if empty, uuid4 will be generated", std::move(client_service)
		)
	{
	}

protected:
	bool handle() override;
};
