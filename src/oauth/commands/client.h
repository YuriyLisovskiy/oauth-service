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
		xw::require_non_null(client_service.get(), "client_service is nullptr", _ERROR_DETAILS_);
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

class NewClientCommand final : public ClientSubcommand
{
public:
	inline explicit NewClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	) :
		ClientSubcommand(
			config, settings, "new", "Creates service client",
			"if empty, uuid4 will be generated", std::move(client_service)
		)
	{
	}

protected:
	bool handle() override;
};

class ListClientsCommand final : public ClientSubcommand
{
public:
	inline explicit ListClientsCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	) :
		ClientSubcommand(
			config, settings, "list", "Prints all service clients", "", std::move(client_service)
		)
	{
	}

protected:
	bool handle() override;
};

class UpdateClientCommand final : public ClientSubcommand
{
public:
	inline explicit UpdateClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	) :
		ClientSubcommand(
			config, settings, "update", "Regenerates secret key of service client",
			"", std::move(client_service)
		)
	{
	}

protected:
	bool handle() final;
};

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

class ClientCommand final : public xw::cmd::Command
{
public:
	explicit inline ClientCommand(
		xw::conf::IModuleConfig* config, xw::conf::Settings* settings,
		std::shared_ptr<ClientService> client_service
	) :
		xw::cmd::Command(config, settings, "client", "Lists, creates, deletes and updates service clients")
	{
		this->add_subcommand(std::make_shared<NewClientCommand>(config, settings, client_service));
		this->add_subcommand(std::make_shared<ListClientsCommand>(config, settings, client_service));
		this->add_subcommand(std::make_shared<UpdateClientCommand>(config, settings, client_service));
		this->add_subcommand(std::make_shared<DeleteClientCommand>(config, settings, client_service));
	}

protected:
	bool handle() override;
};
