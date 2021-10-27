/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <list>
#include <optional>

// oauth-service
#include "../models/user.h"


class IUserService
{
public:
	[[nodiscard]]
	virtual std::list<UserModel> get_all() const = 0;

	[[nodiscard]]
	virtual UserModel create(const std::string& email, const std::string& raw_password) const = 0;

	[[nodiscard]]
	virtual UserModel update(
		long long int id,
		const std::optional<std::string>& email, const std::optional<std::string>& raw_password
	) const = 0;

	[[nodiscard]]
	virtual UserModel remove(long long int id) const = 0;
};
