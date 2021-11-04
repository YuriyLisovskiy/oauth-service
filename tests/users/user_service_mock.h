/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

#include <list>
#include <string>

#include "../../src/users/services/interfaces.h"
#include "../../src/users/services/exceptions.h"


class UserServiceMock : public IUserService
{
public:
	[[nodiscard]]
	std::list<UserModel> get_all() const override
	{
		return this->_data;
	}

	UserModel get_by_id(long long id) const override
	{
		auto model = std::find_if(
			this->_data.begin(), this->_data.end(),
			[id](const UserModel& item) -> bool { return item.id == id; }
		);
		if (model == this->_data.end())
		{
			throw UserNotFoundException(id, _ERROR_DETAILS_);
		}

		return *model;
	}

	[[nodiscard]]
	UserModel create(const std::string& email, const std::string& raw_password) const override
	{
		if (this->_data.empty())
		{
			auto now = get_now();
			UserModel user(email, now, now);
			user.id = 1;
			user.set_password(raw_password);
			return user;
		}

		return this->_data.front();
	}

	[[nodiscard]]
	UserModel update(
		long long int id,
		const std::optional<std::string>& email, const std::optional<std::string>& raw_password
	) const override
	{
		for (auto& model : this->_data)
		{
			if (model.id == id)
			{
				model.email = email.value_or(model.email);
				if (raw_password.has_value())
				{
					model.set_password(raw_password.value());
				}

				model.updated_at = get_now();
				return model;
			}
		}

		throw UserNotFoundException(id, _ERROR_DETAILS_);
	}

	[[nodiscard]]
	UserModel remove(long long int id) const override
	{
		auto model = std::remove_if(
			this->_data.begin(), this->_data.end(),
			[id](const UserModel& item) -> bool { return item.id == id; }
		);
		if (model == this->_data.end())
		{
			throw UserNotFoundException(id, _ERROR_DETAILS_);
		}

		return *model;
	}

	void set_data(std::list<UserModel> data)
	{
		this->_data = std::move(data);
	}

	void set_data(const std::vector<UserModel>& data)
	{
		this->_data = std::list<UserModel>(data.begin(), data.end());
	}

	static xw::dt::Datetime get_now()
	{
		return xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	}

private:
	mutable std::list<UserModel> _data;
};
