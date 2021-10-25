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
#include "./abc.h"


extern std::string _generate_random_alphanum_string(size_t length);

extern std::string _generate_uuid4();

class ClientService final : public IClientService
{
public:
	explicit ClientService(
		std::shared_ptr<xw::orm::Repository> repository, std::shared_ptr<xw::dt::Timezone> tz
	) :
		_timezone(std::move(tz)), _repository(std::move(repository))
	{
		xw::require_non_null(this->_repository.get(), "repository is nullptr", _ERROR_DETAILS_);
	}

	[[nodiscard]]
	std::list<ClientModel> get_all_clients() const override;

	[[nodiscard]]
	ClientModel create_client(std::string id) const override;

	[[nodiscard]]
	ClientModel delete_client(const std::string& id) const override;

	[[nodiscard]]
	ClientModel update_secret(const std::string& client_id) const override;

private:
	std::shared_ptr<xw::orm::Repository> _repository;
	std::shared_ptr<xw::dt::Timezone> _timezone;
};
