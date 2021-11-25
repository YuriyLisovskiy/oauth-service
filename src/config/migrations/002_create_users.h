/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.orm/db/migration.h>


class Migration002_CreateUsers : public xw::orm::db::Migration
{
public:
	inline explicit Migration002_CreateUsers(xw::orm::IBackend* backend)
		: xw::orm::db::Migration(backend, "002_create_users", true)
	{
		this->create_table(
			"users",
			[](auto& table)
			{
				table.template column<long long int>(
					"id", {.null=false, .primary_key=true, .unique=true, .autoincrement=true}
				);
				table.template column<std::string>("email", {.max_len=320, .null=false});
				table.template column<std::string>("password_hash", {.max_len=64, .null=false});
				table.template column<xw::dt::Datetime>("created_at", {.null=false});
				table.template column<xw::dt::Datetime>("updated_at", {.null=false});
			}
		);
	}
};
