/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <memory>
#include <list>

// xalwart
#include <xalwart.orm/repository.h>
#include <xalwart/conf/settings.h>

// oauth-service
#include "../models/client.h"


extern std::string _generate_random_alphanum_string(size_t length);

extern std::string _generate_uuid4();

class ClientService
{
public:
	explicit ClientService(
		const xw::conf::Settings* settings,
		std::shared_ptr<xw::orm::Repository> repository
	) :
		settings(settings), repository(std::move(repository))
	{
		xw::require_non_null(this->settings, "settings is nullptr", _ERROR_DETAILS_);
		xw::require_non_null(this->repository.get(), "repository is nullptr", _ERROR_DETAILS_);
	}

	[[nodiscard]]
	virtual std::list<ClientModel> get_all_clients() const
	{
		std::list<ClientModel> clients = this->repository->select<ClientModel>().all();
		this->repository->free_connection();
		return clients;
	}

	[[nodiscard]]
	virtual inline ClientModel create_client(std::string id, std::string secret) const
	{
		if (id.empty())
		{
			id = _generate_uuid4();
		}

		if (secret.empty())
		{
			secret = _generate_random_alphanum_string(64);
		}

		auto now = xw::dt::Datetime::now(this->settings->TIMEZONE);
		ClientModel client(id, secret, now, now);
		this->repository->transaction([&](auto& transaction)
		{
			transaction.template insert<ClientModel>().model(client).commit_one();
			transaction.commit();
		});

		return client;
	}

	[[nodiscard]]
	virtual inline ClientModel delete_client(const std::string& id) const
	{
		ClientModel client;
		this->repository->transaction([&](auto& transaction)
		{
			client = transaction.template select<ClientModel>()
			    .where(xw::orm::q::c(&ClientModel::client_id) == id)
				.first();
			if (!client.is_null())
			{
				transaction.template delete_<ClientModel>()
				    .where(xw::orm::q::c(&ClientModel::client_id) == id)
					.commit();
			}

			transaction.commit();
		});
		return client;
	}

	[[nodiscard]]
	virtual inline ClientModel edit_client(const std::string& id, const std::string& secret)
	{
		ClientModel client;
		this->repository->transaction([&](auto& transaction)
		{
			client = transaction.template select<ClientModel>()
			    .where(xw::orm::q::c(&ClientModel::client_id) == id)
				.first();
			if (!client.is_null() && client.client_secret != secret)
			{
				client.client_secret = secret.empty() ? _generate_random_alphanum_string(64) : secret;
				client.updated_at = xw::dt::Datetime::now(this->settings->TIMEZONE);
				transaction.template update<ClientModel>()
				    .model(client)
					.commit_one();
			}

			transaction.commit();
		});
		return client;
	}

protected:
	const xw::conf::Settings* settings;
	std::shared_ptr<xw::orm::Repository> repository;
};
