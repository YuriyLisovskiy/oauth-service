/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.orm/db/migration.h>


class Migration002_CreateUser : public xw::orm::db::Migration
{
public:
	inline explicit Migration002_CreateUser(auto* backend)
		: xw::orm::db::Migration(backend, "002_create_user", true)
	{
		this->create_table(
			"user",
			[](auto& table)
			{
				table.template column<long long int>(
					"id", {.primary_key=true, .autoincrement=true, .unique=true, .null=false}
				);
				table.template column<std::string>("email", {.max_len=320, .null=false});
				table.template column<std::string>("password_hash", {.max_len=64, .null=false});
				table.template column<xw::dt::Datetime>("created_at", {.null=false});
				table.template column<xw::dt::Datetime>("updated_at", {.null=false});
			}
		);
	}
};
