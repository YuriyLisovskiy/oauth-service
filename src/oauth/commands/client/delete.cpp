/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./delete.h"


bool DeleteClientCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	auto client_id = this->client_id_flag->get();
	auto client = this->client_service->delete_client(client_id);
	if (!client.is_null())
	{
		this->settings->LOGGER->info("Deleted:\n" + client.to_string());
	}
	else
	{
		this->log_client_not_found(client_id);
	}

	return true;
}
