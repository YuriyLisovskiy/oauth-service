/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.base/interfaces/base.h>
#include <xalwart.orm/db/model.h>
#include <xalwart.crypto/digest.h>


class UserModel : public xw::orm::db::Model, public xw::IJsonSerializable
{
private:
	std::string _password_hash;
	static const inline Digest HASH_FUNCTION = xw::crypto::sha256;

public:
	size_t id{};
	std::string email;
	xw::dt::Datetime created_at;
	xw::dt::Datetime updated_at;

	static constexpr const char* meta_table_name = "user";

	inline static const std::tuple meta_columns = {
		xw::orm::db::make_pk_column_meta("id", &UserModel::id),
		xw::orm::db::make_column_meta("email", &UserModel::email),
		xw::orm::db::make_column_meta("password_hash", &UserModel::_password_hash),
		xw::orm::db::make_column_meta("created_at", &UserModel::created_at),
		xw::orm::db::make_column_meta("updated_at", &UserModel::updated_at)
	};

	UserModel() = default;

	inline UserModel(
		std::string email, const xw::dt::Datetime& created_at, const xw::dt::Datetime& updated_at
	) :
		email(std::move(email)), created_at(created_at), updated_at(updated_at)
	{
	}

	inline void __orm_set_column__(const std::string& column_name, const char* data) override
	{
		this->__orm_set_column_data__(UserModel::meta_columns, column_name, data);
	}

	[[nodiscard]]
	inline std::string to_string() const override
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

	[[nodiscard]]
	nlohmann::json to_json() const override
	{
		return {
			{"id", this->id},
			{"email", this->email},
			{"created_at", this->created_at.str()},
			{"updated_at", this->updated_at.str()}
		};
	}

	inline void set_password(const std::string& raw_password)
	{
		this->_password_hash = HASH_FUNCTION(raw_password);
	}

	[[nodiscard]]
	inline bool check_password(const std::string& raw_password) const
	{
		return this->_password_hash == HASH_FUNCTION(raw_password);
	}
};
