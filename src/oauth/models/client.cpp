/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./client.h"


std::string ClientModel::to_string() const
{
	if (this->is_null())
	{
		return "null";
	}

	return "Client:\n  id: " + this->client_id +
		"\n  secret: " + this->client_secret +
		"\n  created at: " + this->created_at.str() +
		"\n  updated at: " + this->updated_at.str();
}

nlohmann::json ClientModel::to_json() const
{
	if (this->is_null())
	{
		return nullptr;
	}

	return {
		{"client_id", this->client_id},
		{"client_secret", this->client_secret},
		{"created_at", this->created_at.str()},
		{"updated_at", this->updated_at.str()}
	};
}
