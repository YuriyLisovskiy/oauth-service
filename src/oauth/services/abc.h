/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <list>

// oauth-service
#include "../models/client.h"


class IClientService
{
public:
	[[nodiscard]]
	virtual std::list<ClientModel> get_all_clients() const = 0;

	[[nodiscard]]
	virtual ClientModel create_client(std::string id) const = 0;

	[[nodiscard]]
	virtual ClientModel delete_client(const std::string& id) const = 0;

	[[nodiscard]]
	virtual ClientModel update_secret(const std::string& client_id) const = 0;
};
