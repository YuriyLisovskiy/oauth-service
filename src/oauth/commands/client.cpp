/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./client.h"

// STL
#include <iostream>

// xalwart
#include <xalwart.base/exceptions.h>


void ClientSubcommand::add_flags()
{
	xw::cmd::Command::add_flags();
	std::string client_id_usage = "Client id";
	if (!this->_extra_client_id_usage.empty())
	{
		client_id_usage += " (" + this->_extra_client_id_usage + ")";
	}

	this->client_id_flag = this->flag_set->make_string("i", "id", "", client_id_usage);
}

void ClientSubcommand::validate() const
{
	xw::cmd::Command::validate();
	if (!this->client_id_flag->valid())
	{
		throw xw::CommandError("Got invalid value of string flag: -i, --id");
	}
}

void ClientSubcommand::log_client_not_found(const std::string& client_id)
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

	this->logger->error(message);
}

bool NewClientCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	auto client_id = this->client_id_flag->get();
	auto created_client = this->client_service->create(client_id);
	this->logger->info("Created:\n" + created_client.to_string());
	return true;
}

bool ListClientsCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	auto clients = this->client_service->list();
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

	this->logger->info(message);
	return true;
}

bool UpdateClientCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	auto client_id = this->client_id_flag->get();
	auto client = this->client_service->update(client_id);
	if (!client.is_null())
	{
		this->logger->info("Updated:\n" + client.to_string());
	}
	else
	{
		this->log_client_not_found(client_id);
	}

	return true;
}

bool DeleteClientCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	auto client_id = this->client_id_flag->get();
	auto client = this->client_service->remove(client_id);
	if (!client.is_null())
	{
		this->logger->info("Deleted:\n" + client.to_string());
	}
	else
	{
		this->log_client_not_found(client_id);
	}

	return true;
}

bool ClientCommand::handle()
{
	if (!xw::cmd::Command::handle())
	{
		std::cout << this->usage() << '\n';
	}

	return true;
}
