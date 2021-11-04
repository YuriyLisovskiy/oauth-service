/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

#include <list>
#include <vector>
#include <string>

#include "../../src/users/services/interfaces.h"


class UserServiceMock : public IUserService
{
public:
	[[nodiscard]]
	inline std::list<UserModel> get_all() const override
	{
		return this->_data;
	}

	UserModel get_by_id(long long id) const override;

	[[nodiscard]]
	UserModel create(const std::string& email, const std::string& raw_password) const override;

	[[nodiscard]]
	UserModel update(
		long long int id,
		const std::optional<std::string>& email, const std::optional<std::string>& raw_password
	) const override;

	[[nodiscard]]
	UserModel remove(long long int id) const override;

	inline void set_data(std::list<UserModel> data)
	{
		this->_data = std::move(data);
	}

	inline void set_data(const std::vector<UserModel>& data)
	{
		this->_data = std::list<UserModel>(data.begin(), data.end());
	}

	inline static xw::dt::Datetime get_now()
	{
		return xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	}

private:
	mutable std::list<UserModel> _data;
};
