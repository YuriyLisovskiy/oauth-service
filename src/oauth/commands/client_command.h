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
#include "../services/client_service.h"


class ClientCommand final : public xw::cmd::Command
{
public:
	inline explicit ClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::unique_ptr<ClientService> service
	) :
		xw::cmd::Command(config, settings, "client", "Lists, creates and deletes service clients.")
	{
		this->_service = std::move(service);
	}

protected:
	void add_flags() final;

	void handle() final;

	[[nodiscard]]
	static std::string client_as_string(const ClientModel& client)
	{
		return client.__str__();
	}

	void client_not_found(const std::string& client_id);

private:
	std::shared_ptr<xw::cmd::flags::BoolFlag> _new_client_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _delete_client_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _edit_client_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::StringFlag> _client_id_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::StringFlag> _client_secret_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _use_colors_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _print_help_flag = nullptr;

	std::unique_ptr<ClientService> _service;
};
