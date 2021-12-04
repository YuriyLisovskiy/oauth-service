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


class UserModel : public xw::orm::db::Model,
				  public xw::IJsonSerializable
{
private:
	std::string _password_hash;

public:
	long long int id{};
	std::string email;
	xw::dt::Datetime created_at;
	xw::dt::Datetime updated_at;

	static constexpr const char* meta_table_name = "users";

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
	std::string to_string() const override;

	[[nodiscard]]
	nlohmann::json to_json() const override;

	inline void set_password(const std::string& raw_password)
	{
		this->_password_hash = calculate_hash(raw_password);
	}

	[[nodiscard]]
	inline bool check_password(const std::string& raw_password) const
	{
		return this->_password_hash == calculate_hash(raw_password);
	}

protected:
	static inline std::string calculate_hash(const std::string& data)
	{
		return xw::crypto::sha256(data);
	}
};
