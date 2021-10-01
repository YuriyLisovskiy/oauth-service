/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// Xalwart framework
#include <xalwart/controllers/controller.h>


namespace oauth2
{

class HelloController : public xw::ctrl::Controller<>
{
public:
	explicit HelloController(const xw::conf::Settings* settings) :
		xw::ctrl::Controller<>({"get"}, settings)
	{
	}

	inline std::unique_ptr<xw::http::abc::HttpResponse> get(xw::http::Request* request) const override
	{
		auto table_names = this->settings->DB->get_table_names();
		auto tables = nlohmann::json::array();
		for (const auto& table_name : table_names)
		{
			tables.push_back(table_name);
		}

		return std::make_unique<xw::http::JsonResponse>(nlohmann::json{
			{"tables", tables}
		});
	}
};

}
