/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./client.h"

// STL
#include <iostream>

// xalwart
#include <xalwart.base/exceptions.h>
#include <xalwart.base/string_utils.h>

// oauth-service
#include "./new.h"
#include "./update.h"
#include "./delete.h"


ClientCommand::ClientCommand(
	xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
	std::shared_ptr<ClientService> client_service
) :
	xw::cmd::Command(config, settings, "client", "Lists, creates, deletes and updates service clients")
{
	xw::require_non_null(client_service.get(), "client_service is nullptr", _ERROR_DETAILS_);
	this->_client_service = std::move(client_service);
	this->add_subcommand(std::make_shared<NewClientCommand>(config, settings, this->_client_service));
	this->add_subcommand(std::make_shared<UpdateClientCommand>(config, settings, this->_client_service));
	this->add_subcommand(std::make_shared<DeleteClientCommand>(config, settings, this->_client_service));
}

void ClientCommand::validate() const
{
	xw::cmd::Command::validate();
	if (!this->_list_flag->valid())
	{
		throw xw::CommandError("Got invalid value of boolean flag: -l, --list");
	}
}

bool ClientCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	if (this->_list_flag->get())
	{
		auto clients = this->_client_service->get_all_clients();
		std::string message;
		if (clients.empty())
		{
			message = "No clients.";
		}
		else
		{
			message = "List:\n\n" +
			xw::str::join("\n\n", clients.begin(), clients.end(), [](const auto& client) -> std::string
			{
				return client.to_string();
			});
		}

		this->settings->LOGGER->info(message);
	}
	else
	{
		std::cout << this->usage() << '\n';
	}

	return true;
}
