/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./user_service.h"


std::list<UserModel> UserService::get_all() const
{
	std::list<UserModel> users;
	this->_repository->wrap([&](auto*)
	{
		users = this->_repository->select<UserModel>().all();
	});
	return users;
}

UserModel UserService::get_by_id(long long id) const
{
	UserModel user;
	this->_repository->wrap([&](auto*)
	{
		user = this->_repository->select<UserModel>()
		    .where(xw::orm::q::c(&UserModel::id) == id)
			.first();
	});
	return user;
}

UserModel UserService::create(
	const std::string& email, const std::string& raw_password
) const
{
	auto now = xw::dt::Datetime::now(this->_timezone);
	UserModel user(email, now, now);
	user.set_password(raw_password);
	this->_repository->wrap([&](auto*)
	{
		this->_repository->insert<UserModel>().model(user).commit_one(user.id);
	});
	return user;
}

UserModel UserService::update(
	long long int id,
	const std::optional<std::string>& email, const std::optional<std::string>& raw_password
) const
{
	UserModel user;
	this->_repository->wrap([&](auto*)
	{
		user = this->_repository->select<UserModel>()
		    .where(xw::orm::q::c(&UserModel::id) == id)
			.first();
		if (!user.is_null())
		{
			if (email.has_value())
			{
				user.email = email.value();
			}

			if (raw_password.has_value())
			{
				user.set_password(raw_password.value());
			}

			user.updated_at = xw::dt::Datetime::now(this->_timezone);
			this->_repository->update<UserModel>()
			    .model(user)
				.commit_one();
		}
	});
	return user;
}

UserModel UserService::remove(long long int id) const
{
	UserModel user;
	this->_repository->wrap([&](auto*)
	{
		user = this->_repository->select<UserModel>()
		    .where(xw::orm::q::c(&UserModel::id) == id)
			.first();
		if (!user.is_null())
		{
			this->_repository->delete_<UserModel>()
			    .where(xw::orm::q::c(&UserModel::id) == id)
				.commit();
		}
	});
	return user;
}
