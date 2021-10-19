/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./client_command.h"

// STL
#include <iostream>

// xalwart
#include <xalwart.base/string_utils.h>


void ClientCommand::add_flags()
{
	this->_new_client_flag = this->flag_set->make_bool(
		"n", "new", false, "Create client"
	);
	this->_delete_client_flag = this->flag_set->make_bool(
		"d", "delete", false, "Delete client"
	);
	this->_update_client_flag = this->flag_set->make_bool(
		"u", "update", false, "Update client (regenerate secret key)"
	);
	this->_client_id_flag = this->flag_set->make_string(
		"i", "id", "", "Client id (if empty during creation, uuid4 will be generated)"
	);
	this->_use_colors_flag = this->flag_set->make_bool(
		"c", "colors", true, "Enables colors for logging in terminal"
	);
	this->_print_help_flag = this->flag_set->make_bool(
		"h", "help", false, "Print usage"
	);
}

void ClientCommand::handle()
{
	if (this->_print_help_flag->get())
	{
		std::cout << this->usage() << '\n';
		return;
	}

	auto client_id = this->_client_id_flag->get();
	this->use_colors_for_logging(this->_use_colors_flag->get());
	if (this->_new_client_flag->get())
	{
		auto created_client = this->_client_service->create_client(client_id);
		this->settings->LOGGER->info("Created:\n" + client_as_string(created_client));
	}
	else if (this->_delete_client_flag->get())
	{
		auto client = this->_client_service->delete_client(client_id);
		if (!client.is_null())
		{
			this->settings->LOGGER->info("Deleted:\n" + client_as_string(client));
		}
		else
		{
			this->client_not_found(client_id);
		}
	}
	else if (this->_update_client_flag->get())
	{
		auto client = this->_client_service->update_secret(client_id);
		if (!client.is_null())
		{
			this->settings->LOGGER->info("Updated:\n" + client_as_string(client));
		}
		else
		{
			this->client_not_found(client_id);
		}
	}
	else
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
				return client_as_string(client);
			});
		}

		this->settings->LOGGER->info(message);
	}
}

void ClientCommand::client_not_found(const std::string& client_id)
{
	std::string message = "Client with " + (
		client_id.empty() ? "empty id" : "id '" + client_id + "'"
	) + " is not found.";
	if (client_id.empty())
	{
		message = "Client with empty id is not found.";
	}
	else
	{
		message = "Client with id '" + client_id + "' is not found.";
	}

	this->settings->LOGGER->error(message);
}
