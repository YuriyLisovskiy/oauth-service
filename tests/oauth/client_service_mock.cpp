/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./client_service_mock.h"


ClientModel ClientServiceMock::create(std::string id) const
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	ClientModel model(id, _generate_random_alphanum_string(64), now, now);
	this->_clients.push_back(model);
	return model;
}

ClientModel ClientServiceMock::remove(const std::string& id) const
{
	auto model = std::remove_if(
		this->_clients.begin(), this->_clients.end(), [id](const ClientModel& item) -> bool
		{
			return item.client_id == id;
		}
	);
	if (model == this->_clients.end())
	{
		return get_null_model();
	}

	return *model;
}

ClientModel ClientServiceMock::update(const std::string& client_id) const
{
	for (auto& model : this->_clients)
	{
		if (model.client_id == client_id)
		{
			model.client_secret = _generate_random_alphanum_string(64);
			model.updated_at = xw::dt::Datetime::now(
				std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC)
			);
			return model;
		}
	}

	return get_null_model();
}

ClientModel ClientServiceMock::get_client(const std::string& id) const
{
	auto client = std::find_if(
		this->_clients.begin(), this->_clients.end(), [id](const ClientModel& model) -> bool
		{
			return model.client_id == id;
		}
	);
	if (client == this->_clients.end())
	{
		return get_null_model();
	}

	return *client;
}
