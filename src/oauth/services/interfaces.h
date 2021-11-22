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
	virtual std::list<ClientModel> list() const = 0;

	[[nodiscard]]
	virtual ClientModel get_by_id(const std::string& id) const = 0;

	[[nodiscard]]
	virtual ClientModel create(std::string id) const = 0;

	[[nodiscard]]
	virtual ClientModel remove(const std::string& id) const = 0;

	[[nodiscard]]
	virtual ClientModel update(const std::string& client_id) const = 0;
};
