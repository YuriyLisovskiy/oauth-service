/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./user_service_mock.h"

#include "../../src/users/services/exceptions.h"


UserModel UserServiceMock::get_by_id(long long id) const
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

UserModel UserServiceMock::create(const std::string& email, const std::string& raw_password) const
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

UserModel UserServiceMock::update(
	long long int id,
	const std::optional<std::string>& email, const std::optional<std::string>& raw_password
) const
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

UserModel UserServiceMock::remove(long long int id) const
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
