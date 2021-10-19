/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// xalwart
#include <xalwart/commands/command.h>
#include <xalwart/commands/flags/default.h>

// oauth-service
#include "../../services/client_service.h"


class ClientCommand final : public xw::cmd::Command
{
public:
	explicit ClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	);

protected:
	inline void add_flags() override
	{
		xw::cmd::Command::add_flags();
		this->_list_flag = this->flag_set->make_bool("l", "list", false, "Prints all service clients");
	}

	void validate() const override;

	bool handle() override;

private:
	std::shared_ptr<xw::cmd::flags::BoolFlag> _list_flag = nullptr;
	std::shared_ptr<ClientService> _client_service;
};
