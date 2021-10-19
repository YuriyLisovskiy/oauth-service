/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./subcommand.h"


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

	this->settings->LOGGER->error(message);
}
