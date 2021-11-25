/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.orm/db/migration.h>


class Migration001_CreateClients : public xw::orm::db::Migration
{
public:
	inline explicit Migration001_CreateClients(xw::orm::IBackend* backend)
		: xw::orm::db::Migration(backend, "001_create_clients", true)
	{
		this->create_table(
			"oauth_clients",
			[](auto& table)
			{
				table.template column<std::string>(
					"client_id", {.max_len=255, .null=false, .primary_key=true, .unique=true}
				);
				table.template column<std::string>("client_secret", {.max_len=64, .null=false});
				table.template column<xw::dt::Datetime>("created_at", {.null=false});
				table.template column<xw::dt::Datetime>("updated_at", {.null=false});
			}
		);
	}
};
