/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <memory>
#include <list>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.orm/repository.h>

// oauth-service
#include "./interfaces.h"


class UserService final : public IUserService
{
public:
	explicit UserService(
		std::shared_ptr<xw::orm::Repository> repository, std::shared_ptr<xw::dt::Timezone> tz
	) :
		_timezone(std::move(tz)), _repository(std::move(repository))
	{
		xw::require_non_null(this->_repository.get(), "'repository' is nullptr", _ERROR_DETAILS_);
	}

	[[nodiscard]]
	std::list<UserModel> get_all() const override;

	[[nodiscard]]
	UserModel create(const std::string& email, const std::string& raw_password) const override;

	[[nodiscard]]
	UserModel update(
		long long int id,
		const std::optional<std::string>& email, const std::optional<std::string>& raw_password
	) const override;

	[[nodiscard]]
	UserModel remove(long long int id) const override;

private:
	std::shared_ptr<xw::orm::Repository> _repository;
	std::shared_ptr<xw::dt::Timezone> _timezone;
};
