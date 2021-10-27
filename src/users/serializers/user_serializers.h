/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <concepts>
#include <memory>
#include <functional>

// oauth-service
#include "../../config/common_serializers.h"
#include "../services/user_service.h"


class CreateUserSerializer : public CreateModelSerializer<UserModel, std::string, std::string>
{
public:
	explicit inline CreateUserSerializer(std::shared_ptr<IUserService> user_service) :
		CreateModelSerializer(
			{.name="email", .required=true},
			{.name="raw_password", .required=true}
		)
	{
		this->_user_service = std::move(user_service);
		xw::require_non_null(this->_user_service.get(), "user service is nullptr", _ERROR_DETAILS_);
	}

	inline UserModel create(
		std::optional<std::string> email, std::optional<std::string> raw_password
	) override
	{
		return this->_user_service->create(email.value(), raw_password.value());
	}

private:
	std::shared_ptr<IUserService> _user_service;
};

class UpdateUserSerializer :
	public UpdateModelSerializer<UserModel, long long int, std::string, std::string>
{
public:
	explicit inline UpdateUserSerializer(std::shared_ptr<IUserService> user_service) :
		UpdateModelSerializer(
			{.name="id", .required=true},
			{.name="email"},
			{.name="raw_password"}
		)
	{
		this->_user_service = std::move(user_service);
		xw::require_non_null(this->_user_service.get(), "user service is nullptr", _ERROR_DETAILS_);
	}

	inline UserModel update(
		std::optional<long long int> id,
		std::optional<std::string> email, std::optional<std::string> raw_password
	) override
	{
		return this->_user_service->update(id.value(), email, raw_password);
	}

private:
	std::shared_ptr<IUserService> _user_service;
};
