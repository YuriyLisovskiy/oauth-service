/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./new.h"


bool NewClientCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	auto client_id = this->client_id_flag->get();
	auto created_client = this->client_service->create_client(client_id);
	this->settings->LOGGER->info("Created:\n" + created_client.to_string());
	return true;
}
