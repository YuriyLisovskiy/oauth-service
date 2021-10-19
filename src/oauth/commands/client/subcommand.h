/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>
#include <string>

// xalwart
#include <xalwart/commands/command.h>
#include <xalwart/commands/flags/default.h>

// oauth-service
#include "../../services/client_service.h"


class ClientSubcommand : public xw::cmd::Command
{
public:
	inline explicit ClientSubcommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		const std::string& cmd_name, const std::string& help,
		std::string extra_client_id_usage, std::shared_ptr<ClientService> client_service
	) :
		xw::cmd::Command(config, settings, cmd_name, help)
	{
		this->client_service = std::move(client_service);
		this->_extra_client_id_usage = std::move(extra_client_id_usage);
	}

protected:
	std::shared_ptr<xw::cmd::flags::StringFlag> client_id_flag = nullptr;
	std::shared_ptr<ClientService> client_service;

	void add_flags() override;

	void validate() const override;

	void log_client_not_found(const std::string& client_id);

private:
	std::string _extra_client_id_usage;
};
