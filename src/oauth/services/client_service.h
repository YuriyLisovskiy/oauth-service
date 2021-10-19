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
	virtual std::list<ClientModel> get_all_clients() const;

	[[nodiscard]]
	virtual ClientModel create_client(std::string id) const;

	[[nodiscard]]
	virtual ClientModel delete_client(const std::string& id) const;

	[[nodiscard]]
	virtual ClientModel update_secret(const std::string& client_id) const;

protected:
	const xw::conf::Settings* settings;
	std::shared_ptr<xw::orm::Repository> repository;
};
