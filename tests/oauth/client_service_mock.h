/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/oauth/services/interfaces.h"
#include "../../src/oauth/services/client_service.h"


class ClientServiceMock : public IClientService
{
public:
	[[nodiscard]]
	inline std::list<ClientModel> list() const override
	{
		return this->_clients;
	}

	ClientModel get_by_id(const std::string& id) const override;

	[[nodiscard]]
	ClientModel create(std::string id) const override;

	[[nodiscard]]
	ClientModel remove(const std::string& id) const override;

	[[nodiscard]]
	ClientModel update(const std::string& client_id) const override;

	inline void set_data(std::list<ClientModel> data)
	{
		this->_clients = std::move(data);
	}

	inline std::list<ClientModel> get_data() const
	{
		return this->_clients;
	}

	ClientModel get_client(const std::string& id) const;

	inline bool has_client(const std::string& id) const
	{
		return std::find_if(
			this->_clients.begin(), this->_clients.end(), [id](const ClientModel& model) -> bool
			{
				return model.client_id == id;
			}
		) != this->_clients.end();
	}

private:
	mutable std::list<ClientModel> _clients;

	static inline ClientModel get_null_model()
	{
		ClientModel model;
		model.mark_as_null();
		return model;
	}
};
