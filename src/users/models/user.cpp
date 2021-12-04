/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./user.h"


std::string UserModel::to_string() const
{
	if (this->is_null())
	{
		return "null";
	}

	return "User:\n  id: " + std::to_string(this->id) +
		"\n  email: " + this->email +
		"\n  created at: " + this->created_at.str() +
		"\n  updated at: " + this->updated_at.str();
}

nlohmann::json UserModel::to_json() const
{
	if (this->is_null())
	{
		return nullptr;
	}

	return {
		{"id", this->id},
		{"email", this->email},
		{"created_at", this->created_at.str()},
		{"updated_at", this->updated_at.str()}
	};
}
