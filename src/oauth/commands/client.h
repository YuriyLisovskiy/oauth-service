/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>
#include <string>

// xalwart
#include <xalwart.base/abc/base.h>
#include <xalwart/commands/command.h>
#include <xalwart/commands/flags/default.h>

// oauth-service
#include "../services/abc.h"


class ClientSubcommand : public xw::cmd::Command
{
public:
	inline explicit ClientSubcommand(
		const std::string& cmd_name, const std::string& help,
		const std::shared_ptr<xw::abc::ILogger>& logger,
		std::string extra_client_id_usage, std::shared_ptr<IClientService> client_service
	) : xw::cmd::Command(cmd_name, help, logger)
	{
		xw::require_non_null(client_service.get(), "client_service is nullptr", _ERROR_DETAILS_);
		this->client_service = std::move(client_service);
		this->_extra_client_id_usage = std::move(extra_client_id_usage);
	}

protected:
	std::shared_ptr<xw::cmd::flags::StringFlag> client_id_flag = nullptr;
	std::shared_ptr<IClientService> client_service;

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
		const std::shared_ptr<xw::abc::ILogger>& logger,
		std::shared_ptr<IClientService> client_service
	) :
		ClientSubcommand(
			"new", "Creates service client", logger,
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
		const std::shared_ptr<xw::abc::ILogger>& logger,
		std::shared_ptr<IClientService> client_service
	) :
		ClientSubcommand(
			"list", "Prints all service clients", logger, "", std::move(client_service)
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
		const std::shared_ptr<xw::abc::ILogger>& logger,
		std::shared_ptr<IClientService> client_service
	) :
		ClientSubcommand(
			"update", "Regenerates secret key of service client", logger,
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
		const std::shared_ptr<xw::abc::ILogger>& logger,
		std::shared_ptr<IClientService> client_service
	) :
		ClientSubcommand(
			"delete", "Deletes service client", logger, "", std::move(client_service)
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
		const std::shared_ptr<xw::abc::ILogger>& logger,
		std::shared_ptr<IClientService> client_service
	) : xw::cmd::Command("client", "Lists, creates, deletes and updates service clients", logger)
	{
		this->add_subcommand(std::make_shared<NewClientCommand>(logger, client_service));
		this->add_subcommand(std::make_shared<ListClientsCommand>(logger, client_service));
		this->add_subcommand(std::make_shared<UpdateClientCommand>(logger, client_service));
		this->add_subcommand(std::make_shared<DeleteClientCommand>(logger, client_service));
	}

protected:
	bool handle() override;
};
