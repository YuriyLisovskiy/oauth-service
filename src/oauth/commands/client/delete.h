/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// oauth-service
#include "./subcommand.h"


class DeleteClientCommand final : public ClientSubcommand
{
public:
	inline explicit DeleteClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	) :
		ClientSubcommand(
			config, settings, "delete", "Deletes service client", "", std::move(client_service)
		)
	{
	}

protected:
	bool handle() override;
};
